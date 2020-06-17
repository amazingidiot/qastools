/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_simple_setup.hpp"

namespace MWdg
{

Mixer_Simple_Setup::Mixer_Simple_Setup ()
{
  show_slider_value_labels = true;
  show_stream[ 0 ] = true;
  show_stream[ 1 ] = false;

  mixer_simple = 0;
  wdg_style_db = 0;
  image_alloc = 0;
}

} // namespace MWdg
