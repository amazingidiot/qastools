//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_volume_sliders_joinable.hpp"
#include "gw_volume_slider.hpp"
#include "gw_volume_slider_multi.hpp"
#include <iostream>

namespace Wdg2
{


GW_Volume_Sliders_Joinable::GW_Volume_Sliders_Joinable (
	::QSnd2::Proxies_Group1_Slider & snd_proxies_n,
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Sliders_Joinable ( scene_db_n, parent_n ),
_proxies_grp ( snd_proxies_n )
{
	{
		::QSnd2::Integer_Pair vrange;
		_proxies_grp.int_range ( vrange );
		value_map().set_value_range ( vrange[0], vrange[1] );
	}
	set_num_sliders ( proxies_grp().num_proxies() );
}

GW_Volume_Sliders_Joinable::~GW_Volume_Sliders_Joinable ( )
{
}

::Wdg2::GW_Slider *
GW_Volume_Sliders_Joinable::create_single_slider (
	unsigned int idx_n )
{
	::Wdg2::GW_Volume_Slider * slider;
	{
		::QSnd2::Proxy_Slider & sproxy ( *proxies_grp().slider_proxy ( idx_n ) );
		slider = new ::Wdg2::GW_Volume_Slider ( sproxy, scene_db() );
	}
	slider->set_value_map ( &value_map() );
	slider->read_value_from_proxy();
	return slider;
}

::Wdg2::GW_Slider_Multi *
GW_Volume_Sliders_Joinable::create_multi_slider ( )
{
	::Wdg2::GW_Volume_Slider_Multi * slider (
		new ::Wdg2::GW_Volume_Slider_Multi ( proxies_grp(), scene_db() ) );
	slider->set_value_map ( &value_map() );
	slider->read_value_from_proxy();
	return slider;
}


} // End of namespace
