/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "pad_proxy_slider.hpp"

namespace Wdg
{

Pad_Proxy_Slider::Pad_Proxy_Slider ()
: ::Wdg::Pad_Proxy ( 0 )
, _slider_index ( 0 )
, _slider_index_max ( 0 )
{
}

Pad_Proxy_Slider::~Pad_Proxy_Slider () {}

void
Pad_Proxy_Slider::set_slider_index ( unsigned long index_n )
{
  if ( index_n > slider_index_max () ) {
    index_n = slider_index_max ();
  }
  if ( slider_index () != index_n ) {
    _slider_index = index_n;
    this->slider_index_changed ();
    emit sig_slider_index_changed ( slider_index () );
  }
}

void
Pad_Proxy_Slider::set_slider_index_max ( unsigned long index_n )
{
  if ( slider_index_max () != index_n ) {
    _slider_index_max = index_n;
    if ( slider_index () > slider_index_max () ) {
      set_slider_index ( slider_index_max () );
    }
    this->slider_index_max_changed ();
    emit sig_slider_index_max_changed ( slider_index_max () );
  }
}

void
Pad_Proxy_Slider::slider_index_changed ()
{
  // Dummy implementation
}

void
Pad_Proxy_Slider::slider_index_max_changed ()
{
  // Dummy implementation
}

} // namespace Wdg
