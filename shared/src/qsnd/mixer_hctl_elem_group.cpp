/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl_elem_group.hpp"

namespace QSnd
{

void
Mixer_HCTL_Elem_Group::clear ()
{
  _snd_elems.clear ();
}

} // namespace QSnd
