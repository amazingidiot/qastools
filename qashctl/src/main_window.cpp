/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "main_window.hpp"
#include "mwdg/mixer_style.hpp"
#include "qastools_config.hpp"
#include "qsnd/alsa_config_watcher.hpp"
#include "views/device_selection_view.hpp"
#include "views/info_dialog.hpp"
#include "views/view_utility.hpp"
#include "wdg/ds_slider_painter_bevelled.hpp"
#include "wdg/ds_switch_painter_circle.hpp"
#include "wdg/ds_switch_painter_svg.hpp"
#include <QDockWidget>
#include <QEvent>
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

  // Strings and icons
  _str_fscreen_enable = tr ( "&Fullscreen mode" );
  _str_fscreen_disable = tr ( "Exit &fullscreen mode" );
  _icon_fscreen_enable = QIcon::fromTheme ( "view-fullscreen" );
  _icon_fscreen_disable = QIcon::fromTheme ( "view-restore" );

  // File watcher
  {
    ::QSnd::ALSA_Config_Watcher * watcher (
        new ::QSnd::ALSA_Config_Watcher ( this ) );

    connect ( watcher, SIGNAL ( sig_change () ), this, SLOT ( refresh () ) );
  }

  // Slider painter bevelled
  {
    ::Wdg::Painter::DS_Slider_Painter_Bevelled * pnt (
        new ::Wdg::Painter::DS_Slider_Painter_Bevelled );
    pnt->set_wdg_style_db ( &_wdg_style_db );
    _image_alloc.install_painter ( pnt );
  }

  // Switch painter circle
  {
    ::Wdg::Painter::DS_Switch_Painter_Circle * pnt (
        new ::Wdg::Painter::DS_Switch_Painter_Circle );
    pnt->set_group_variant ( ::Wdg::DS_CIRCLE );
    pnt->set_wdg_style_db ( &_wdg_style_db );
    _image_alloc.install_painter ( pnt );
  }
  // Switch painter SVG
  {
    QScopedPointer<::Wdg::Painter::DS_Switch_Painter_SVG > pnt (
        new ::Wdg::Painter::DS_Switch_Painter_SVG );
    pnt->set_group_variant ( ::Wdg::DS_SVG_JOINED );
    pnt->set_wdg_style_db ( &_wdg_style_db );
    pnt->set_base_dir ( INSTALL_DIR_WIDGETS_GRAPHICS );
    pnt->set_file_prefix_bg ( "sw_joined_bg_" );
    pnt->set_file_prefix_handle ( "sw_joined_handle_" );
    if ( pnt->ready () ) {
      _image_alloc.install_painter ( pnt.take () );
    }
  }

  // Init widget style database
  {
    ::MWdg::Mixer_Style mstyle;
    ::MWdg::Mixer_Style::Style_Type stype;

    stype = ::MWdg::Mixer_Style::PLAYBACK;
    _wdg_style_db.palettes.insert ( stype, mstyle.style_palette ( stype ) );

    stype = ::MWdg::Mixer_Style::CAPTURE;
    _wdg_style_db.palettes.insert ( stype, mstyle.style_palette ( stype ) );
  }

  // Init setup
  {
    _setup.hctl.wdg_style_db = &_wdg_style_db;
    _setup.hctl.image_alloc = &_image_alloc;
  }

  // Widgets and docks
  init_widgets ();
  init_menu_bar ();

  update_fullscreen_action ();
}

