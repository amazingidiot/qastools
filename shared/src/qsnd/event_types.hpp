/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_event_types_hpp__
#define __INC_qsnd_event_types_hpp__

#include <QEvent>

namespace QSnd
{

extern QEvent::Type evt_update_values_request;
extern QEvent::Type evt_update_values;

extern QEvent::Type evt_reload_request;
extern QEvent::Type evt_reload;

void
init_event_types ();

} // namespace QSnd

#endif
