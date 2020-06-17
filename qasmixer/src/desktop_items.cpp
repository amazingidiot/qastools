/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "desktop_items.hpp"
#include "info_texts.hpp"
#include "main_window.hpp"
#include "qastools_config.hpp"
#include "tray_mixer.hpp"
#include "views/info_dialog.hpp"
#include "views/settings_dialog.hpp"
#include "wdg/ds_slider_painter_bevelled.hpp"
#include "wdg/ds_switch_painter_circle.hpp"
#include "wdg/ds_switch_painter_close.hpp"
#include "wdg/ds_switch_painter_svg.hpp"
#include "wdg/ds_widget_types.hpp"
#include <getopt.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QEvent>
#include <QKeyEvent>
#include <iostream>
#include <sstream>

Desktop_Items::Desktop_Items ( QObject * parent_n )
: QObject ( parent_n )
, _tray_mixer ( 0 )
, _main_mixer ( 0 )
, _started ( false )
, _shutdown ( false )
{
  // Init widget style database
  {
    ::MWdg::Mixer_Style mstyle;
    ::MWdg::Mixer_Style::Style_Type stype;

    stype = ::MWdg::Mixer_Style::PLAYBACK;
    _wdg_style_db.palettes.insert ( stype, mstyle.style_palette ( stype ) );

    stype = ::MWdg::Mixer_Style::CAPTURE;
    _wdg_style_db.palettes.insert ( stype, mstyle.style_palette ( stype ) );
  }

  {
    _dsetup.main_window.mixer_simple.wdg_style_db = &_wdg_style_db;
    _dsetup.main_window.mixer_simple.image_alloc = &_image_alloc;
  }
  _dsetup.tray_view.image_alloc = &_image_alloc;

  _evt_mixer_closed = QEvent::registerEventType ();
}

Desktop_Items::~Desktop_Items ()
{
  if ( _started ) {
    shutdown ();
    _dsetup.write_to_storage ();
  }
}

int
Desktop_Items::init_settings ( int argc, char * argv[] )
{
  _dsetup.read_from_storage ();
  return parse_cmd_options ( argc, argv );
}

int
Desktop_Items::parse_cmd_options ( int argc, char * argv[] )
{
  // Suppresses error messages by getopt_long
  opterr = 0;

  QString card_idx;
  QString ctl_address;

  bool flag_print_version ( false );
  bool flag_print_help ( false );
  bool flag_print_copy_info ( false );

  bool scan_further ( true );

  while ( scan_further ) {

    static struct option long_opts[] = {{"help", no_argument, 0, 'h'},
                                        {"card", required_argument, 0, 'c'},
                                        {"device", required_argument, 0, 'D'},

                                        {"tray", no_argument, 0, 't'},
                                        {"no-single", no_argument, 0, 'n'},

                                        {"copying", no_argument, 0, 'i'},
                                        {"version", no_argument, 0, 'v'},
                                        {0, 0, 0, 0}};

    // getopt_long stores the option index here.
    int long_opts_idx ( 0 );
    int opt_char =
        getopt_long ( argc, argv, "hc:D:tniv", long_opts, &long_opts_idx );

    // Leave loop
    if ( opt_char < 0 ) {
      break;
    }

    switch ( opt_char ) {
    case 0:
      break;
    case 'h':
      flag_print_help = true;
      scan_further = false;
      break;
    case 'c':
      card_idx = optarg;
      break;
    case 'D':
      ctl_address = optarg;
      break;
    case 't':
      _cmd_opts.start_tray_minimized = true;
      break;
    case 'n':
      _cmd_opts.start_single_instance = false;
      break;
    case 'i':
      flag_print_copy_info = true;
      break;
    case 'v':
      flag_print_version = true;
      break;
    default:
      // Dont't break, as the option may be for QT
      break;
    }
  }

  if ( flag_print_help ) {
    {
      ::std::stringstream msg;
      msg << "Usage:" << ::std::endl;
      msg << "  " << PROGRAM_NAME << " [OPTION]..." << ::std::endl;
      msg << ::std::endl;
      msg << info_text_options;
      msg << ::std::endl;
      ::std::cout << msg.str () << ::std::flush;
    }
    return -1;
  }

  if ( flag_print_version ) {
    {
      ::std::stringstream msg;
      msg << PROGRAM_NAME << " " << VERSION << ::std::endl;
      ::std::cout << msg.str () << ::std::flush;
    }
    return -1;
  }

  if ( flag_print_copy_info ) {
    {
      ::std::stringstream msg;
      msg << PROGRAM_TITLE;
      msg << " - desktop mixer for the Linux sound system ALSA.";
      msg << ::std::endl;
      msg << ::std::endl;
      msg << license_text_short;
      msg << ::std::endl;
      ::std::cout << msg.str () << ::std::flush;
    }
    return -1;
  }

  if ( ctl_address.isEmpty () && !card_idx.isEmpty () ) {
    bool is_numeric;
    int card_idx_int ( card_idx.toInt ( &is_numeric ) );
    if ( is_numeric ) {
      if ( card_idx_int < 0 ) {
        card_idx_int = 0;
      }
      ctl_address = QString ( "hw:%1" ).arg ( card_idx_int );
    }
  }

  if ( !ctl_address.isEmpty () ) {
    _cmd_opts.start_ctl_address = ctl_address;
    _dsetup.main_window.mixer_dev.ctl_addr = ctl_address;
  }

  return 0;
}

