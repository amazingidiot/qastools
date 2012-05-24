//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_switches.hpp"
#include <iostream>


namespace Wdg2
{


GW_Switches::GW_Switches (
	::QSnd2::Proxies_Group1_Switch & snd_proxies_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_snd_proxies ( snd_proxies_n )
{
}

GW_Switches::~GW_Switches ( )
{
}


} // End of namespace
