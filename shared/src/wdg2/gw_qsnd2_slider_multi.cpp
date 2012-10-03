//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_qsnd2_slider_multi.hpp"
#include <iostream>

namespace Wdg2
{


GW_QSnd2_Slider_Multi::GW_QSnd2_Slider_Multi (
	::QSnd2::Proxies_Group_Sliders & proxies_grp_n,
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Slider_Multi ( scene_db_n, parent_n ),
_proxies_group ( proxies_grp_n )
{
	set_val_change_callback (
		::Context_Callback ( this, ::Wdg2::GW_QSnd2_Slider_Multi::write_value_to_proxy_cb ) );
	_proxies_group.set_val_change_callback (
		::Context_Callback ( this, ::Wdg2::GW_QSnd2_Slider_Multi::read_value_from_proxy_cb ) );
}

GW_QSnd2_Slider_Multi::~GW_QSnd2_Slider_Multi ( )
{
	_proxies_group.clear_val_change_callback();
}

void
GW_QSnd2_Slider_Multi::read_value_from_proxy ( )
{
	set_int_value ( _proxies_group.int_value_joined() );
}

void
GW_QSnd2_Slider_Multi::write_value_to_proxy ( )
{
	_proxies_group.set_int_value_joined ( int_value() );
	read_value_from_proxy();
}

void
GW_QSnd2_Slider_Multi::read_value_from_proxy_cb (
	void * context_n )
{
	::Wdg2::GW_QSnd2_Slider_Multi * slider (
		reinterpret_cast < ::Wdg2::GW_QSnd2_Slider_Multi * > ( context_n ) );
	slider->read_value_from_proxy();
}

void
GW_QSnd2_Slider_Multi::write_value_to_proxy_cb (
	void * context_n )
{
	::Wdg2::GW_QSnd2_Slider_Multi * slider (
		reinterpret_cast < ::Wdg2::GW_QSnd2_Slider_Multi * > ( context_n ) );
	slider->write_value_to_proxy();
}



} // End of namespace
