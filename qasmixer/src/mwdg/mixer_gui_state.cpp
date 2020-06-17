/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_gui_state.hpp"

namespace MWdg
{

Mixer_GUI_State_Proxy::Mixer_GUI_State_Proxy ()
{
  clear ();
}

void
Mixer_GUI_State_Proxy::clear ()
{
  group_name.clear ();
  snd_dir = 0;
  column_idx = 0;
  row_idx = 0;
  has_focus = false;
}

} // namespace MWdg
