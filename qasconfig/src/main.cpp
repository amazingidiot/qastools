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
#include "desktop_items.hpp"
#include "views/view_helper.hpp"

#include <QString>
#include <QApplication>
#include <iostream>


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

	Desktop_Items ditems;
	{
		int res ( ditems.init_settings ( argc, argv ) );
		if ( res < ditems.init_settings ( argc, argv ) ) {
			return res;
		}
	}

	// Load application icon, translators
	::Views::load_application_icon ( &app, "multimedia-volume-control" );
	::Views::load_translators ( &app );

	ditems.start();

	return app.exec();
}

