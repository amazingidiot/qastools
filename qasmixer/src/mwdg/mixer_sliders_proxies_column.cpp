/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_sliders_proxies_column.hpp"
#include "wdg/event_types.hpp"
#include "wdg/pass_events.hpp"
#include <QCoreApplication>
#include <iostream>

namespace MWdg
{

Mixer_Sliders_Proxies_Column::Mixer_Sliders_Proxies_Column ()
{
  _str_value_dB = tr ( "%1 dB" );
  _str_value_pc = tr ( "%1 %" );
}

Mixer_Sliders_Proxies_Column::~Mixer_Sliders_Proxies_Column () {}

void
Mixer_Sliders_Proxies_Column::update_mixer_values ()
{
  if ( slider_proxy () != 0 ) {
    mslider_proxy ()->update_value_from_source ();
  }
  if ( switch_proxy () != 0 ) {
    mswitch_proxy ()->update_value_from_source ();
  }
}

void
Mixer_Sliders_Proxies_Column::slider_proxy_changed ()
{
  update_connections ();
}

void
Mixer_Sliders_Proxies_Column::switch_proxy_changed ()
{
  update_connections ();
}

void
Mixer_Sliders_Proxies_Column::show_value_string_changed ()
{
  update_connections ();
}

void
Mixer_Sliders_Proxies_Column::update_connections ()
{
  if ( slider_proxy () != 0 ) {
    ::MWdg::Mixer_Sliders_Proxy_Slider * msps ( mslider_proxy () );
    disconnect ( msps, 0, this, 0 );
    if ( show_value_string () ) {
      if ( msps->has_dB () ) {
        connect ( msps,
                  SIGNAL ( sig_dB_value_changed ( long ) ),
                  this,
                  SIGNAL ( sig_value_string_changed () ) );
      } else {
        connect ( msps,
                  SIGNAL ( sig_slider_index_changed ( unsigned long ) ),
                  this,
                  SIGNAL ( sig_value_string_changed () ) );
      }
    }
  }
}

// Value string

QString
Mixer_Sliders_Proxies_Column::value_string () const
{
  QString res;
  if ( has_slider () && show_value_string () ) {
    ::MWdg::Mixer_Sliders_Proxy_Slider * msps ( mslider_proxy () );
    if ( msps->has_dB () ) {
      dB_string ( res, msps->dB_value () );
    } else {
      percent_string ( res, msps->volume_permille () );
    }
  }
  return res;
}

QString
Mixer_Sliders_Proxies_Column::value_min_string () const
{
  QString res;
  if ( has_slider () && show_value_string () ) {
    ::MWdg::Mixer_Sliders_Proxy_Slider * msps ( mslider_proxy () );
    if ( msps->has_dB () ) {
      dB_string ( res, msps->dB_max () );
    } else {
      percent_string ( res, -1000 );
    }
  }
  return res;
}

QString
Mixer_Sliders_Proxies_Column::value_max_string () const
{
  QString res;
  if ( has_slider () && show_value_string () ) {
    ::MWdg::Mixer_Sliders_Proxy_Slider * msps ( mslider_proxy () );
    if ( msps->has_dB () ) {
      dB_string ( res, msps->dB_min () );
    } else {
      percent_string ( res, 1000 );
    }
  }
  return res;
}

void
Mixer_Sliders_Proxies_Column::dB_string ( QString & str_n,
                                          long dB_value_n ) const
{
  str_n = _loc.toString ( dB_value_n / 100.0, 'f', 2 );
  str_n = _str_value_dB.arg ( str_n );
}

void
Mixer_Sliders_Proxies_Column::percent_string ( QString & str_n,
                                               int permille_n ) const
{
  str_n = _loc.toString ( permille_n / 10.0, 'f', 1 );
  str_n = _str_value_pc.arg ( str_n );
}

bool
Mixer_Sliders_Proxies_Column::event ( QEvent * event_n )
{
  if ( event_n->type () == ::Wdg::evt_pass_event_key ) {
    ::Wdg::Pass_Event_Key * ev_kp (
        static_cast<::Wdg::Pass_Event_Key * > ( event_n ) );

    if ( parent () != 0 ) {
      ev_kp->column_idx = column_index ();
      QCoreApplication::sendEvent ( parent (), event_n );
    }
    return true;
  }

  return ::Wdg::Pad_Proxies_Column::event ( event_n );
}

} // namespace MWdg
