/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "tray_mixer_mdev_setup.hpp"

Tray_Mixer_MDev_Setup::Tray_Mixer_MDev_Setup ()
: device_mode ( ::Tray_Mixer_MDev_Setup::MIXER_DEV_DEFAULT )
, user_device ( "hw:0" )
{
}
