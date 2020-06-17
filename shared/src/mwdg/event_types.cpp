/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "event_types.hpp"

namespace MWdg
{

::QEvent::Type evt_separation_request;
::QEvent::Type evt_separation;
::QEvent::Type evt_values_changed;
::QEvent::Type evt_refresh_data;

void
init_event_types ()
{
  evt_separation_request =
      static_cast< QEvent::Type > ( QEvent::registerEventType () );
  evt_separation = static_cast< QEvent::Type > ( QEvent::registerEventType () );

  evt_values_changed =
      static_cast< QEvent::Type > ( QEvent::registerEventType () );

  evt_refresh_data =
      static_cast< QEvent::Type > ( QEvent::registerEventType () );
}

} // namespace MWdg
