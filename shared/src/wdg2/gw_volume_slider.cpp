//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_volume_slider.hpp"
#include "gw_multi_slider.hpp"
#include <iostream>

namespace Wdg2
{


GW_Volume_Slider::GW_Volume_Slider (
	::QSnd2::Proxy_Slider & slider_proxy_n,
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Slider ( scene_db_n, parent_n ),
_slider_proxy ( slider_proxy_n )
{
	set_val_change_callback (
		::Context_Callback ( this, ::Wdg2::GW_Volume_Slider::write_value_to_proxy_cb ) );
	_slider_proxy.set_val_change_callback (
		::Context_Callback ( this, ::Wdg2::GW_Volume_Slider::read_value_from_proxy_cb ) );
}

GW_Volume_Slider::~GW_Volume_Slider ( )
{
	_slider_proxy.set_val_change_callback ( ::Context_Callback() );
}

void
GW_Volume_Slider::read_value_from_proxy ( )
{
	set_int_value ( _slider_proxy.int_value() );
}

void
GW_Volume_Slider::read_value_from_proxy_cb (
	void * context_n )
{
	::Wdg2::GW_Volume_Slider * slider (
		reinterpret_cast < ::Wdg2::GW_Volume_Slider * > ( context_n ) );
	slider->read_value_from_proxy();
}

void
GW_Volume_Slider::write_value_to_proxy ( ) const
{
	_slider_proxy.set_int_value ( int_value() );
}

void
GW_Volume_Slider::write_value_to_proxy_cb (
	void * context_n )
{
	::Wdg2::GW_Volume_Slider * slider (
		reinterpret_cast < ::Wdg2::GW_Volume_Slider * > ( context_n ) );
	slider->write_value_to_proxy();
}


} // End of namespace
