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

#include "qastools_config.hpp"
#include "main_window.hpp"
#include "views/view_helper.hpp"

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
	app.setOrganizationName ( QString ( PROGRAM_NAME ).toLower() );
	app.setApplicationName ( PROGRAM_NAME );
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

	// Translation loading
	::Views::load_translators ( &app );

	Main_Window mwin;

	// Restore mixer window state
	bool size_restored = false;
	//size_restored =  mwin->restoreGeometry (
	//	_dsetup.main_window.window_geometry );

	// Adjust startup size
	if ( !size_restored ) {
		::Views::resize_to_default ( &mwin );
	}

	mwin.show();

	return app.exec();
}

