//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_multi_slider.hpp"
#include <iostream>

namespace Wdg2
{


GW_Multi_Slider::GW_Multi_Slider (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Slider ( scene_db_n, parent_n )
{
}

GW_Multi_Slider::~GW_Multi_Slider ( )
{
}

void
GW_Multi_Slider::set_num_sliders (
	unsigned int num_n )
{
	if ( _num_sliders != num_n ) {
		_num_sliders = num_n;
		update_geometries();
	}
}

void
GW_Multi_Slider::set_sizes (
	const ::Wdg2::GW_Multi_Slider_Sizes & sizes_n )
{
	_sizes = sizes_n;
	update_geometries();
}

void
GW_Multi_Slider::update_geometries ( )
{
	set_bounding_rect ( QSizeF ( int_width(), _sizes.area_height ) );
}

unsigned int
GW_Multi_Slider::int_width_probe (
	const ::Wdg2::GW_Multi_Slider_Sizes & sizes_n ) const
{
	unsigned int iwidth ( 0 );
	const unsigned int num ( num_sliders() );
	if ( num > 0 ) {
		iwidth += sizes_n.slider_width * num;
		iwidth += sizes_n.channels_hgap * ( num - 1 );
	}
	return iwidth;
}

unsigned int
GW_Multi_Slider::int_width ( ) const
{
	return int_width_probe ( _sizes );
}


} // End of namespace
