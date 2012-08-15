//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "timer_callback.hpp"
#include <cassert>

namespace Wdg2
{


Timer_Callback::Timer_Callback ( ) :
_msec_interval ( 0 ),
_timer_idx ( 0 ),
_is_used ( false ),
_is_running ( false ),
_is_repeating ( false )
{
}


} // End of namespace
