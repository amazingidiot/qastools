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

#include "config.hpp"
#include "views/view_helper.hpp"
#include "main_window.hpp"

#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QDesktopWidget>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QIcon>
#include <QApplication>


/// @brief The main function
///
int
main (
	int argc,
	char * argv[] )
{

	// QT Application
	QApplication app ( argc, argv );
	app.setOrganizationName ( QString ( PACKAGE_NAME ).toLower() );
	app.setApplicationName ( QString ( PROGRAM_NAME ).toLower() );
	app.setAttribute ( Qt::AA_DontShowIconsInMenus, false );

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
		{
			QString l10n_db ( "qt_" );
			l10n_db.append ( QLocale::system().name() );
			trans_qt->load ( l10n_db,
				QLibraryInfo::location ( QLibraryInfo::TranslationsPath ) );
		}
		app.installTranslator ( trans_qt );
	}
	{
		QTranslator * trans_local ( new QTranslator ( &app ) );
		{
			QString l10n_db ( L10N_PREFIX );
			l10n_db.append ( QLocale::system().name() );
			trans_local->load ( l10n_db, INSTALL_DIR_L10N );
		}
		app.installTranslator ( trans_local );
	}

	Main_Window mwin;
	mwin.restore_state();
	mwin.show();

	return app.exec();
}

