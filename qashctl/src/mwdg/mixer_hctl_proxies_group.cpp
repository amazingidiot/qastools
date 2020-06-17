/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl_proxies_group.hpp"
#include "qsnd/mixer_hctl_elem.hpp"
#include <iostream>

namespace MWdg
{

Mixer_HCTL_Proxies_Group::Mixer_HCTL_Proxies_Group ( QObject * parent_n )
: QObject ( parent_n )
{
}

Mixer_HCTL_Proxies_Group::~Mixer_HCTL_Proxies_Group () {}

void
Mixer_HCTL_Proxies_Group::update_values ()
{
  for ( unsigned int pii = 0; pii < num_proxies (); ++pii ) {
    proxy ( pii )->update_value_from_source ();
  }
}

void
Mixer_HCTL_Proxies_Group::set_joined ( bool flag_n )
{
  for ( unsigned int pii = 0; pii < num_proxies (); ++pii ) {
    Mixer_HCTL_Proxy * pro ( proxy ( pii ) );
    pro->set_joined ( flag_n );
    pro->set_enabled ( ( pii == 0 ) || !flag_n );
  }

  if ( flag_n && ( num_proxies () > 1 ) ) {
    Mixer_HCTL_Proxy * pro ( proxy ( 0 ) );
    if ( pro->snd_elem () != 0 ) {
      pro->snd_elem ()->level_values ();
    }
  }
}

} // namespace MWdg
