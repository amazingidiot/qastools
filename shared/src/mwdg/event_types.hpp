//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_mwdg_event_types_hpp__
#define __INC_mwdg_event_types_hpp__

#include <QEvent>


namespace MWdg
{

extern ::QEvent::Type evt_separation_request;
extern ::QEvent::Type evt_separation;

extern ::QEvent::Type evt_values_changed;
extern ::QEvent::Type evt_refresh_data;


void
init_event_types ( );


} // End of namespace


#endif
