/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "main_window.hpp"
#include "views/device_selection_view.hpp"
#include "views/mixer_simple.hpp"
#include "views/view_utility.hpp"
#include <QCloseEvent>
#include <QFile>
#include <QIcon>
#include <QLabel>
#include <QMenuBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <iostream>

Main_Window::Main_Window ( QWidget * parent_n, Qt::WindowFlags flags_n )
: QMainWindow ( parent_n, flags_n )
, _win_setup ( 0 )
, _mixer_simple ( 0 )
, _dev_select ( 0 )
{
  setWindowTitle ( PROGRAM_TITLE );
  setObjectName ( PROGRAM_TITLE );
  setContextMenuPolicy ( Qt::NoContextMenu );

  _str_fscreen_enable = tr ( "&Fullscreen mode" );
  _str_fscreen_disable = tr ( "Exit &fullscreen mode" );
  _icon_fscreen_enable = QIcon::fromTheme ( "view-fullscreen" );
  _icon_fscreen_disable = QIcon::fromTheme ( "view-restore" );

  // Init menus
  init_menus ();
  init_widgets ();

  update_fullscreen_action ();
}

Main_Window::~Main_Window () {}

void
Main_Window::init_menus ()
{
  // Action: Exit / Quit
  QAction * act_quit = new QAction ( tr ( "&Quit" ), this );
  act_quit->setShortcut ( QKeySequence ( QKeySequence::Quit ) );
  act_quit->setIcon ( QIcon::fromTheme ( "application-exit" ) );

  // Action: Settings
  QAction * act_settings = new QAction ( tr ( "&Settings" ), this );
  act_settings->setShortcut ( QKeySequence ( tr ( "Ctrl+s" ) ) );
  act_settings->setIcon ( QIcon::fromTheme ( "preferences-desktop" ) );

  // Action: Refresh
  QAction * act_refresh = new QAction ( tr ( "&Refresh" ), this );
  act_refresh->setShortcut ( QKeySequence ( QKeySequence::Refresh ) );
  act_refresh->setIcon ( QIcon::fromTheme ( "view-refresh" ) );

  // Action: Device selection
  _act_show_dev_select = new QAction ( this );
  _act_show_dev_select->setText ( tr ( "Show &device selection" ) );
  _act_show_dev_select->setCheckable ( true );

  // Action: Fullscreen
  _act_fullscreen = new QAction ( this );
  _act_fullscreen->setShortcut ( QKeySequence ( Qt::Key_F11 ) );
  _act_fullscreen->setCheckable ( true );

  // Action: Info
  QAction * act_info = new QAction ( tr ( "&Info" ), this );
  act_info->setShortcut ( QKeySequence ( QKeySequence::HelpContents ) );
  act_info->setIcon ( QIcon::fromTheme ( "help-about" ) );

  // Menus
  {
    QMenu * cmenu = menuBar ()->addMenu ( tr ( "&File" ) );
    cmenu->addAction ( act_settings );
    cmenu->addSeparator ();
    cmenu->addAction ( act_quit );
  }
  {
    QMenu * cmenu = menuBar ()->addMenu ( tr ( "&View" ) );
    cmenu->addAction ( _act_show_dev_select );
    cmenu->addAction ( _act_fullscreen );
    cmenu->addSeparator ();
    cmenu->addAction ( act_refresh );
  }
  {
    QMenu * cmenu = menuBar ()->addMenu ( tr ( "&Help" ) );
    cmenu->addAction ( act_info );
  }

  // Connect actions

  connect ( act_quit, SIGNAL ( triggered () ), this, SIGNAL ( sig_quit () ) );

  // View actions

  connect ( _act_show_dev_select,
            SIGNAL ( toggled ( bool ) ),
            this,
            SLOT ( show_device_selection ( bool ) ) );

  connect (
      act_refresh, SIGNAL ( triggered () ), this, SLOT ( refresh_views () ) );

  connect ( _act_fullscreen,
            SIGNAL ( toggled ( bool ) ),
            this,
            SLOT ( set_fullscreen ( bool ) ) );

  // Info action

  connect ( act_settings,
            SIGNAL ( triggered () ),
            this,
            SIGNAL ( sig_show_settings () ) );

  connect (
      act_info, SIGNAL ( triggered () ), this, SIGNAL ( sig_show_info () ) );
}

void
Main_Window::init_widgets ()
{
  // Device selection
  {
    _dev_select = new ::Views::Device_Selection_View;
    _dev_select->hide ();

    // QueuedConnection to update the GUI before loading the mixer
    connect ( _dev_select,
              SIGNAL ( sig_control_selected () ),
              this,
              SLOT ( select_ctl_from_side_iface () ),
              Qt::QueuedConnection );

    connect ( _dev_select,
              SIGNAL ( sig_control_reload () ),
              this,
              SLOT ( reload_mixer_device () ),
              Qt::QueuedConnection );

    connect ( _dev_select,
              SIGNAL ( sig_close () ),
              this,
              SLOT ( toggle_device_selection () ) );
  }

  // Central mixer
  {
    _mixer_simple = new ::Views::Mixer_Simple;
  }

  // Central splitter
  {
    _splitter.reset ( new QSplitter );
    _splitter->addWidget ( _mixer_simple );
    _splitter->addWidget ( _dev_select );
    _splitter->setStretchFactor ( 0, 1 );
    _splitter->setStretchFactor ( 1, 0 );
    _splitter->setCollapsible ( 0, false );
    _splitter->setCollapsible ( 1, false );
    setCentralWidget ( _splitter.data () );
  }
}

