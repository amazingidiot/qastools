//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
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
