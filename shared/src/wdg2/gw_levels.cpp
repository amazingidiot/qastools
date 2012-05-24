//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_levels.hpp"
#include <iostream>


namespace Wdg2
{


GW_Levels::GW_Levels (
	::QSnd2::Proxies_Group1_Slider & snd_proxies_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_snd_proxies ( snd_proxies_n )
{
}

GW_Levels::~GW_Levels ( )
{
}


} // End of namespace
