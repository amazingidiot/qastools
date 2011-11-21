//
// C++ Implementation:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include <iostream>
#include <memory>

#include <QTranslator>
#include <QLibraryInfo>
#include <QDesktopWidget>
#include <QString>
#include <QFile>
#include <QFileInfo>

#include "single_application.hpp"
#include "mixer_window.hpp"

#include "init_globals.hpp"
#include "desktop_items.hpp"



/// @brief The main function
///
int
main (
	int argc,
	char * argv[] )
{
	init_globals();


	// Qt Application

	Single_Application app ( argc, argv );
	app.setOrganizationName ( QString ( PACKAGE_NAME ).toLower() );
	app.setApplicationName ( PROGRAM_NAME );
	app.setQuitOnLastWindowClosed ( false );
	app.setAttribute ( Qt::AA_DontShowIconsInMenus, false );

	Desktop_Items ditems;

	// Parse remaining command line options
	{
		const int res ( ditems.init_settings ( argc, argv ) );
		if ( res < 0 ) {
			return res;
		}
	}

	// Send a message and quit if an other instance is running
	if ( ditems.cmd_opts().start_single_instance ) {
		app.set_unique_key ( SINGLE_APPLICATION_KEY );

		if ( app.is_running() ) {
			if ( app.send_message ( ditems.new_instance_message() ) ) {
				return 0;
			}
		}

	}

	// Application icon setup
	{
		QIcon icon;
		{
			// Try application icon
			QString icon_path ( INSTALL_DIR_ICONS_SVG );
			icon_path += "/";
			icon_path += PROGRAM_NAME;
			icon_path += ".svg";

			QFileInfo finfo ( icon_path );
			if ( finfo.exists() && finfo.isReadable() ) {
				icon = QIcon ( icon_path );
			}
		}
		if ( icon.isNull() ) {
			// Try system theme icon
			const QString iname ( "multimedia-volume-control" );
			if ( QIcon::hasThemeIcon ( iname  ) ) {
				icon = QIcon::fromTheme ( iname  );
			}
		}
		if ( !icon.isNull() ) {
			app.setWindowIcon ( icon );
		}
	}

	// Application translators setup
	{
		QTranslator * trans_qt ( new QTranslator ( &app ) );
		QTranslator * trans_default ( new QTranslator ( &app ) );
		QTranslator * trans_local ( new QTranslator ( &app ) );
		{
			trans_qt->load ( "qt_" + QLocale::system().name(),
				QLibraryInfo::location ( QLibraryInfo::TranslationsPath ) );

			trans_default->load ( "app_C",
				INSTALL_DIR_L10N );
			trans_local->load ( "app_" + QLocale::system().name(),
				INSTALL_DIR_L10N );

			app.installTranslator ( trans_qt );
			app.installTranslator ( trans_default );
			app.installTranslator ( trans_local );
		}
	}

	// Connect single instance signals with desktop item manager
	{
		QObject::connect ( &app, SIGNAL ( sig_message_available ( QString ) ),
			&ditems, SLOT ( parse_message ( QString ) ) );

		QObject::connect ( &app, SIGNAL ( commitDataRequest ( QSessionManager & ) ),
			&ditems, SLOT ( shutdown() ) );

		QObject::connect ( &ditems, SIGNAL ( sig_quit() ),
			&app, SLOT ( quit() ), Qt::QueuedConnection );
	}

	// Start and restore session (on demand)
	ditems.start ( app.isSessionRestored() );

	return app.exec();
}

