/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl_proxy_enum.hpp"
#include "qsnd/mixer_hctl_elem.hpp"
#include <iostream>

namespace MWdg
{

Mixer_HCTL_Proxy_Enum::Mixer_HCTL_Proxy_Enum ( QObject * parent_n )
: Mixer_HCTL_Proxy ( parent_n )
, _enum_index ( 0 )
, _updating_state ( false )
{
}

unsigned int
Mixer_HCTL_Proxy_Enum::enum_num_items () const
{
  return snd_elem ()->enum_num_items ();
}

const char *
Mixer_HCTL_Proxy_Enum::enum_item_name ( unsigned int index_n )
{
  return snd_elem ()->enum_item_name ( index_n );
}

void
Mixer_HCTL_Proxy_Enum::set_enum_index ( unsigned int index_n )
{
  if ( enum_index () != index_n ) {
    _enum_index = index_n;
    this->enum_index_changed ();
    emit sig_enum_index_changed ( enum_index () );
    emit sig_enum_index_changed ( static_cast< int > ( enum_index () ) );
  }
}

void
Mixer_HCTL_Proxy_Enum::set_enum_index ( int index_n )
{
  if ( index_n >= 0 ) {
    set_enum_index ( static_cast< unsigned int > ( index_n ) );
  }
}

void
Mixer_HCTL_Proxy_Enum::enum_index_changed ()
{
  if ( ( snd_elem () != 0 ) && !_updating_state ) {
    if ( is_joined () || joined_by_key () ) {
      snd_elem ()->set_enum_index_all ( enum_index () );
    } else {
      snd_elem ()->set_enum_index ( elem_idx (), enum_index () );
    }
  }
}

void
Mixer_HCTL_Proxy_Enum::update_value_from_source ()
{
  if ( ( snd_elem () != 0 ) && !_updating_state ) {
    _updating_state = true;
    set_enum_index ( snd_elem ()->enum_index ( elem_idx () ) );
    _updating_state = false;
  }
}

} // namespace MWdg
