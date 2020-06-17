/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_switches_proxies_group.hpp"
#include "mwdg/event_types.hpp"
#include "wdg/pad_proxies_column.hpp"
#include <QCoreApplication>
#include <iostream>

namespace MWdg
{

Mixer_Switches_Proxies_Group::Mixer_Switches_Proxies_Group (
    QObject * parent_n )
: Wdg::Pad_Proxies_Group ( parent_n )
, _mixer_simple_elem ( 0 )
, _snd_dir ( 0 )
, _is_joined ( false )
, _is_visible ( false )
, _notify_value_change ( false )
, _separation_request ( false )
{
  // Separation timer
  _separation_timer.setSingleShot ( true );
  _separation_timer.setInterval ( 550 );
  connect ( &_separation_timer,
            SIGNAL ( timeout () ),
            this,
            SLOT ( timer_separation_check () ) );
}

void
Mixer_Switches_Proxies_Group::set_mixer_simple_elem (
    ::QSnd::Mixer_Simple_Elem * selem_n )
{
  if ( mixer_simple_elem () != 0 ) {
    disconnect ( mixer_simple_elem (), 0, this, 0 );
  }

  _mixer_simple_elem = selem_n;

  if ( mixer_simple_elem () != 0 ) {
    connect ( mixer_simple_elem (),
              SIGNAL ( sig_values_changed () ),
              this,
              SLOT ( update_mixer_values () ) );
  }
}

void
Mixer_Switches_Proxies_Group::set_snd_dir ( unsigned char dir_n )
{
  _snd_dir = dir_n;
}

void
Mixer_Switches_Proxies_Group::set_is_joined ( bool flag_n )
{
  _is_joined = flag_n;
}

void
Mixer_Switches_Proxies_Group::set_is_visible ( bool flag_n )
{
  _is_visible = flag_n;
}

void
Mixer_Switches_Proxies_Group::set_notify_value_change ( bool flag_n )
{
  _notify_value_change = flag_n;
}

void
Mixer_Switches_Proxies_Group::set_separation_request ( bool flag_n )
{
  _separation_request = flag_n;
}

bool
Mixer_Switches_Proxies_Group::can_be_separated () const
{
  if ( mixer_simple_elem () != 0 ) {
    return ( mixer_simple_elem ()->num_channels ( snd_dir () ) > 1 );
  }
  return false;
}

bool
Mixer_Switches_Proxies_Group::should_be_separated () const
{
  bool res ( false );
  ::QSnd::Mixer_Simple_Elem * sme ( mixer_simple_elem () );
  if ( sme != 0 ) {
    res = ( ( sme->num_switch_channels ( snd_dir () ) > 1 ) &&
            !sme->switches_equal ( snd_dir () ) ) ||
          ( ( sme->num_enum_channels ( snd_dir () ) > 1 ) &&
            !sme->enums_equal ( snd_dir () ) );
  }
  return res;
}

bool
Mixer_Switches_Proxies_Group::needs_separation () const
{
  bool res ( false );
  ::QSnd::Mixer_Simple_Elem * sme ( mixer_simple_elem () );
  if ( sme != 0 ) {
    res = is_joined () && ( ( ( sme->num_switch_channels ( snd_dir () ) > 1 ) &&
                              !sme->switches_equal ( snd_dir () ) ) ||
                            ( ( sme->num_enum_channels ( snd_dir () ) > 1 ) &&
                              !sme->enums_equal ( snd_dir () ) ) );
  }
  return res;
}

void
Mixer_Switches_Proxies_Group::update_mixer_values ()
{
  for ( unsigned int ii = 0; ii < num_columns (); ++ii ) {
    ::Wdg::Pad_Proxies_Column * col ( column ( ii ) );
    if ( col->enum_proxy () != 0 ) {
      col->enum_proxy ()->update_value_from_source ();
    }
    if ( col->switch_proxy () != 0 ) {
      col->switch_proxy ()->update_value_from_source ();
    }
  }

  // Check for needed separation
  if ( needs_separation () ) {
    if ( !_separation_timer.isActive () ) {
      _separation_timer.start ();
    }
  } else {
    if ( _separation_timer.isActive () ) {
      _separation_timer.stop ();
    }
  }

  // Notify parent on demand
  if ( notify_value_change () && ( parent () != 0 ) ) {
    QEvent ev_req ( ::MWdg::evt_values_changed );
    QCoreApplication::sendEvent ( parent (), &ev_req );
  }
}

void
Mixer_Switches_Proxies_Group::timer_separation_check ()
{
  //::std::cout << "Mixer_Switches_Proxies_Group::timer_separation_check\n";

  if ( parent () != 0 ) {
    if ( needs_separation () ) {
      set_separation_request ( true );

      // Notify parent
      QEvent ev_req ( ::MWdg::evt_separation_request );
      QCoreApplication::sendEvent ( parent (), &ev_req );
    }
  }
}

} // namespace MWdg
