/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "qsnd/event_types.hpp"

namespace QSnd
{

::QEvent::Type evt_update_values_request;
::QEvent::Type evt_update_values;

::QEvent::Type evt_reload_request;
::QEvent::Type evt_reload;

void
init_event_types ()
{
  evt_update_values_request =
      static_cast< QEvent::Type > ( QEvent::registerEventType () );
  evt_update_values =
      static_cast< QEvent::Type > ( QEvent::registerEventType () );

  evt_reload_request =
      static_cast< QEvent::Type > ( QEvent::registerEventType () );
  evt_reload = static_cast< QEvent::Type > ( QEvent::registerEventType () );
}

} // namespace QSnd
