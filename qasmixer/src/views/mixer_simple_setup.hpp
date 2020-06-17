/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mv_mixer_simple_setup_hpp__
#define __INC_mv_mixer_simple_setup_hpp__

#include "views/view_base_setup.hpp"

namespace Views
{

class Mixer_Simple_Setup : public ::Views::View_Base_Setup
{
  // Public methods
  public:
  Mixer_Simple_Setup ();

  // Public attributes
  public:
  bool show_slider_value_labels;
  bool show_stream[ 2 ];
};

} // namespace Views

#endif
