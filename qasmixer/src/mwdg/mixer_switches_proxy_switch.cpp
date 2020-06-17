/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_switches_proxy_switch.hpp"
#include <QEvent>
#include <QKeyEvent>

namespace MWdg
{

Mixer_Switches_Proxy_Switch::Mixer_Switches_Proxy_Switch ()
: _mixer_simple_elem ( 0 )
, _channel_idx ( 0 )
, _snd_dir ( 0 )
, _is_joined ( false )
, _alsa_updating ( false )
{
}

void
Mixer_Switches_Proxy_Switch::set_snd_dir ( unsigned char dir_n )
{
  _snd_dir = dir_n;
}

void
Mixer_Switches_Proxy_Switch::set_channel_idx ( unsigned int idx_n )
{
  _channel_idx = idx_n;
}

void
Mixer_Switches_Proxy_Switch::set_mixer_simple_elem (
    ::QSnd::Mixer_Simple_Elem * selem_n )
{
  _mixer_simple_elem = selem_n;
}

void
Mixer_Switches_Proxy_Switch::set_is_joined ( bool flag_n )
{
  _is_joined = flag_n;
}

void
Mixer_Switches_Proxy_Switch::switch_state_changed ()
{
  if ( ( mixer_simple_elem () != 0 ) && ( !_alsa_updating ) ) {
    //::std::cout << "Mixer_Switches_Proxy_Switch::switch_state_changed " <<
    //switch_state() << "\n";

    if ( is_joined () ) {
      mixer_simple_elem ()->set_switch_all ( snd_dir (), switch_state () );
    } else {
      mixer_simple_elem ()->set_switch (
          snd_dir (), channel_idx (), switch_state () );
    }

    //::std::cout << "Mixer_Switches_Proxy_Switch::switch_state_changed " <<
    //"done" << "\n";
  }
}

void
Mixer_Switches_Proxy_Switch::update_value_from_source ()
{
  if ( ( mixer_simple_elem () != 0 ) && ( !_alsa_updating ) ) {
    _alsa_updating = true;
    set_switch_state (
        mixer_simple_elem ()->switch_state ( snd_dir (), channel_idx () ) );
    _alsa_updating = false;
  }
}

bool
Mixer_Switches_Proxy_Switch::eventFilter ( QObject * obj_n, QEvent * event_n )
{
  bool res ( Pad_Proxy_Switch::eventFilter ( obj_n, event_n ) );
  if ( !res ) {
    if ( event_n->type () == QEvent::KeyPress ) {
      QKeyEvent * kev ( static_cast< QKeyEvent * > ( event_n ) );
      if ( kev->key () == Qt::Key_VolumeMute ) {
        toggle_switch_state ();
        res = true;
      }
    }
  }
  return res;
}

} // namespace MWdg
