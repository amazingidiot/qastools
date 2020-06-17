/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl_int_proxies_group.hpp"
#include "mwdg/event_types.hpp"
#include "mwdg/mixer_hctl_int_proxy_column.hpp"
#include "mwdg/mixer_hctl_int_proxy_slider.hpp"
#include "qsnd/mixer_hctl_elem.hpp"
#include "wdg/pad_proxy_switch.hpp"
#include <QCoreApplication>
#include <QEvent>
#include <cassert>
#include <iostream>

namespace MWdg
{

Mixer_HCTL_Int_Proxies_Group::Mixer_HCTL_Int_Proxies_Group (
    ::QSnd::Mixer_HCTL_Elem * snd_elem_n, QObject * parent_n )
: ::Wdg::Pad_Proxies_Group ( parent_n )
, _snd_elem ( snd_elem_n )
, _notify_value_change ( false )
{
  assert ( _snd_elem != 0 );

  connect ( _snd_elem,
            SIGNAL ( sig_values_changed () ),
            this,
            SLOT ( update_values () ) );
}

Mixer_HCTL_Int_Proxies_Group::~Mixer_HCTL_Int_Proxies_Group () {}

bool
Mixer_HCTL_Int_Proxies_Group::is_joined () const
{
  bool res ( false );
  if ( num_columns () > 0 ) {
    Mixer_HCTL_Int_Proxy_Column * col ( mcolumn ( 0 ) );
    if ( col->has_slider () ) {
      res = col->mslider_proxy ()->is_joined ();
    }
  }
  return res;
}

inline Mixer_HCTL_Int_Proxy_Column *
Mixer_HCTL_Int_Proxies_Group::mcolumn ( unsigned int idx_n ) const
{
  return static_cast<::MWdg::Mixer_HCTL_Int_Proxy_Column * > (
      column ( idx_n ) );
}

void
Mixer_HCTL_Int_Proxies_Group::set_joined ( bool flag_n )
{
  if ( num_columns () < 2 ) {
    return;
  }

  for ( unsigned int cii = 0; cii < num_columns (); ++cii ) {
    ::MWdg::Mixer_HCTL_Int_Proxy_Column * col ( mcolumn ( cii ) );
    if ( col->has_slider () ) {
      ::MWdg::Mixer_HCTL_Int_Proxy_Slider * pslider ( col->mslider_proxy () );
      pslider->set_joined ( flag_n );
      pslider->set_enabled ( ( cii == 0 ) || !flag_n );
      if ( flag_n && ( cii == 0 ) ) {
        pslider->snd_elem ()->level_integers ();
      }
    }
  }

  {
    ::MWdg::Mixer_HCTL_Int_Proxy_Column * col ( mcolumn ( 0 ) );
    if ( col->has_switch () ) {
      col->switch_proxy ()->set_switch_state ( flag_n );
    }
  }
}

bool
Mixer_HCTL_Int_Proxies_Group::volumes_equal () const
{
  return _snd_elem->integers_equal ();
  ;
}

void
Mixer_HCTL_Int_Proxies_Group::level_volumes ( unsigned int column_n )
{
  if ( num_columns () < 2 ) {
    return;
  }
  if ( column_n >= num_columns () ) {
    column_n = 0;
  }
  Mixer_HCTL_Int_Proxy_Column * col ( mcolumn ( column_n ) );
  if ( col->has_slider () ) {
    Mixer_HCTL_Int_Proxy_Slider * psl ( col->mslider_proxy () );
    psl->snd_elem ()->set_integer_all ( psl->integer_value () );
  }
}

void
Mixer_HCTL_Int_Proxies_Group::set_notify_value_change ( bool flag_n )
{
  _notify_value_change = flag_n;
}

void
Mixer_HCTL_Int_Proxies_Group::update_values ()
{
  for ( unsigned int cii = 0; cii < num_columns (); ++cii ) {
    Mixer_HCTL_Int_Proxy_Column * col ( mcolumn ( cii ) );
    if ( col->has_slider () ) {
      col->slider_proxy ()->update_value_from_source ();
    }
  }

  // Notify parent on demand
  if ( notify_value_change () && ( parent () != 0 ) ) {
    QEvent ev_req ( ::MWdg::evt_values_changed );
    QCoreApplication::sendEvent ( parent (), &ev_req );
  }
}

} // namespace MWdg
