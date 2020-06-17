/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "tray_mixer.hpp"
#include "qastools_config.hpp"
#include "qsnd/event_types.hpp"
#include "qsnd/mixer_simple.hpp"
#include "qsnd/mixer_simple_elem.hpp"
#include "qsnd/mixer_simple_filter_name.hpp"
#include "tray_mixer_balloon.hpp"
#include "tray_mixer_icon.hpp"
#include "tray_mixer_mdev_setup.hpp"
#include "tray_mixer_view_setup.hpp"
#include "wdg/uint_mapper.hpp"
#include <QCoreApplication>
#include <QEvent>
#include <QFileInfo>
#include <cmath>
#include <iostream>

Tray_Mixer::Tray_Mixer ( QObject * parent_n )
: QObject ( parent_n )
, _mdev_setup ( 0 )
, _view_setup ( 0 )
, _mx_elem ( 0 )
, _volume_permille ( 0 )
, _icon_idx ( 9999 )
, _volume_muted ( false )
, _updating_scheduled ( false )
, _tray_icon ( 0 )
, _balloon ( 0 )
{
  _snd_mixer = new ::QSnd::Mixer_Simple ( this );
  {
    ::QSnd::Mixer_Simple_Filter_Name * filter (
        new ::QSnd::Mixer_Simple_Filter_Name );
    filter->set_blacklist ( false );
    filter->append_name ( "master" );
    _snd_mixer->append_elem_filter ( filter );
  }

  // Tray icon
  {
    _tray_icon = new Tray_Mixer_Icon ( this );

    connect (
        _tray_icon, SIGNAL ( sig_quit () ), this, SIGNAL ( sig_quit () ) );

    connect (
        _tray_icon, SIGNAL ( sig_hover () ), this, SLOT ( raise_balloon () ) );

    connect ( _tray_icon,
              SIGNAL ( sig_activated () ),
              this,
              SIGNAL ( sig_toggle_mixer () ) );

    connect ( _tray_icon,
              SIGNAL ( sig_wheel_delta ( int ) ),
              this,
              SLOT ( mouse_wheel_delta ( int ) ) );

    connect ( _tray_icon,
              SIGNAL ( sig_middle_click () ),
              this,
              SLOT ( mixer_toggle_switch () ) );
  }

  init_icons ();
  update_tray_icon ();

  _tray_icon->show ();
}

Tray_Mixer::~Tray_Mixer ()
{
  close_balloon ();
}

void
Tray_Mixer::init_icons ()
{
  QIcon icon_def;
  {
    const char * def_str ( 0 );
    if ( QIcon::hasThemeIcon ( "multimedia-volume-control" ) ) {
      def_str = "multimedia-volume-control";
    } else if ( QIcon::hasThemeIcon ( "audio-volume-high" ) ) {
      def_str = "audio-volume-high";
    }

    if ( def_str != 0 ) {
      icon_def = QIcon::fromTheme ( def_str );
    } else {
      QString icon_path ( INSTALL_DIR_ICONS_SVG );
      icon_path += "/";
      icon_path += PROGRAM_NAME;
      icon_path += ".svg";

      QFileInfo finfo ( icon_path );
      if ( finfo.exists () && finfo.isReadable () ) {
        icon_def = QIcon ( icon_path );
      }
    }
  }

  for ( unsigned int ii = 0; ii < 5; ++ii ) {
    _icons_volume.append ( icon_def );
  }

  _icons_volume[ 2 ] = QIcon::fromTheme ( "audio-volume-low", icon_def );
  _icons_volume[ 3 ] = QIcon::fromTheme ( "audio-volume-medium", icon_def );
  _icons_volume[ 4 ] = QIcon::fromTheme ( "audio-volume-high", icon_def );

  // Default and muted
  _icons_volume[ 0 ] = _icons_volume[ 3 ];
  _icons_volume[ 1 ] =
      QIcon::fromTheme ( "audio-volume-muted", _icons_volume[ 2 ] );
}

