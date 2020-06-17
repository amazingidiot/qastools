/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "switches_pad_widgets.hpp"

namespace Wdg
{

Switches_Pad_Widgets::Switches_Pad_Widgets () {}

Switches_Pad_Widgets::~Switches_Pad_Widgets ()
{
  clear_widgets ();
}

void
Switches_Pad_Widgets::clear_widgets ()
{
  _label.reset ();
  _input.reset ();
}

void
Switches_Pad_Widgets::set_input_wdg ( QWidget * wdg_n )
{
  _input.reset ( wdg_n );
}

void
Switches_Pad_Widgets::set_label_wdg ( QLabel * wdg_n )
{
  _label.reset ( wdg_n );
}

} // namespace Wdg
