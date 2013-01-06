//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "qastools_config.hpp"
#include "desktop_items.hpp"
#include "views/view_utility.hpp"

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

