/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mwdg_mixer_device_setup_hpp__
#define __INC_mwdg_mixer_device_setup_hpp__

#include <QString>

namespace MWdg
{

/// @brief Mixer_Device_Setup
///
class Mixer_Device_Setup
{
  // Public methods
  public:
  Mixer_Device_Setup ();

  // Public attributes
  public:
  QString ctl_addr;
};

} // namespace MWdg

#endif
