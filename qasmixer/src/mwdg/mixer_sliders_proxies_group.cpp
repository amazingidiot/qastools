/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_sliders_proxies_group.hpp"
#include "mwdg/event_types.hpp"
#include "wdg/event_types.hpp"
#include "wdg/pass_events.hpp"
#include <QCoreApplication>
#include <iostream>

namespace MWdg
{

Mixer_Sliders_Proxies_Group::Mixer_Sliders_Proxies_Group ( QObject * parent_n )
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
Mixer_Sliders_Proxies_Group::set_mixer_simple_elem (
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
Mixer_Sliders_Proxies_Group::set_snd_dir ( unsigned char dir_n )
{
  _snd_dir = dir_n;
}

void
Mixer_Sliders_Proxies_Group::set_is_joined ( bool flag_n )
{
  _is_joined = flag_n;
}

void
Mixer_Sliders_Proxies_Group::set_is_visible ( bool flag_n )
{
  _is_visible = flag_n;
}

void
Mixer_Sliders_Proxies_Group::set_notify_value_change ( bool flag_n )
{
  _notify_value_change = flag_n;
}

void
Mixer_Sliders_Proxies_Group::set_separation_request ( bool flag_n )
{
  _separation_request = flag_n;
}

bool
Mixer_Sliders_Proxies_Group::can_be_separated () const
{
  if ( mixer_simple_elem () != 0 ) {
    return ( mixer_simple_elem ()->num_channels ( snd_dir () ) > 1 );
  }
  return false;
}

bool
Mixer_Sliders_Proxies_Group::should_be_separated () const
{
  bool res ( false );
  ::QSnd::Mixer_Simple_Elem * sme ( mixer_simple_elem () );
  if ( sme != 0 ) {
    res = ( ( sme->num_volume_channels ( snd_dir () ) > 1 ) &&
            !sme->volumes_equal ( snd_dir () ) ) ||
          ( ( sme->num_switch_channels ( snd_dir () ) > 1 ) &&
            !sme->switches_equal ( snd_dir () ) );
  }
  return res;
}

bool
Mixer_Sliders_Proxies_Group::needs_separation () const
{
  bool res ( false );
  ::QSnd::Mixer_Simple_Elem * sme ( mixer_simple_elem () );
  if ( sme != 0 ) {
    res = is_joined () && ( ( ( sme->num_volume_channels ( snd_dir () ) > 1 ) &&
                              !sme->volumes_equal ( snd_dir () ) ) ||
                            ( ( sme->num_switch_channels ( snd_dir () ) > 1 ) &&
                              !sme->switches_equal ( snd_dir () ) ) );
  }
  return res;
}

void
Mixer_Sliders_Proxies_Group::update_mixer_values ()
{
  for ( unsigned int ii = 0; ii < num_columns (); ++ii ) {
    static_cast< Mixer_Sliders_Proxies_Column * > ( column ( ii ) )
        ->update_mixer_values ();
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
Mixer_Sliders_Proxies_Group::timer_separation_check ()
{
  //::std::cout << "Mixer_Sliders_Proxies_Group::timer_separation_check\n";

  if ( parent () != 0 ) {
    if ( needs_separation () ) {
      set_separation_request ( true );

      // Notify parent
      QEvent ev_req ( ::MWdg::evt_separation_request );
      QCoreApplication::sendEvent ( parent (), &ev_req );
    }
  }
}

bool
Mixer_Sliders_Proxies_Group::event ( QEvent * event_n )
{
  if ( event_n->type () == ::Wdg::evt_pass_event_key ) {
    ::Wdg::Pass_Event_Key * ev_kp (
        static_cast<::Wdg::Pass_Event_Key * > ( event_n ) );

    if ( ev_kp->column_idx < num_columns () ) {
      ::Wdg::Pad_Proxies_Column * col ( column ( ev_kp->column_idx ) );
      // Pass the key event to the switch widget
      ::Wdg::Pad_Proxy_Slider * spp_sl ( col->slider_proxy () );
      if ( spp_sl != 0 ) {
        ::Wdg::Pad_Proxy_Switch * spp_sw ( col->switch_proxy () );
        // Use joined switch on demand
        if ( spp_sw == 0 ) {
          ::Wdg::Pad_Proxies_Column * col0 ( column ( 0 ) );
          spp_sw = col0->switch_proxy ();
        }
        if ( spp_sw != 0 ) {
          // Pass event to switch widget
          if ( spp_sw->widget () != 0 ) {
            const bool old_focus ( spp_sw->has_focus () );
            spp_sw->set_has_focus ( spp_sl->has_focus () );
            QCoreApplication::sendEvent ( spp_sw->widget (), &ev_kp->ev_key );
            spp_sw->set_has_focus ( old_focus );
          }
        }
      }
    }
    return true;
  }

  return ::Wdg::Pad_Proxies_Group::event ( event_n );
}

} // namespace MWdg
