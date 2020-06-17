/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl_proxy_switch.hpp"
#include "qsnd/mixer_hctl_elem.hpp"
#include <iostream>

namespace MWdg
{

Mixer_HCTL_Proxy_Switch::Mixer_HCTL_Proxy_Switch ( QObject * parent_n )
: Mixer_HCTL_Proxy ( parent_n )
, _switch_state ( false )
, _updating_state ( false )
{
}

void
Mixer_HCTL_Proxy_Switch::set_switch_state ( bool state_n )
{
  if ( switch_state () != state_n ) {
    _switch_state = state_n;
    this->switch_state_changed ();
    emit sig_switch_state_changed ( switch_state () );
  }
}

void
Mixer_HCTL_Proxy_Switch::switch_state_changed ()
{
  if ( ( snd_elem () != 0 ) && !_updating_state ) {
    if ( is_joined () || joined_by_key () ) {
      snd_elem ()->set_switch_all ( switch_state () );
    } else {
      snd_elem ()->set_switch_state ( elem_idx (), switch_state () );
    }
  }
}

void
Mixer_HCTL_Proxy_Switch::update_value_from_source ()
{
  if ( ( snd_elem () != 0 ) && !_updating_state ) {
    _updating_state = true;
    set_switch_state ( snd_elem ()->switch_state ( elem_idx () ) );
    _updating_state = false;
  }
}

} // namespace MWdg
