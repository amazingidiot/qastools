/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_switches_proxy_enum.hpp"
#include <iostream>

namespace MWdg
{

Mixer_Switches_Proxy_Enum::Mixer_Switches_Proxy_Enum ()
: _mixer_simple_elem ( 0 )
, _channel_idx ( 0 )
, _snd_dir ( 0 )
, _is_joined ( false )
, _alsa_updating ( false )
{
}

void
Mixer_Switches_Proxy_Enum::set_snd_dir ( unsigned char dir_n )
{
  _snd_dir = dir_n;
}

void
Mixer_Switches_Proxy_Enum::set_channel_idx ( unsigned int idx_n )
{
  _channel_idx = idx_n;
}

void
Mixer_Switches_Proxy_Enum::set_mixer_simple_elem (
    ::QSnd::Mixer_Simple_Elem * selem_n )
{
  _mixer_simple_elem = selem_n;
}

void
Mixer_Switches_Proxy_Enum::set_is_joined ( bool flag_n )
{
  _is_joined = flag_n;
}

QString
Mixer_Switches_Proxy_Enum::enum_item_name ( int idx_n )
{
  QString res;
  if ( mixer_simple_elem () != 0 ) {
    if ( static_cast< unsigned int > ( idx_n ) <
         mixer_simple_elem ()->enum_item_names ().size () ) {
      res = mixer_simple_elem ()->enum_item_names ()[ idx_n ];
    }
  }
  return res;
}

void
Mixer_Switches_Proxy_Enum::enum_index_changed ()
{
  if ( ( mixer_simple_elem () != 0 ) && ( !_alsa_updating ) ) {
    //::std::cout << "Mixer_Switches_Proxy_Enum::enum_index_changed " <<
    //enum_index() << "\n";

    if ( is_joined () ) {
      mixer_simple_elem ()->set_enum_index_all ( snd_dir (), enum_index () );
    } else {
      mixer_simple_elem ()->set_enum_index (
          snd_dir (), channel_idx (), enum_index () );
    }

    //::std::cout << "Mixer_Switches_Proxy_Enum::enum_index_changed " << "done"
    //<< "\n";
  }
}

void
Mixer_Switches_Proxy_Enum::update_value_from_source ()
{
  if ( ( mixer_simple_elem () != 0 ) && ( !_alsa_updating ) ) {
    _alsa_updating = true;
    set_enum_index (
        mixer_simple_elem ()->enum_index ( snd_dir (), channel_idx () ) );
    _alsa_updating = false;
  }
}

} // namespace MWdg
