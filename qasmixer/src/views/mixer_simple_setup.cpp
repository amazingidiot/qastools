//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "mixer_simple_setup.hpp"


namespace Views
{


Mixer_Simple_Setup::Mixer_Simple_Setup ( )
{
	show_slider_value_labels = true;
	show_stream[0] = true;
	show_stream[1] = false;
}


} // End of namespace