void
Tray_Mixer::set_mdev_setup ( Tray_Mixer_MDev_Setup * setup_n )
{
  if ( _mdev_setup != 0 ) {
    close_mixer ();
  }

  _mdev_setup = setup_n;

  if ( _mdev_setup != 0 ) {
    load_mixer ();
  }
}

void
Tray_Mixer::set_view_setup ( Tray_Mixer_View_Setup * setup_n )
{
  if ( _view_setup != 0 ) {
    close_balloon ();
  }

  _view_setup = setup_n;
}

void
Tray_Mixer::update_balloon_setup ()
{
  bool do_close ( false );
  if ( _view_setup == 0 ) {
    do_close = true;
  } else if ( !_view_setup->show_balloon ) {
    do_close = true;
  }

  if ( do_close ) {
    close_balloon ();
  }
  if ( ( _balloon != 0 ) && ( _view_setup != 0 ) ) {
    _balloon->set_duration_ms ( _view_setup->balloon_lifetime );
  }
}

bool
Tray_Mixer::is_visible () const
{
  return ( QSystemTrayIcon::isSystemTrayAvailable () &&
           _tray_icon->isVisible () );
}

void
Tray_Mixer::close_mixer ()
{
  close_balloon ();

  // Clear mixer element
  if ( _mx_elem != 0 ) {
    disconnect ( _mx_elem, 0, this, 0 );
  }
  _mx_elem = 0;
  _volume_permille = 0;
  _volume_muted = false;

  _snd_mixer->close ();
  update_tray_icon ();
}

void
Tray_Mixer::load_mixer ()
{
  if ( _mdev_setup == 0 ) {
    return;
  }

  QString mdev;
  switch ( _mdev_setup->device_mode ) {
  case ::Tray_Mixer_MDev_Setup::MIXER_DEV_CURRENT:
    mdev = _mdev_setup->current_device;
    break;
  case ::Tray_Mixer_MDev_Setup::MIXER_DEV_USER:
    mdev = _mdev_setup->user_device;
    break;
  default:
    break;
  }
  if ( mdev.isEmpty () ) {
    mdev = "default";
  }

  if ( !_snd_mixer->open ( mdev ) ) {

    // Update mixer element
    for ( unsigned int ii = 0; ii < _snd_mixer->num_elems (); ++ii ) {
      if ( _snd_mixer->elem ( ii )->has_volume ( 0 ) ) {
        _mx_elem = _snd_mixer->elem ( ii );
        break;
      }
    }
    if ( _mx_elem != 0 ) {
      connect ( _mx_elem,
                SIGNAL ( sig_values_changed () ),
                this,
                SLOT ( mixer_values_changed () ) );
    }
    update_volume_permille ();
  }

  update_tray_icon ();
}

void
Tray_Mixer::raise_balloon ()
{
  if ( _view_setup == 0 ) {
    return;
  }

  if ( ( _balloon == 0 ) && QSystemTrayIcon::isSystemTrayAvailable () &&
       _tray_icon->isVisible () && _view_setup->show_balloon ) {
    _balloon.reset ( new Tray_Mixer_Balloon ( _view_setup->image_alloc ) );
    _balloon->set_tray_icon_geometry ( _tray_icon->geometry () );
    _balloon->set_duration_ms ( _view_setup->balloon_lifetime );
    _balloon->set_icon ( _icons_volume[ 3 ] );

    connect ( _balloon.data (),
              SIGNAL ( sig_activated () ),
              this,
              SIGNAL ( sig_toggle_mixer () ) );

    connect ( _balloon.data (),
              SIGNAL ( sig_middle_click () ),
              this,
              SLOT ( mixer_toggle_switch () ) );

    connect ( _balloon.data (),
              SIGNAL ( sig_wheel_delta ( int ) ),
              this,
              SLOT ( mouse_wheel_delta ( int ) ) );

    connect ( _balloon.data (),
              SIGNAL ( sig_close () ),
              this,
              SLOT ( close_balloon () ) );
  }
  if ( _balloon != 0 ) {
    _balloon->set_muted ( _volume_muted );
    _balloon->set_permille ( _volume_permille );
    _balloon->start_show ();
  }
}

