/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "main_window.hpp"
#include "qastools_config.hpp"
#include "qsnd/alsa_config_watcher.hpp"
#include "views/info_dialog.hpp"
#include "views/view_utility.hpp"
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include <QVBoxLayout>
#include <iostream>

Main_Window::Main_Window ()
{
  setWindowTitle ( PROGRAM_TITLE );
  setObjectName ( PROGRAM_TITLE );
  setContextMenuPolicy ( Qt::NoContextMenu );

  init_menu_bar ();

  {
    ::QSnd::ALSA_Config_Watcher * watcher (
        new ::QSnd::ALSA_Config_Watcher ( this ) );

    connect ( watcher, SIGNAL ( sig_change () ), this, SLOT ( refresh () ) );
  }

  _alsa_cfg_view.set_model ( &_alsa_cfg_model );

  setCentralWidget ( &_alsa_cfg_view );
}

QSize
Main_Window::sizeHint () const
{
  QSize res ( QMainWindow::sizeHint () );
  ::Views::win_default_size ( res );
  return res;
}

void
Main_Window::restore_state ()
{
  QSettings settings;

  QByteArray mwin_state;
  QByteArray mwin_geom;
  bool cfg_sorted;

  // Main window state
  {
    settings.beginGroup ( "main_window" );
    mwin_state =
        settings.value ( "window_state", QByteArray () ).toByteArray ();
    mwin_geom =
        settings.value ( "window_geometry", QByteArray () ).toByteArray ();
    settings.endGroup ();
  }

  // Configuration view
  {
    settings.beginGroup ( "alsa_config" );
    cfg_sorted = settings.value ( "sorting_enabled", true ).toBool ();
    settings.endGroup ();
  }

  // Apply settings
  restoreState ( mwin_state );
  if ( !restoreGeometry ( mwin_geom ) ) {
    ::Views::resize_to_default ( this );
  }
  _alsa_cfg_view.set_sorting_enabled ( cfg_sorted );
}

void
Main_Window::save_state ()
{
  QSettings settings;

  settings.beginGroup ( "main_window" );
  settings.setValue ( "window_state", saveState () );
  settings.setValue ( "window_geometry", saveGeometry () );
  settings.endGroup ();

  settings.beginGroup ( "alsa_config" );
  settings.setValue ( "sorting_enabled", _alsa_cfg_view.sorting_enabled () );
  settings.endGroup ();
}

void
Main_Window::show_info_dialog ()
{
  if ( _info_dialog == 0 ) {
    ::Views::Info_Dialog * dlg ( new ::Views::Info_Dialog ( this ) );
    dlg->setAttribute ( Qt::WA_DeleteOnClose );

    _info_dialog = dlg;
  }

  _info_dialog->show ();
}

void
Main_Window::init_menu_bar ()
{
  // Action: Quit
  QAction * act_quit ( new QAction ( tr ( "&Quit" ), this ) );
  act_quit->setShortcut ( QKeySequence ( QKeySequence::Quit ) );
  act_quit->setIcon ( QIcon::fromTheme ( "application-exit" ) );

  // Action: Refresh
  QAction * act_refresh ( new QAction ( tr ( "&Refresh" ), this ) );
  act_refresh->setShortcut ( QKeySequence ( QKeySequence::Refresh ) );
  act_refresh->setIcon ( QIcon::fromTheme ( "view-refresh" ) );

  // Action: Info
  QAction * act_info = new QAction ( tr ( "&Info" ), this );
  act_info->setShortcut ( QKeySequence ( QKeySequence::HelpContents ) );
  act_info->setIcon ( QIcon::fromTheme ( "help-about" ) );

  // Menu: File
  {
    QMenu * cmenu ( menuBar ()->addMenu ( tr ( "&File" ) ) );
    cmenu->addAction ( act_quit );
  }

  // Menu: View
  {
    QMenu * cmenu = menuBar ()->addMenu ( tr ( "&View" ) );
    cmenu->addAction ( act_refresh );
  }

  // Menu: Help
  {
    QMenu * cmenu = menuBar ()->addMenu ( tr ( "&Help" ) );
    cmenu->addAction ( act_info );
  }

  // Connect actions

  connect ( act_quit, SIGNAL ( triggered () ), this, SLOT ( close () ) );

  connect ( act_refresh, SIGNAL ( triggered () ), this, SLOT ( refresh () ) );

  connect (
      act_info, SIGNAL ( triggered () ), this, SLOT ( show_info_dialog () ) );
}

void
Main_Window::refresh ()
{
  //::std::cout << "Refresh" << "\n";
  _alsa_cfg_view.reload_config ();
}

void
Main_Window::closeEvent ( QCloseEvent * event_n )
{
  save_state ();
  QMainWindow::closeEvent ( event_n );
}
