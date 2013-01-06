//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "gw_qsnd2_sliders_joinable.hpp"
#include "gw_qsnd2_slider.hpp"
#include "gw_qsnd2_slider_multi.hpp"
#include <iostream>

namespace Wdg2
{


GW_QSnd2_Sliders_Joinable::GW_QSnd2_Sliders_Joinable (
	::QSnd2::Proxies_Group_Sliders & snd_proxies_n,
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
	set_num_sliders ( proxies_grp().num_children() );
}

GW_QSnd2_Sliders_Joinable::~GW_QSnd2_Sliders_Joinable ( )
{
}

::Wdg2::GW_Slider *
GW_QSnd2_Sliders_Joinable::create_single_slider (
	unsigned int idx_n )
{
	::Wdg2::GW_QSnd2_Slider * slider;
	{
		::QSnd2::Proxy_Slider & sproxy ( *proxies_grp().slider_proxy ( idx_n ) );
		slider = new ::Wdg2::GW_QSnd2_Slider ( sproxy, scene_db() );
	}
	slider->set_value_map ( &value_map() );
	slider->read_value_from_proxy();
	return slider;
}

::Wdg2::GW_Slider_Multi *
GW_QSnd2_Sliders_Joinable::create_multi_slider ( )
{
	::Wdg2::GW_QSnd2_Slider_Multi * slider (
		new ::Wdg2::GW_QSnd2_Slider_Multi ( proxies_grp(), scene_db() ) );
	slider->set_value_map ( &value_map() );
	slider->read_value_from_proxy();
	slider->write_value_to_proxy();
	return slider;
}


} // End of namespace
