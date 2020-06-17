/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_tray_mixer_mdev_setup_hpp__
#define __INC_tray_mixer_mdev_setup_hpp__

#include <QString>

/// @brief Tray_Mixer_MDev_Setup
///
class Tray_Mixer_MDev_Setup
{
  // Public typedefs
  public:
  enum Mixer_Device
  {
    MIXER_DEV_DEFAULT = 0,
    MIXER_DEV_CURRENT = 1,
    MIXER_DEV_USER = 2,
    MIXER_DEV_LAST = MIXER_DEV_USER
  };

  // Public methods
  public:
  Tray_Mixer_MDev_Setup ();

  // Public attributes
  public:
  /// @brief Which device to use for the mixer
  unsigned int device_mode;

  /// @brief Mixer device of the main mixer window
  QString current_device;

  /// @brief User defined mixer device
  QString user_device;
};

#endif
