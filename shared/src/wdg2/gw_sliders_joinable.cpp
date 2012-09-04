//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_sliders_joinable.hpp"
#include "wdg2/gw_slider.hpp"
#include "wdg2/gw_slider_multi.hpp"
#include <iostream>

namespace Wdg2
{


GW_Sliders_Joinable::GW_Sliders_Joinable (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget ( scene_db_n, parent_n ),
_slider_multi ( 0 ),
_num_sliders ( 0 )
{
	setFlags ( QGraphicsItem::ItemHasNoContents );
}

GW_Sliders_Joinable::~GW_Sliders_Joinable ( )
{
	destroy_sliders();
}

void
GW_Sliders_Joinable::set_num_sliders (
	unsigned int num_n )
{
	if ( _num_sliders != num_n ) {
		destroy_sliders();
		_state_flags.unset ( SF_SEPARATE | SF_JOINED );
		_num_sliders = num_n;
	}
}

void
GW_Sliders_Joinable::set_sizes (
	const ::Wdg2::GW_Joinable_Sliders_Sizes & sizes_n )
{
	_sizes = sizes_n;
	set_bounding_rect ( QSizeF ( int_width(), _sizes.area_height ) );
	update_geometries();
}

void
GW_Sliders_Joinable::update_geometries ( )
{
	if ( _sliders.size() > 0 ) {
		const double delta_x ( _sizes.slider_width + _sizes.channels_gap );
		QPointF spos ( 0.0, 0.0 );

		::Wdg2::GW_Slider_Sizes lsizes;
		lsizes.size.setWidth ( _sizes.slider_width );
		lsizes.size.setHeight ( _sizes.area_height );
		lsizes.handle_length = _sizes.area_height / 10;

		_value_map.set_px_span ( lsizes.size.height() - lsizes.handle_length );

		for ( int ii=0; ii < _sliders.size(); ++ii ) {
			::Wdg2::GW_Slider * slider ( _sliders[ii] );
			slider->set_sizes ( lsizes );
			slider->setPos ( spos );
			spos.rx() += delta_x;
		}
	}
	if ( _slider_multi != 0 ) {
		{
			::Wdg2::GW_Slider_Multi_Settings lsettings;
			lsettings.num_sliders = num_sliders();
			lsettings.area_height = _sizes.area_height;
			lsettings.slider_width = _sizes.slider_width;
			lsettings.channels_gap = _sizes.channels_gap;

			_slider_multi->load_settings ( lsettings );
		}
		_slider_multi->setPos ( QPointF ( 0.0, 0.0 ) );
	}
}

unsigned int
GW_Sliders_Joinable::int_width_probe (
	const ::Wdg2::GW_Joinable_Sliders_Sizes & sizes_n ) const
{
	unsigned int iwidth ( 0 );
	const unsigned int num ( num_sliders() );
	if ( num > 0 ) {
		iwidth += sizes_n.slider_width * num;
		iwidth += sizes_n.channels_gap * ( num - 1 );
	}
	return iwidth;
}

unsigned int
GW_Sliders_Joinable::int_width ( ) const
{
	return int_width_probe ( _sizes );
}

void
GW_Sliders_Joinable::select_joined (
	bool flag_n )
{
	if ( flag_n ) {
		select_joined();
	} else {
		select_separate();
	}
}

void
GW_Sliders_Joinable::select_separate ( )
{
	if ( ( num_sliders() > 0 ) &&
		!_state_flags.has_any ( SF_SEPARATE ) )
	{
		destroy_sliders();
		_state_flags.unset ( SF_JOINED );
		_state_flags.set ( SF_SEPARATE );
		init_single_sliders();
		update_geometries();
	}
}

void
GW_Sliders_Joinable::select_joined ( )
{
	if ( ( num_sliders() > 0 ) &&
		!_state_flags.has_any ( SF_JOINED ) )
	{
		destroy_sliders();
		_state_flags.unset ( SF_SEPARATE );
		_state_flags.set ( SF_JOINED );
		init_multi_slider();
		update_geometries();
	}
}

void
GW_Sliders_Joinable::destroy_sliders ( )
{
	if ( _sliders.size() > 0 ) {
		for ( int ii=0; ii < _sliders.size(); ++ii ) {
			delete _sliders[ii];
		}
		_sliders.clear();
	}

	if ( _slider_multi != 0 ) {
		delete _slider_multi;
		_slider_multi = 0;
	}
}

void
GW_Sliders_Joinable::init_single_sliders ( )
{
	for ( unsigned int ii=0; ii < num_sliders(); ++ii ) {
		::Wdg2::GW_Slider * slider ( this->create_single_slider ( ii ) );
		slider->setParentItem ( this );
		_sliders.append ( slider );
	}
}

void
GW_Sliders_Joinable::init_multi_slider ( )
{
	_slider_multi = this->create_multi_slider();
	_slider_multi->setParentItem ( this );
}


} // End of namespace