void
Desktop_Items::parse_message ( QString msg_n )
{
  //::std::cout << "Desktop_Items::parse_message " << "\n";

  QStringList rows ( msg_n.split ( "\n", QString::SkipEmptyParts ) );

  for ( int ii = 0; ii < rows.size (); ++ii ) {
    const QString & row ( rows[ ii ] );

    //::std::cout << "row[" << ii << "]: " << row.toLocal8Bit().constData() <<
    //"\n";

    if ( row.contains ( "raise", Qt::CaseInsensitive ) ) {

      main_mixer_raise ();

    } else if ( row.contains ( "ctl_address=", Qt::CaseInsensitive ) ) {

      QString ctl_str;
      {
        const int idx ( row.indexOf ( "=" ) + 1 );
        ctl_str = ( row.mid ( idx ).trimmed () );
      }
      if ( !ctl_str.isEmpty () ) {
        if ( _main_mixer == 0 ) {
          _dsetup.main_window.mixer_dev.ctl_addr = ctl_str;
          tray_mixer_reload_current_mdev ();
        } else {
          _main_mixer->select_ctl ( ctl_str );
        }
      }
    }
  }
}

QString
Desktop_Items::message_to_other_instance () const
{
  // The message that gets sent to the other instance
  QString msg ( "new_instance\n" );

  // Append ALSA ctl address command
  if ( !cmd_opts ().start_ctl_address.isEmpty () ) {
    msg.append ( "ctl_address=" );
    msg.append ( cmd_opts ().start_ctl_address );
    msg.append ( "\n" );
  }

  // Append raise main window command
  if ( !cmd_opts ().start_tray_minimized ) {
    msg.append ( "raise\n" );
  }

  return msg;
}

