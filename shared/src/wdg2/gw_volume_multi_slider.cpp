//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_volume_multi_slider.hpp"
#include "gw_volume_slider.hpp"
#include <iostream>

namespace Wdg2
{


GW_Volume_Multi_Slider::GW_Volume_Multi_Slider (
	::QSnd2::Proxies_Group1_Slider & snd_proxies_n,
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Multi_Slider ( scene_db_n, parent_n ),
_proxies_grp ( snd_proxies_n )
{
	{
		::QSnd2::Integer_Pair vrange;
		_proxies_grp.int_range ( vrange );
		value_map().set_value_range ( vrange[0], vrange[1] );
	}
	for ( unsigned int ii=0; ii < proxies_grp().num_proxies(); ++ii ) {
		::Wdg2::GW_Volume_Slider * gw_slider (
			new ::Wdg2::GW_Volume_Slider ( *proxies_grp().slider_proxy ( ii ), scene_db() ) );
		add_slider ( gw_slider );
		gw_slider->read_value_from_proxy();
	}
}

GW_Volume_Multi_Slider::~GW_Volume_Multi_Slider ( )
{
}


} // End of namespace
