/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_wdg_event_types_hpp__
#define __INC_wdg_event_types_hpp__

#include <QEvent>

namespace Wdg
{

extern ::QEvent::Type evt_pass_event_focus;
extern ::QEvent::Type evt_pass_event_key;

void
init_event_types ();

} // namespace Wdg

#endif