void
Tray_Mixer::close_balloon ()
{
  if ( _balloon != 0 ) {
    _balloon.reset ();
  }
}

void
Tray_Mixer::update_tray_icon ()
{
  unsigned int icon_idx ( 0 );

  if ( _mx_elem != 0 ) {
    if ( _volume_muted ) {
      icon_idx = 1;
    } else {
      if ( _volume_permille < 333 ) {
        icon_idx = 2;
      } else if ( _volume_permille < 750 ) {
        icon_idx = 3;
      } else {
        icon_idx = 4;
      }
    }
  }

  if ( _icon_idx != icon_idx ) {
    _icon_idx = icon_idx;
    _tray_icon->setIcon ( _icons_volume[ _icon_idx ] );
  }
}

bool
Tray_Mixer::update_volume_permille ()
{
  bool res ( false );

  bool muted ( _volume_muted );
  unsigned int perm ( _volume_permille );

  if ( _mx_elem != 0 ) {
    unsigned long dist_current ( ::Wdg::integer_distance (
        _mx_elem->volume_min ( 0 ), _mx_elem->volume ( 0, 0 ) ) );

    unsigned long dist_total ( ::Wdg::integer_distance (
        _mx_elem->volume_min ( 0 ), _mx_elem->volume_max ( 0 ) ) );

    perm = ::Wdg::permille ( dist_current, dist_total );

    if ( _mx_elem->has_switch ( 0 ) ) {
      muted = !_mx_elem->switch_state ( 0, 0 );
    }
  }

  if ( ( perm != _volume_permille ) || ( muted != _volume_muted ) ) {
    _volume_muted = muted;
    _volume_permille = perm;
    res = true;
  }

  return res;
}

void
Tray_Mixer::update_volume_widgets ()
{
  if ( update_volume_permille () ) {
    raise_balloon ();
  }
  update_tray_icon ();
}

void
Tray_Mixer::mixer_values_changed ()
{
  if ( !_updating_scheduled ) {
    _updating_scheduled = true;
    QEvent * ev_req ( new QEvent ( ::QSnd::evt_update_values ) );
    QCoreApplication::postEvent ( this, ev_req );
  }
}

void
Tray_Mixer::mixer_toggle_switch ()
{
  if ( _mx_elem != 0 ) {
    if ( _mx_elem->has_switch ( 0 ) ) {
      _mx_elem->invert_switches ( 0 );
    }
  }
}

void
Tray_Mixer::mouse_wheel_delta ( int wheel_delta_n )
{
  if ( ( _view_setup == 0 ) || ( _mx_elem == 0 ) ) {
    return;
  }

  const long vol_min ( _mx_elem->volume_min ( 0 ) );
  const long vol_max ( _mx_elem->volume_max ( 0 ) );
  long vol_old ( _mx_elem->volume ( 0, 0 ) );

  long delta;
  {
    const double range ( double ( vol_max ) - double ( vol_min ) );
    double amount ( range / double ( _view_setup->wheel_degrees ) );
    amount *= ( wheel_delta_n / 8.0 );
    if ( amount > 0 ) {
      delta = std::ceil ( amount );
    } else {
      delta = std::floor ( amount );
    }
  }
  if ( delta == 0 ) {
    if ( wheel_delta_n > 0 ) {
      delta = 1;
    } else {
      delta = -1;
    }
  }

  long vol ( vol_old );
  if ( delta > 0 ) {
    if ( ( vol_max - vol ) > delta ) {
      vol += delta;
    } else {
      vol = vol_max;
    }
  } else {
    if ( ( vol_min - vol ) < delta ) {
      vol += delta;
    } else {
      vol = vol_min;
    }
  }

  if ( vol != vol_old ) {
    _mx_elem->set_volume_all ( 0, vol );
  }
}

bool
Tray_Mixer::event ( QEvent * event_n )
{
  if ( event_n->type () == ::QSnd::evt_update_values ) {
    _updating_scheduled = false;
    update_volume_widgets ();
    return true;
  }

  return QObject::event ( event_n );
}
