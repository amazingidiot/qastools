//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
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
