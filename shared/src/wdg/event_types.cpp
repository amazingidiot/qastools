/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "event_types.hpp"

namespace Wdg
{

::QEvent::Type evt_pass_event_focus;
::QEvent::Type evt_pass_event_key;

void
init_event_types ()
{
  evt_pass_event_focus =
      static_cast< QEvent::Type > ( QEvent::registerEventType () );
  evt_pass_event_key =
      static_cast< QEvent::Type > ( QEvent::registerEventType () );
}

} // namespace Wdg
