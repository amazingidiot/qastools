/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "pad_proxy_switch.hpp"

namespace Wdg
{

Pad_Proxy_Switch::Pad_Proxy_Switch ()
: ::Wdg::Pad_Proxy ( 1 )
, _switch_state ( false )
{
}

Pad_Proxy_Switch::~Pad_Proxy_Switch () {}

void
Pad_Proxy_Switch::set_switch_state ( bool state_n )
{
  if ( state_n != _switch_state ) {
    _switch_state = state_n;
    this->switch_state_changed ();
    emit sig_switch_state_changed ( switch_state () );
  }
}

void
Pad_Proxy_Switch::toggle_switch_state ()
{
  this->set_switch_state ( !switch_state () );
}

void
Pad_Proxy_Switch::switch_state_changed ()
{
  // Dummy implementation
}

} // namespace Wdg
