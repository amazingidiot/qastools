//
// C++ Implementation:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "tray_mixer_mdev_setup.hpp"


Tray_Mixer_MDev_Setup::Tray_Mixer_MDev_Setup ( ) :
device_mode ( ::Tray_Mixer_MDev_Setup::MIXER_DEV_DEFAULT ),
user_device ( "hw:0" )
{
}
