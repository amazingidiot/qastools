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
::Wdg2::GW_Widget ( scene_db_n, parent_n ),
_is_joined ( false )
{
	setFlags ( QGraphicsItem::ItemHasNoContents );
}

GW_Multi_Slider::~GW_Multi_Slider ( )
{
}

void
GW_Multi_Slider::add_slider (
	::Wdg2::GW_Slider * slider_n )
{
	if ( slider_n != 0 ) {
		slider_n->setParentItem ( this );
		slider_n->set_value_map ( &value_map() );
		_sliders.append ( slider_n );
	}
}

void
GW_Multi_Slider::set_sizes (
	const ::Wdg2::GW_Multi_Slider_Sizes & sizes_n )
{
	_sizes = sizes_n;
	set_bounding_rect ( QSizeF ( int_width(), _sizes.area_height ) );
	{
		::Wdg2::GW_Slider_Sizes lsizes;
		lsizes.size.setWidth ( _sizes.slider_width );
		lsizes.size.setHeight ( _sizes.area_height );
		lsizes.handle_length = _sizes.area_height / 10;
		_value_map.set_px_span ( lsizes.size.height() - lsizes.handle_length );
		for ( int ii=0; ii < _sliders.size(); ++ii ) {
			_sliders[ii]->set_sizes ( lsizes );
		}
	}
	update_geometries();
}

void
GW_Multi_Slider::update_geometries ( )
{
	const double delta_x ( _sizes.slider_width + _sizes.channels_hgap );
	{
		QPointF spos ( 0.0, 0.0 );
		for ( int ii=0; ii < _sliders.size(); ++ii ) {
			_sliders[ii]->setPos ( spos );
			spos.rx() += delta_x;
		}
	}
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

void
GW_Multi_Slider::set_is_joined (
	bool flag_n )
{
	if ( flag_n != _is_joined ) {
		_is_joined = flag_n;
		if ( _is_joined ) {

		} else {

		}
	}
}


} // End of namespace
