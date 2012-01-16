//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
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
