//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "init_globals.hpp"

#include "qsnd/event_types.hpp"
#include "wdg/event_types.hpp"
#include "mwdg/event_types.hpp"


int
init_globals ( )
{
	::QSnd::init_event_types();
	::Wdg::init_event_types();
	::MWdg::init_event_types();

	return 0;
}
