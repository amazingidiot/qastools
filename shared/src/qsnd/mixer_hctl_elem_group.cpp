//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "mixer_hctl_elem_group.hpp"


namespace QSnd
{


void
Mixer_HCTL_Elem_Group::clear ( )
{
	_snd_elems.clear();
}


} // End of namespace
