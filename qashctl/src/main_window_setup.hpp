/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_main_window_setup_hpp__
#define __INC_main_window_setup_hpp__

#include "mwdg/inputs_setup.hpp"
#include "mwdg/mixer_device_setup.hpp"
#include "views/device_selection_view_setup.hpp"
#include "views/mixer_hctl_setup.hpp"
#include <QByteArray>
#include <QKeySequence>

/// @brief Main_Window_Setup
///
class Main_Window_Setup
{
  // Public methods
  public:
  Main_Window_Setup ();

  // Public attributes
  public:
  bool show_dev_select;

  ::MWdg::Mixer_Device_Setup mixer_dev;
  ::MWdg::Inputs_Setup inputs;

  ::Views::Mixer_HCTL_Setup hctl;
  ::Views::Device_Selection_View_Setup dev_select;
};

#endif
