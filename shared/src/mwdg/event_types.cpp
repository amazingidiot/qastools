//
// C++ Implementation:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "event_types.hpp"


namespace MWdg
{


::QEvent::Type evt_separation_request;
::QEvent::Type evt_separation;
::QEvent::Type evt_values_changed;
::QEvent::Type evt_refresh_data;


void
init_event_types ( )
{
	evt_separation_request =
		static_cast < QEvent::Type > ( QEvent::registerEventType() );
	evt_separation =
		static_cast < QEvent::Type > ( QEvent::registerEventType() );

	evt_values_changed =
		static_cast < QEvent::Type > ( QEvent::registerEventType() );

	evt_refresh_data =
		static_cast < QEvent::Type > ( QEvent::registerEventType() );
}


} // End of namespace

