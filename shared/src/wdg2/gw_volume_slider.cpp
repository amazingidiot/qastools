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
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Slider ( parent_n ),
_slider_proxy ( slider_proxy_n )
{
	_slider_proxy.set_val_change_callback (
		::Context_Callback ( this, ::Wdg2::GW_Slider::read_client_value_cb ) );
}

GW_Volume_Slider::~GW_Volume_Slider ( )
{
	_slider_proxy.set_val_change_callback ( ::Context_Callback() );
}

const ::Wdg2::Slider_Value_Map &
GW_Volume_Slider::value_map ( ) const
{
	const ::Wdg2::GW_Multi_Slider & sgrp (
		*static_cast < ::Wdg2::GW_Multi_Slider * > ( parentItem() ) );
	return sgrp.value_map();
}

long
GW_Volume_Slider::client_read_value ( ) const
{
	return _slider_proxy.int_value();
}

void
GW_Volume_Slider::client_set_value (
	long value_n )
{
	_slider_proxy.set_int_value ( value_n );
}


} // End of namespace
