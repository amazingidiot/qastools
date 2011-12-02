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

#include "qastools_config.hpp"
#include "init_globals.hpp"
#include "single_application.hpp"
#include "desktop_items.hpp"
#include "views/view_helper.hpp"

#include <QDesktopWidget>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QIcon>

#include <iostream>
#include <memory>


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
			if ( app.send_message ( ditems.message_to_other_instance() ) ) {
				return 0;
			}
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

	// Translation loading
	::Views::load_translators ( &app );

	// Start and restore session (on demand)
	ditems.start ( app.isSessionRestored() );

	return app.exec();
}

