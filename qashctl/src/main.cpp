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
#include "views/view_helper.hpp"
#include "desktop_items.hpp"

#include <QString>
#include <QApplication>


/// @brief The main function
///
int
main (
	int argc,
	char * argv[] )
{
	// Qt Application
	QApplication app ( argc, argv );
	app.setOrganizationName ( QString ( PACKAGE_NAME ).toLower() );
	app.setApplicationName ( QString ( PROGRAM_NAME ).toLower() );
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

	ditems.start ( app.isSessionRestored() );

	return app.exec();
}

