/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl_int_proxy_slider.hpp"
#include "qsnd/mixer_hctl_elem.hpp"
#include <QApplication>
#include <QFocusEvent>
#include <iostream>

namespace MWdg
{

Mixer_HCTL_Int_Proxy_Slider::Mixer_HCTL_Int_Proxy_Slider (
    ::QSnd::Mixer_HCTL_Elem * elem_n, unsigned int elem_idx_n )
: _integer_value ( 0 )
, _is_joined ( false )
, _updating_state ( false )
{
  _snd_elem = elem_n;
  _elem_idx = elem_idx_n;

  assert ( _snd_elem != 0 );

  set_slider_index_max (
      ::Wdg::integer_distance ( integer_min (), integer_max () ) );
}

Mixer_HCTL_Int_Proxy_Slider::~Mixer_HCTL_Int_Proxy_Slider () {}

long
Mixer_HCTL_Int_Proxy_Slider::integer_min () const
{
  return snd_elem ()->integer_min ();
}

long
Mixer_HCTL_Int_Proxy_Slider::integer_max () const
{
  return snd_elem ()->integer_max ();
}

void
Mixer_HCTL_Int_Proxy_Slider::set_joined ( bool flag_n )
{
  _is_joined = flag_n;
}

void
Mixer_HCTL_Int_Proxy_Slider::set_integer_value ( long value_n )
{
  if ( integer_value () != value_n ) {
    _integer_value = value_n;
    this->integer_value_changed ();
    emit sig_integer_value_changed ( integer_value () );
    emit sig_integer_value_changed ( int ( integer_value () ) );
  }
}

void
Mixer_HCTL_Int_Proxy_Slider::set_integer_value ( int value_n )
{
  set_integer_value ( long ( value_n ) );
}

void
Mixer_HCTL_Int_Proxy_Slider::integer_value_changed ()
{
  //::std::cout << "Mixer_HCTL_Int_Proxy_Slider::integer_value_changed " <<
  //integer_value() << "\n";

  if ( snd_elem () != 0 ) {
    set_slider_index ( integer_to_index ( integer_value () ) );
  }

  if ( ( snd_elem () != 0 ) && !_updating_state ) {
    if ( is_joined () || joined_by_key () ) {
      snd_elem ()->set_integer_all ( integer_value () );
    } else {
      snd_elem ()->set_integer ( elem_idx (), integer_value () );
    }
  }
}

void
Mixer_HCTL_Int_Proxy_Slider::slider_index_changed ()
{
  //::std::cout << "Mixer_HCTL_Int_Proxy_Slider::slider_index_changed " <<
  //integer_index() << "\n";

  if ( snd_elem () != 0 ) {
    set_integer_value ( index_to_integer ( slider_index () ) );
  }
}

void
Mixer_HCTL_Int_Proxy_Slider::update_value_from_source ()
{
  if ( ( snd_elem () != 0 ) && !_updating_state ) {
    _updating_state = true;
    set_integer_value ( snd_elem ()->integer_value ( elem_idx () ) );
    _updating_state = false;
  }
}

bool
Mixer_HCTL_Int_Proxy_Slider::joined_by_key () const
{
  bool res ( true );
  if ( ( QApplication::keyboardModifiers () & Qt::ControlModifier ) == 0 ) {
    res = false;
  }
  res = ( res && has_focus () );
  return res;
}

} // namespace MWdg
