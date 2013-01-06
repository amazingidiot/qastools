//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_wdg_event_types_hpp__
#define __INC_wdg_event_types_hpp__

#include <QEvent>


namespace Wdg
{

extern ::QEvent::Type evt_pass_event_focus;
extern ::QEvent::Type evt_pass_event_key;


void
init_event_types ( );


} // End of namespace


#endif
