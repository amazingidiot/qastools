//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "event_types.hpp"


namespace Wdg
{


::QEvent::Type evt_pass_event_focus;
::QEvent::Type evt_pass_event_key;


void
init_event_types ( )
{
	evt_pass_event_focus =
		static_cast < QEvent::Type > ( QEvent::registerEventType() );
	evt_pass_event_key =
		static_cast < QEvent::Type > ( QEvent::registerEventType() );
}


} // End of namespace