void
Desktop_Items::start ( bool restore_session_n )
{
  _started = true;

  // Translation
  _dsetup.main_window.inputs.update_translation ();

  // Startup mixer device
  {
    QString & ctl_addr ( _dsetup.main_window.mixer_dev.ctl_addr );
    if ( _cmd_opts.start_ctl_address.isEmpty () ) {
      bool use_default ( true );
      switch ( _dsetup.start_device_mode ) {
      case 1:
        // Device should've been set during configuration reading
        use_default = ctl_addr.isEmpty ();
        break;
      case 2:
        ctl_addr = _dsetup.start_user_device;
        use_default = false;
        break;
      default:
        break;
      }
      if ( use_default ) {
        ctl_addr = "default";
      }
    } else {
      ctl_addr = _cmd_opts.start_ctl_address;
    }
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
  // Switch painter close
  {
    ::Wdg::Painter::DS_Switch_Painter_Close * pnt (
        new ::Wdg::Painter::DS_Switch_Painter_Close );
    pnt->set_group_variant ( ::Wdg::DS_CLOSE );
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

  if ( restore_session_n ) {
    // Restore minimized session
    if ( _dsetup.tray_is_minimized ) {
      _cmd_opts.start_tray_minimized = true;
    }
  }

  if ( _dsetup.tray_show_icon ||
       ( _cmd_opts.start_tray_minimized && _dsetup.tray_on_close ) ) {
    tray_mixer_create ();
  }

  // Raise the main mixer only on demand
  if ( !( _cmd_opts.start_tray_minimized && tray_mixer_visible () ) ) {
    main_mixer_raise ();
  }
}

void
Desktop_Items::main_mixer_create ()
{
  if ( _main_mixer != 0 ) {
    return;
  }

  _main_mixer = new Main_Window;
  _main_mixer->installEventFilter ( this );

  connect ( _main_mixer,
            SIGNAL ( sig_quit () ),
            this,
            SLOT ( quit () ),
            Qt::QueuedConnection );

  connect ( _main_mixer,
            SIGNAL ( sig_control_changed () ),
            this,
            SLOT ( tray_mixer_reload_current_mdev () ),
            Qt::QueuedConnection );

  connect ( _main_mixer,
            SIGNAL ( sig_show_settings () ),
            this,
            SLOT ( show_dialog_settings () ) );

  connect ( _main_mixer,
            SIGNAL ( sig_show_info () ),
            this,
            SLOT ( show_dialog_info () ) );

  _main_mixer->set_window_setup ( &_dsetup.main_window );
  _main_mixer->show ();
}

void
Desktop_Items::main_mixer_destroy ()
{
  if ( _main_mixer != 0 ) {
    delete _main_mixer;
    _main_mixer = 0;
    //::std::cout << "Main mixer destroyed\n";
  }
}

bool
Desktop_Items::main_mixer_visible ()
{
  bool res ( false );
  if ( _main_mixer != 0 ) {
    res = !_main_mixer->isHidden ();
  }
  return res;
}

void
Desktop_Items::main_mixer_close ()
{
  if ( _main_mixer != 0 ) {
    _main_mixer->close ();
  }
}

void
Desktop_Items::main_mixer_raise ()
{
  if ( _main_mixer == 0 ) {
    main_mixer_create ();
  }

  _main_mixer->show ();
  if ( _main_mixer->isMinimized () ) {
    _main_mixer->showNormal ();
  }
  _main_mixer->raise ();

  _dsetup.tray_is_minimized = false;

  tray_mixer_update_visibility ();
}

void
Desktop_Items::main_mixer_toggle_by_tray ()
{
  bool raise_main ( true );
  if ( _main_mixer != 0 ) {
    if ( _main_mixer->isActiveWindow () ) {
      raise_main = false;
    }
  }

  if ( raise_main ) {
    main_mixer_raise ();
  } else {
    if ( _dsetup.tray_show_icon || _dsetup.tray_on_close ) {
      if ( tray_mixer_visible () ) {
        _dsetup.tray_is_minimized = true;
        main_mixer_close ();
      }
    }
  }
}

void
Desktop_Items::main_mixer_closed ()
{
  // Destroy main mixer to be sure
  main_mixer_destroy ();

  tray_mixer_update_visibility ();
  if ( tray_mixer_visible () && _dsetup.tray_on_close ) {
    _dsetup.tray_is_minimized = true;
  }

  if ( !_dsetup.tray_is_minimized ) {
    quit ();
  }
}

void
Desktop_Items::main_mixer_reload_view ()
{
  if ( _main_mixer != 0 ) {
    _main_mixer->reload_mixer_view ();
  }
}

void
Desktop_Items::tray_mixer_create ()
{
  if ( _tray_mixer != 0 ) {
    return;
  }

  _tray_mixer = new Tray_Mixer ( this );

  connect ( _tray_mixer,
            SIGNAL ( sig_toggle_mixer () ),
            this,
            SLOT ( main_mixer_toggle_by_tray () ) );

  connect ( _tray_mixer,
            SIGNAL ( sig_quit () ),
            this,
            SLOT ( quit () ),
            Qt::QueuedConnection );

  // Initialize setup tree values
  _dsetup.tray_view.wheel_degrees = _dsetup.main_window.inputs.wheel_degrees;
  _dsetup.tray_mdev.current_device = _dsetup.main_window.mixer_dev.ctl_addr;

  // Install setup tree
  _tray_mixer->set_mdev_setup ( &_dsetup.tray_mdev );
  _tray_mixer->set_view_setup ( &_dsetup.tray_view );
}

void
Desktop_Items::tray_mixer_destroy ()
{
  if ( _tray_mixer != 0 ) {
    delete _tray_mixer;
    _tray_mixer = 0;
  }
}

void
Desktop_Items::tray_mixer_update_visibility ()
{
  bool tray_vis ( false );

  if ( !_shutdown ) {
    if ( _dsetup.tray_show_icon ) {
      tray_vis = true;
    } else if ( _dsetup.tray_on_close ) {
      tray_vis = !main_mixer_visible ();
    }
  }

  if ( tray_vis ) {
    tray_mixer_create ();
  } else {
    tray_mixer_destroy ();
  }
}

bool
Desktop_Items::tray_mixer_visible ()
{
  bool res ( false );
  if ( _tray_mixer != 0 ) {
    res = _tray_mixer->is_visible ();
  }
  return res;
}

void
Desktop_Items::tray_mixer_reload_mdev ()
{
  if ( _tray_mixer != 0 ) {
    _tray_mixer->set_mdev_setup ( 0 );
    _tray_mixer->set_mdev_setup ( &_dsetup.tray_mdev );
  }
}

void
Desktop_Items::tray_mixer_reload_current_mdev ()
{
  const QString & ctl_main ( _dsetup.main_window.mixer_dev.ctl_addr );
  if ( _dsetup.tray_mdev.current_device != ctl_main ) {
    _dsetup.tray_mdev.current_device = ctl_main;

    if ( _dsetup.tray_mdev.device_mode ==
         ::Tray_Mixer_MDev_Setup::MIXER_DEV_CURRENT ) {
      tray_mixer_reload_mdev ();
    }
  }
}

void
Desktop_Items::tray_mixer_update_balloon_setup ()
{
  if ( _tray_mixer != 0 ) {
    _tray_mixer->update_balloon_setup ();
  }
}

void
Desktop_Items::reload_inputs_setup ()
{
  // Main mixer
  if ( _main_mixer != 0 ) {
    _main_mixer->reload_mixer_inputs ();
  }

  // Tray mixer
  _dsetup.tray_view.wheel_degrees = _dsetup.main_window.inputs.wheel_degrees;
  if ( _tray_mixer != 0 ) {
    _tray_mixer->update_balloon_setup ();
  }
}

void
Desktop_Items::show_dialog_settings ()
{
  if ( _main_mixer == 0 ) {
    return;
  }

  if ( _dialog_settings == 0 ) {
    ::Views::Settings_Dialog * dlg (
        new ::Views::Settings_Dialog ( _main_mixer ) );
    dlg->setAttribute ( Qt::WA_DeleteOnClose );
    dlg->set_setup ( &_dsetup );

    connect ( dlg,
              SIGNAL ( sig_change_mixer_view () ),
              this,
              SLOT ( main_mixer_reload_view () ) );

    connect ( dlg,
              SIGNAL ( sig_change_input () ),
              this,
              SLOT ( reload_inputs_setup () ) );

    connect ( dlg,
              SIGNAL ( sig_change_tray_view () ),
              this,
              SLOT ( tray_mixer_update_visibility () ) );

    connect ( dlg,
              SIGNAL ( sig_change_tray_mdev () ),
              this,
              SLOT ( tray_mixer_reload_mdev () ) );

    connect ( dlg,
              SIGNAL ( sig_change_tray_balloon () ),
              this,
              SLOT ( tray_mixer_update_balloon_setup () ) );

    _dialog_settings = dlg;
  }
  _dialog_settings->show ();
}

void
Desktop_Items::show_dialog_info ()
{
  if ( _main_mixer == 0 ) {
    return;
  }

  if ( _dialog_info == 0 ) {
    ::Views::Info_Dialog * dlg ( new ::Views::Info_Dialog ( _main_mixer ) );
    dlg->setAttribute ( Qt::WA_DeleteOnClose );

    _dialog_info = dlg;
  }
  _dialog_info->show ();
}

void
Desktop_Items::shutdown ()
{
  if ( !_shutdown ) {
    _shutdown = true;
    main_mixer_close ();
    main_mixer_destroy ();
    tray_mixer_destroy ();
  }
}

void
Desktop_Items::quit ()
{
  shutdown ();
  emit sig_quit ();
}

bool
Desktop_Items::event ( QEvent * event_n )
{
  bool res ( false );
  if ( event_n->type () == _evt_mixer_closed ) {
    main_mixer_closed ();
    res = true;
  } else {
    res = QObject::event ( event_n );
  }
  return res;
}

bool
Desktop_Items::eventFilter ( QObject * obj_n, QEvent * event_n )
{
  bool filtered ( false );
  if ( obj_n == _main_mixer ) {
    if ( event_n->type () == QEvent::Close ) {
      //::std::cout << "Close event catched\n";
      _main_mixer->save_state ();
      if ( !_shutdown ) {
        QEvent * ev_req ( new QEvent ( QEvent::Type ( _evt_mixer_closed ) ) );
        QCoreApplication::postEvent ( this, ev_req );
      }
    }
  }
  return filtered;
}
