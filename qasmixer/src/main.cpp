/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "desktop_items.hpp"
#include "init_globals.hpp"
#include "qastools_config.hpp"
#include "single_application.hpp"
#include "views/view_utility.hpp"
#include <QDesktopWidget>
#include <QFile>
#include <QFileInfo>
#include <QIcon>
#include <QString>
#include <iostream>
#include <memory>

/// @brief The main function
///
int
main ( int argc, char * argv[] )
{
  init_globals ();

  // Qt Application
  Single_Application app ( argc, argv );
  app.setOrganizationName ( QString ( PACKAGE_NAME ).toLower () );
  app.setApplicationName ( QString ( PROGRAM_NAME ).toLower () );
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

  // If an other instance is running send a message to it and quit
  if ( ditems.cmd_opts ().start_single_instance ) {
    app.set_unique_key ( SINGLE_APPLICATION_KEY );

    if ( app.is_running () ) {
      if ( app.send_message ( ditems.message_to_other_instance () ) ) {
        return 0;
      }
    }
  }

  // Connect single instance signals with desktop item manager
  {
    QObject::connect ( &app,
                       SIGNAL ( sig_message_available ( QString ) ),
                       &ditems,
                       SLOT ( parse_message ( QString ) ) );

    QObject::connect ( &app,
                       SIGNAL ( commitDataRequest ( QSessionManager & ) ),
                       &ditems,
                       SLOT ( shutdown () ) );

    QObject::connect ( &ditems,
                       SIGNAL ( sig_quit () ),
                       &app,
                       SLOT ( quit () ),
                       Qt::QueuedConnection );
  }

  // Load application icon, translators
  ::Views::load_application_icon ( &app, "multimedia-volume-control" );
  ::Views::load_translators ( &app );

  // Start and restore session (on demand)
  ditems.start ( app.isSessionRestored () );

  return app.exec ();
}