QSize
Main_Window::sizeHint () const
{
  QSize res ( QMainWindow::sizeHint () );
  ::Views::win_default_size ( res );
  return res;
}

void
Main_Window::set_window_setup ( Main_Window_Setup * setup_n )
{
  if ( _win_setup != 0 ) {
    _mixer_simple->set_mdev_setup ( 0 );
    _mixer_simple->set_inputs_setup ( 0 );
    _mixer_simple->set_view_setup ( 0 );
    _dev_select->set_view_setup ( 0 );
  }

  _win_setup = setup_n;

  if ( _win_setup != 0 ) {
    // Restore mixer window state
    restoreState ( _win_setup->window_state );
    if ( !restoreGeometry ( _win_setup->window_geometry ) ) {
      ::Views::resize_to_default ( this );
    }

    _splitter->restoreState ( _win_setup->splitter_state );

    // Actions
    _act_show_dev_select->setShortcut (
        _win_setup->dev_select.kseq_toggle_vis );
    _act_show_dev_select->setChecked ( _win_setup->show_dev_select );

    // Pass setup tree to child classes
    _dev_select->set_view_setup ( &_win_setup->dev_select );
    _dev_select->silent_select_ctl ( _win_setup->mixer_dev.ctl_addr );

    _mixer_simple->set_view_setup ( &_win_setup->mixer_simple );
    _mixer_simple->set_inputs_setup ( &_win_setup->inputs );
    _mixer_simple->set_mdev_setup ( &_win_setup->mixer_dev );
  }
}

void
Main_Window::update_fullscreen_action ()
{
  QString * txt;
  QIcon * icon;
  bool checked;

  if ( isFullScreen () ) {
    txt = &_str_fscreen_disable;
    icon = &_icon_fscreen_disable;
    checked = true;
  } else {
    txt = &_str_fscreen_enable;
    icon = &_icon_fscreen_enable;
    checked = false;
  }

  _act_fullscreen->setText ( *txt );
  _act_fullscreen->setIcon ( *icon );
  _act_fullscreen->setChecked ( checked );
}

void
Main_Window::select_ctl ( const QString & ctl_n )
{
  //::std::cout << "Main_Window::select_ctl " << ctl_n.toLocal8Bit().data() <<
  //"\n";

  _dev_select->silent_select_ctl ( ctl_n );

  if ( _win_setup != 0 ) {
    if ( _win_setup->mixer_dev.ctl_addr != ctl_n ) {
      // Remove
      _mixer_simple->set_mdev_setup ( 0 );
      // Change
      _win_setup->mixer_dev.ctl_addr = ctl_n;
      // Reinstall
      _mixer_simple->set_mdev_setup ( &_win_setup->mixer_dev );
    }
  }

  emit sig_control_changed ();
}

void
Main_Window::select_ctl_from_side_iface ()
{
  //::std::cout << "Main_Window::select_ctl_from_side_iface " << "\n";

  select_ctl ( _dev_select->selected_ctl ().addr_str () );
  emit sig_control_changed ();
}

void
Main_Window::reload_mixer_device ()
{
  //::std::cout << "Main_Window::reload_mixer_device" << "\n";
  _mixer_simple->set_mdev_setup ( 0 );
  _mixer_simple->set_mdev_setup ( &_win_setup->mixer_dev );
}

void
Main_Window::reload_mixer_inputs ()
{
  //::std::cout << "Main_Window::reload_mixer_inputs" << "\n";
  _mixer_simple->set_inputs_setup ( 0 );
  _mixer_simple->set_inputs_setup ( &_win_setup->inputs );
}

void
Main_Window::refresh_views ()
{
  _dev_select->reload_database ();
  reload_mixer_device ();
}

void
Main_Window::reload_mixer_view ()
{
  //::std::cout << "Main_Window::reload_mixer_view" << "\n";
  _mixer_simple->set_view_setup ( 0 );
  _mixer_simple->set_view_setup ( &_win_setup->mixer_simple );
}

void
Main_Window::set_fullscreen ( bool flag_n )
{
  if ( flag_n != isFullScreen () ) {
    if ( flag_n ) {
      showFullScreen ();
    } else {
      showNormal ();
    }
    update_fullscreen_action ();
  }
}

void
Main_Window::show_device_selection ( bool flag_n )
{
  if ( _win_setup != 0 ) {
    if ( _win_setup->show_dev_select != flag_n ) {
      _win_setup->show_dev_select = flag_n;
    }
  }
  _dev_select->setVisible ( flag_n );
}

void
Main_Window::toggle_device_selection ()
{
  _act_show_dev_select->setChecked ( !_act_show_dev_select->isChecked () );
}

void
Main_Window::save_state ()
{
  if ( _win_setup != 0 ) {
    _win_setup->window_state = saveState ();
    _win_setup->window_geometry = saveGeometry ();
    _win_setup->splitter_state = _splitter->saveState ();
  }
}

void
Main_Window::changeEvent ( QEvent * event_n )
{
  QMainWindow::changeEvent ( event_n );
  if ( event_n->type () == QEvent::WindowStateChange ) {
    update_fullscreen_action ();
  }
}

void
Main_Window::keyPressEvent ( QKeyEvent * event_n )
{
  QMainWindow::keyPressEvent ( event_n );
  if ( _win_setup != 0 ) {
    const QKeySequence kseq ( event_n->key () );
    if ( kseq == _win_setup->dev_select.kseq_toggle_vis ) {
      toggle_device_selection ();
    }
  }
}