void
Main_Window::init_widgets ()
{
  // Device selection
  {
    // QueuedConnection to update the GUI before loading the mixer
    _dev_select = new ::Views::Device_Selection_View;
    _dev_select->hide ();

    connect ( _dev_select,
              SIGNAL ( sig_control_selected () ),
              this,
              SLOT ( select_ctl_from_side_iface () ) );

    connect ( _dev_select,
              SIGNAL ( sig_close () ),
              this,
              SLOT ( toggle_device_selection () ) );
  }

  // Central mixer
  {
    _mixer_hctl = new ::Views::Mixer_HCTL;
  }

  // Central splitter
  {
    _splitter.reset ( new QSplitter );
    _splitter->addWidget ( _mixer_hctl );
    _splitter->addWidget ( _dev_select );
    _splitter->setStretchFactor ( 0, 1 );
    _splitter->setStretchFactor ( 1, 0 );
    _splitter->setCollapsible ( 0, false );
    _splitter->setCollapsible ( 1, false );
    setCentralWidget ( _splitter.data () );
  }
}

void
Main_Window::init_menu_bar ()
{
  // Action: Quit
  QAction * act_quit ( new QAction ( tr ( "&Quit" ), this ) );
  act_quit->setShortcut ( QKeySequence ( QKeySequence::Quit ) );
  act_quit->setIcon ( QIcon::fromTheme ( "application-exit" ) );

  // Action: Device selection
  _act_show_dev_select = new QAction ( this );
  _act_show_dev_select->setText ( tr ( "Show &device selection" ) );
  _act_show_dev_select->setCheckable ( true );
  _act_show_dev_select->setShortcut ( _setup.dev_select.kseq_toggle_vis );

  // Action: Fullscreen
  _act_fullscreen = new QAction ( this );
  _act_fullscreen->setShortcut ( QKeySequence ( Qt::Key_F11 ) );
  _act_fullscreen->setCheckable ( true );

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
    cmenu->addAction ( _act_show_dev_select );
    cmenu->addAction ( _act_fullscreen );
    cmenu->addSeparator ();
    cmenu->addAction ( act_refresh );
  }

  // Menu: Help
  {
    QMenu * menu = menuBar ()->addMenu ( tr ( "&Help" ) );
    menu->addAction ( act_info );
  }

  // Connect actions

  connect ( act_quit, SIGNAL ( triggered () ), this, SLOT ( close () ) );

  connect ( _act_show_dev_select,
            SIGNAL ( toggled ( bool ) ),
            this,
            SLOT ( show_device_selection ( bool ) ) );

  connect ( _act_fullscreen,
            SIGNAL ( toggled ( bool ) ),
            this,
            SLOT ( set_fullscreen ( bool ) ) );

  connect ( act_refresh, SIGNAL ( triggered () ), this, SLOT ( refresh () ) );

  connect (
      act_info, SIGNAL ( triggered () ), this, SLOT ( show_info_dialog () ) );
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
  QByteArray mwin_splitter;

  // Main window state
  {
    settings.beginGroup ( "main_window" );
    {
      mwin_state =
          settings.value ( "window_state", QByteArray () ).toByteArray ();

      mwin_geom =
          settings.value ( "window_geometry", QByteArray () ).toByteArray ();

      mwin_splitter =
          settings.value ( "splitter_state", QByteArray () ).toByteArray ();
    }

    _setup.show_dev_select =
        settings.value ( "show_device_selection", _setup.show_dev_select )
            .toBool ();
    settings.endGroup ();
  }

  // General
  {
    _setup.mixer_dev.ctl_addr =
        settings.value ( "current_device", _setup.mixer_dev.ctl_addr )
            .toString ();

    _setup.inputs.wheel_degrees =
        settings.value ( "wheel_degrees", _setup.inputs.wheel_degrees )
            .toUInt ();
  }

  // CTL mixer
  {
    settings.beginGroup ( "element_mixer" );

    _setup.hctl.iface_name =
        settings.value ( "iface_name", _setup.hctl.iface_name ).toString ();

    _setup.hctl.elem_grp_name =
        settings.value ( "elem_grp_name", _setup.hctl.elem_grp_name )
            .toString ();

    _setup.hctl.elem_grp_index =
        settings.value ( "elem_grp_index", _setup.hctl.elem_grp_index )
            .toUInt ();

    settings.endGroup ();
  }

  // Device selection
  {
    settings.beginGroup ( "device_selection" );

    _setup.dev_select.selection_db_set (
        settings.value ( "selection_db", _setup.dev_select.selection_db_get () )
            .toStringList () );

    settings.endGroup ();
  }

  // Sanitize values

  if ( _setup.mixer_dev.ctl_addr.isEmpty () ) {
    _setup.mixer_dev.ctl_addr = "default";
  }

  if ( _setup.inputs.wheel_degrees == 0 ) {
    _setup.inputs.wheel_degrees = 720;
  }

  _setup.inputs.update_translation ();

  // Apply values
  restoreState ( mwin_state );
  if ( !restoreGeometry ( mwin_geom ) ) {
    ::Views::resize_to_default ( this );
  }
  _splitter->restoreState ( mwin_splitter );
  update_fullscreen_action ();
  show_device_selection ( _setup.show_dev_select );
  _dev_select->set_view_setup ( &_setup.dev_select );
  _dev_select->silent_select_ctl ( _setup.mixer_dev.ctl_addr );
  _mixer_hctl->set_inputs_setup ( &_setup.inputs );
  _mixer_hctl->set_mdev_setup ( &_setup.mixer_dev );
  _mixer_hctl->set_view_setup ( &_setup.hctl );
}

