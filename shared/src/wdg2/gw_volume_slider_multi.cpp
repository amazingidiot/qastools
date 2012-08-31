//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_volume_slider_multi.hpp"
#include <iostream>

namespace Wdg2
{


GW_Volume_Slider_Multi::GW_Volume_Slider_Multi (
	::QSnd2::Proxies_Group1_Slider & proxy_group_n,
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Slider_Multi ( scene_db_n, parent_n ),
_proxy_group ( proxy_group_n )
{
}

GW_Volume_Slider_Multi::~GW_Volume_Slider_Multi ( )
{
}

void
GW_Volume_Slider_Multi::read_value_from_proxy ( )
{
	set_int_value ( _proxy_group.int_value_joined() );
}

void
GW_Volume_Slider_Multi::write_value_to_proxy ( ) const
{
	_proxy_group.set_int_value_joined ( int_value() );
}

void
GW_Volume_Slider_Multi::read_value_from_proxy_cb (
	void * context_n )
{
	::Wdg2::GW_Volume_Slider_Multi * slider (
		reinterpret_cast < ::Wdg2::GW_Volume_Slider_Multi * > ( context_n ) );
	slider->read_value_from_proxy();
}

void
GW_Volume_Slider_Multi::write_value_to_proxy_cb (
	void * context_n )
{
	::Wdg2::GW_Volume_Slider_Multi * slider (
		reinterpret_cast < ::Wdg2::GW_Volume_Slider_Multi * > ( context_n ) );
	slider->write_value_to_proxy();
}



} // End of namespace
