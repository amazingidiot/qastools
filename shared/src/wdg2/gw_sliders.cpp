//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_sliders.hpp"
#include <iostream>


namespace Wdg2
{


GW_Sliders::GW_Sliders (
	::QSnd2::Proxies_Group1_Slider & snd_proxies_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_snd_proxies ( snd_proxies_n )
{
}

GW_Sliders::~GW_Sliders ( )
{
}


} // End of namespace