void
Main_Window::save_state ()
{
  QSettings settings;

  // Main window state
  {
    settings.beginGroup ( "main_window" );
    settings.setValue ( "window_state", saveState () );
    settings.setValue ( "window_geometry", saveGeometry () );
    settings.setValue ( "splitter_state", _splitter->saveState () );
    settings.setValue ( "show_device_selection", _setup.show_dev_select );
    settings.endGroup ();
  }

  settings.setValue ( "current_device", _setup.mixer_dev.ctl_addr );
  settings.setValue ( "wheel_degrees", _setup.inputs.wheel_degrees );

  // CTL mixer
  {
    settings.beginGroup ( "element_mixer" );
    settings.setValue ( "iface_name", _setup.hctl.iface_name );
    settings.setValue ( "elem_grp_name", _setup.hctl.elem_grp_name );
    settings.setValue ( "elem_grp_index", _setup.hctl.elem_grp_index );
    settings.endGroup ();
  }

  // Device selection
  {
    settings.beginGroup ( "device_selection" );
    settings.setValue ( "selection_db", _setup.dev_select.selection_db_get () );
    settings.endGroup ();
  }
}

void
Main_Window::refresh ()
{
  //::std::cout << "Refresh" << "\n";
  _mixer_hctl->set_mdev_setup ( 0 );
  _mixer_hctl->set_mdev_setup ( &_setup.mixer_dev );
}

void
Main_Window::select_snd_ctl ( const QString & ctl_n )
{
  if ( ctl_n != _setup.mixer_dev.ctl_addr ) {
    _mixer_hctl->set_mdev_setup ( 0 );
    _setup.mixer_dev.ctl_addr = ctl_n;
    _mixer_hctl->set_mdev_setup ( &_setup.mixer_dev );
  }
}

void
Main_Window::select_ctl_from_side_iface ()
{
  select_snd_ctl ( _dev_select->selected_ctl ().addr_str () );
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
  if ( _setup.show_dev_select != flag_n ) {
    _setup.show_dev_select = flag_n;
  }
  _act_show_dev_select->setChecked ( flag_n );
  _dev_select->setVisible ( flag_n );
}

void
Main_Window::toggle_device_selection ()
{
  _act_show_dev_select->setChecked ( !_act_show_dev_select->isChecked () );
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
Main_Window::changeEvent ( QEvent * event_n )
{
  QMainWindow::changeEvent ( event_n );
  if ( event_n->type () == QEvent::WindowStateChange ) {
    update_fullscreen_action ();
  }
}

void
Main_Window::closeEvent ( QCloseEvent * event_n )
{
  save_state ();
  QMainWindow::closeEvent ( event_n );
}
