/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "init_globals.hpp"
#include "mwdg/event_types.hpp"
#include "qsnd/event_types.hpp"
#include "wdg/event_types.hpp"

int
init_globals ()
{
  ::QSnd::init_event_types ();
  ::Wdg::init_event_types ();
  ::MWdg::init_event_types ();

  return 0;
}
