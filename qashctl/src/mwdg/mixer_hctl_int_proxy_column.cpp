/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl_int_proxy_column.hpp"
#include <iostream>

namespace MWdg
{

Mixer_HCTL_Int_Proxy_Column::Mixer_HCTL_Int_Proxy_Column ()
{
  //: Decibel value string template
  _str_value_dB = tr ( "%1 dB" );
  //: Percent value string template
  _str_value_pc = tr ( "%1 %" );
}

Mixer_HCTL_Int_Proxy_Column::~Mixer_HCTL_Int_Proxy_Column () {}

void
Mixer_HCTL_Int_Proxy_Column::slider_proxy_changed ()
{
  update_connections ();
}

void
Mixer_HCTL_Int_Proxy_Column::show_value_string_changed ()
{
  update_connections ();
}

void
Mixer_HCTL_Int_Proxy_Column::update_connections ()
{
  if ( slider_proxy () != 0 ) {
    Mixer_HCTL_Int_Proxy_Slider * msps ( mslider_proxy () );
    disconnect ( msps, 0, this, 0 );
    if ( show_value_string () ) {
      // if ( msps->has_dB() ) {
      //	connect (
      //		msps, SIGNAL ( sig_dB_value_changed ( long ) ),
      //		this, SIGNAL ( sig_value_string_changed() ) );
      //} else {
      connect ( msps,
                SIGNAL ( sig_slider_index_changed ( unsigned long ) ),
                this,
                SIGNAL ( sig_value_string_changed () ) );
      //}
    }
  }
}

// Value string

QString
Mixer_HCTL_Int_Proxy_Column::value_string () const
{
  QString res;
  if ( has_slider () && show_value_string () ) {
    Mixer_HCTL_Int_Proxy_Slider * msps ( mslider_proxy () );
    // if ( msps->has_dB() ) {
    //	dB_string ( res, msps->dB_value() );
    //} else {
    //	percent_string ( res, msps->volume_permille() );
    //}
    integer_string ( res, msps->integer_value () );
  }
  return res;
}

QString
Mixer_HCTL_Int_Proxy_Column::value_min_string () const
{
  QString res;
  if ( has_slider () && show_value_string () ) {
    Mixer_HCTL_Int_Proxy_Slider * msps ( mslider_proxy () );
    // if ( msps->has_dB() ) {
    //	dB_string ( res, msps->dB_max() );
    //} else {
    //	percent_string ( res, -1000 );
    //}
    integer_string ( res, msps->integer_min () );
  }
  return res;
}

QString
Mixer_HCTL_Int_Proxy_Column::value_max_string () const
{
  QString res;
  if ( has_slider () && show_value_string () ) {
    Mixer_HCTL_Int_Proxy_Slider * msps ( mslider_proxy () );
    // if ( msps->has_dB() ) {
    //	dB_string ( res, msps->dB_min() );
    //} else {
    //	percent_string ( res, 1000 );
    //}
    integer_string ( res, msps->integer_max () );
  }
  return res;
}

void
Mixer_HCTL_Int_Proxy_Column::integer_string ( QString & str_n,
                                              long value_n ) const
{
  str_n = _loc.toString ( (int)value_n );
}

void
Mixer_HCTL_Int_Proxy_Column::dB_string ( QString & str_n,
                                         long dB_value_n ) const
{
  str_n = _loc.toString ( dB_value_n / 100.0, 'f', 2 );
  str_n = _str_value_dB.arg ( str_n );
}

void
Mixer_HCTL_Int_Proxy_Column::percent_string ( QString & str_n,
                                              int permille_n ) const
{
  str_n = _loc.toString ( permille_n / 10.0, 'f', 1 );
  str_n = _str_value_pc.arg ( str_n );
}

} // namespace MWdg
