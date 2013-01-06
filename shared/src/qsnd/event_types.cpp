//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "qsnd/event_types.hpp"


namespace QSnd
{


::QEvent::Type evt_update_values_request;
::QEvent::Type evt_update_values;

::QEvent::Type evt_reload_request;
::QEvent::Type evt_reload;


void
init_event_types ( )
{
	evt_update_values_request =
		static_cast < QEvent::Type > ( QEvent::registerEventType() );
	evt_update_values =
		static_cast < QEvent::Type > ( QEvent::registerEventType() );

	evt_reload_request =
		static_cast < QEvent::Type > ( QEvent::registerEventType() );
	evt_reload =
		static_cast < QEvent::Type > ( QEvent::registerEventType() );
}


} // End of namespace

