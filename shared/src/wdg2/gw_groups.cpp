//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_groups.hpp"
#include <iostream>
#include <cmath>


namespace Wdg2
{


GW_Group2::GW_Group2 (
	::QSnd2::Proxies_Group2 & proxies_group_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_proxies_group ( proxies_group_n ),
_gw_levels ( 0 ),
_gw_switches ( 0 ),
_label_item ( 0 )
{
	setFlags ( QGraphicsItem::ItemHasNoContents );

	// Sliders
	{
		::QSnd2::Proxies_Group1_Slider * sliders ( _proxies_group.sliders() );
		if ( sliders != 0 ) {
			_gw_levels = new ::Wdg2::GW_Multi_Slider ( *sliders, this );
		}
	}

	// Switches
	{
		::QSnd2::Proxies_Group1_Switch * switches ( _proxies_group.switches() );
		if ( switches != 0 ) {
			_gw_switches = new ::Wdg2::GW_Multi_Switch ( *switches, this );
		}
	}

	// Label string
	_proxies_group.string_val ( _str_label, ::QSnd2::SK_NAME_L10N );

	if ( !_str_label.isEmpty() ) {
		_label_item = new QGraphicsSimpleTextItem ( _str_label, this );
	}

	update_geometries();
}

GW_Group2::~GW_Group2 ( )
{
}

QRectF
GW_Group2::boundingRect ( ) const
{
	return _brect;
}

void
GW_Group2::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;
}

void
GW_Group2::set_sizes (
	const ::Wdg2::GW_Group2_Sizes & sizes_n )
{
	_sizes = sizes_n;

	_sizes.switches_height = _sizes.slider_width;
	_sizes.switches_vgap = _sizes.channels_hgap;

	_sizes.levels_height = _sizes.height;
	_sizes.levels_height -= _sizes.switches_height;
	_sizes.levels_height -= _sizes.switches_vgap;

	update_geometries();
}

inline
::Wdg2::GW_Multi_Slider_Sizes
GW_Group2::gw_levels_sizes (
	const ::Wdg2::GW_Group2_Sizes & sizes_n ) const
{
	::Wdg2::GW_Multi_Slider_Sizes lsizes;
	lsizes.area_height = sizes_n.levels_height;
	lsizes.slider_width = sizes_n.slider_width;
	lsizes.channels_hgap = sizes_n.channels_hgap;
	return lsizes;
}

inline
::Wdg2::GW_Multi_Switch_Sizes
GW_Group2::gw_switches_sizes (
	const ::Wdg2::GW_Group2_Sizes & sizes_n ) const
{
	::Wdg2::GW_Multi_Switch_Sizes lsizes;
	lsizes.area_height = sizes_n.switches_height;
	lsizes.switch_width = sizes_n.slider_width;
	lsizes.channels_hgap = sizes_n.channels_hgap;
	return lsizes;
}

void
GW_Group2::update_geometries ( )
{
	double pos_x ( 0.0 );
	if ( _label_item != 0 ) {
		pos_x += _sizes.label_fnt_height;
		pos_x += _sizes.label_hpadi;
		pos_x += _sizes.label_hpado;

		QRectF brect ( _label_item->boundingRect() );
		_label_item->setPos ( QPointF ( _sizes.label_hpado, ::std::ceil ( brect.width() ) ) );
		_label_item->setRotation ( -90.0 );
	}

	// Levels
	if ( _gw_levels != 0 ) {
		_gw_levels->set_sizes ( gw_levels_sizes ( _sizes) );
		_gw_levels->setPos ( QPointF ( pos_x, 0.0 ) );
	}

	// Switches
	if ( _gw_switches != 0 ) {
		_gw_switches->set_sizes ( gw_switches_sizes ( _sizes ) );
		QPointF spos ( pos_x, _sizes.levels_height + _sizes.switches_vgap );
		_gw_switches->setPos ( spos );
	}

	// Bounding rect
	_brect.moveTopLeft ( QPointF ( 0.0, 0.0 ) );
	_brect.setHeight ( _sizes.height );
	_brect.setWidth ( int_width() );
}

unsigned int
GW_Group2::int_width ( ) const
{
	return int_width_probe ( _sizes );
}

unsigned int
GW_Group2::int_width_probe (
	const ::Wdg2::GW_Group2_Sizes & sizes_n ) const
{
	unsigned int iwidth ( 0 );
	if ( _gw_levels != 0 ) {
		iwidth = _gw_levels->int_width_probe (
			gw_levels_sizes ( sizes_n ) );
	}
	if ( _gw_switches != 0 ) {
		const unsigned int giwidth (
			_gw_switches->int_width_probe (
				gw_switches_sizes ( sizes_n ) ) );
		if ( giwidth > iwidth ) {
			iwidth = giwidth;
		}
	}
	if ( iwidth > 0 ) {
		if ( _label_item != 0 ) {
			iwidth += sizes_n.label_fnt_height;
			iwidth += sizes_n.label_hpadi;
			iwidth += sizes_n.label_hpado;
		}
	}
	return iwidth;
}



GW_Group3::GW_Group3 (
	::QSnd2::Proxies_Group3 & proxies_group_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_proxies_group ( proxies_group_n )
{
	for ( unsigned int ii=0; ii < _proxies_group.num_groups(); ++ii ) {
		::Wdg2::GW_Group2 * grp2 (
			new ::Wdg2::GW_Group2 ( *_proxies_group.group ( ii ), this ) );
		_gw_groups.append ( grp2 );
	}
}

GW_Group3::~GW_Group3 ( )
{
}

QRectF
GW_Group3::boundingRect ( ) const
{
	return QRectF ( 0, 0, 0, 0 );
}

void
GW_Group3::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;
}

void
GW_Group3::set_sizes (
	const ::Wdg2::GW_Group3_Sizes & sizes_n )
{
	_sizes = sizes_n;
	update_geometries();
}

inline
::Wdg2::GW_Group2_Sizes
GW_Group3::gw_group2_sizes (
	const ::Wdg2::GW_Group3_Sizes & sizes_n ) const
{
	::Wdg2::GW_Group2_Sizes lsizes;
	lsizes.height = sizes_n.height;
	lsizes.slider_width = sizes_n.slider_width;
	lsizes.channels_hgap = sizes_n.channels_hgap;
	return lsizes;
}

void
GW_Group3::update_geometries ( )
{
	unsigned int xpos ( 0 );
	const unsigned int group2_hgap ( _sizes.group2_hgap );
	for ( int ii=0; ii < _gw_groups.size(); ++ii ) {
		unsigned int grp_width;
		{
			::Wdg2::GW_Group2 * grp2 ( _gw_groups[ii] );
			grp2->set_sizes ( gw_group2_sizes ( _sizes ) );
			grp2->setPos ( xpos, 0.0 );
			grp_width = grp2->int_width();
		}

		if ( grp_width > 0 ) {
			xpos += grp_width;
			xpos += group2_hgap;
		}
	}
}

unsigned int
GW_Group3::int_width ( ) const
{
	return int_width_probe ( _sizes );
}

unsigned int
GW_Group3::int_width_probe (
	const ::Wdg2::GW_Group3_Sizes & sizes_n ) const
{
	unsigned int iwidth ( 0 );
	const unsigned int group2_hgap ( sizes_n.group2_hgap );
	for ( int ii=0; ii < _gw_groups.size(); ++ii ) {
		::Wdg2::GW_Group2 * grp2 ( _gw_groups[ii] );
		const unsigned int grp_width (
			grp2->int_width_probe ( gw_group2_sizes ( sizes_n ) ) );
		if ( grp_width > 0 ) {
			if ( iwidth > 0 ) {
				iwidth += group2_hgap;
			}
			iwidth += grp_width;
		}
	}
	return iwidth;
}



GW_Group4::GW_Group4 (
	::QSnd2::Proxies_Group4 & proxies_group_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_proxies_group ( proxies_group_n )
{
	for ( unsigned int ii=0; ii < _proxies_group.num_groups(); ++ii ) {
		::Wdg2::GW_Group3 * grp3 (
			new ::Wdg2::GW_Group3 ( *_proxies_group.group ( ii ), this ) );
		_gw_groups.append ( grp3 );
	}
}

GW_Group4::~GW_Group4 ( )
{
}

QRectF
GW_Group4::boundingRect ( ) const
{
	return QRectF ( 0, 0, 0, 0 );
}

void
GW_Group4::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;
}

void
GW_Group4::set_sizes (
	const ::Wdg2::GW_Group4_Sizes & sizes_n )
{
	_sizes = sizes_n;
	update_geometries();
}

inline
::Wdg2::GW_Group3_Sizes
GW_Group4::gw_group3_sizes (
	const ::Wdg2::GW_Group4_Sizes & sizes_n ) const
{
	::Wdg2::GW_Group3_Sizes lsizes;
	lsizes.height = sizes_n.height;
	lsizes.slider_width = sizes_n.slider_width;
	lsizes.channels_hgap = sizes_n.channels_hgap;
	lsizes.group2_hgap = sizes_n.group2_hgap;
	return lsizes;
}

void
GW_Group4::update_geometries ( )
{
	unsigned int xpos ( 0 );
	const unsigned int group3_hgap ( _sizes.group3_hgap );
	for ( int ii=0; ii < _gw_groups.size(); ++ii ) {
		unsigned int grp_width;
		{
			::Wdg2::GW_Group3 * grp3 ( _gw_groups[ii] );
			grp3->set_sizes ( gw_group3_sizes ( _sizes ) );
			grp3->setPos ( xpos, 0.0 );
			grp_width = grp3->int_width();
		}

		if ( grp_width > 0 ) {
			xpos += grp_width;
			xpos += group3_hgap;
		}
	}
}

unsigned int
GW_Group4::int_width ( ) const
{
	return int_width_probe ( _sizes );
}

unsigned int
GW_Group4::int_width_probe (
	const ::Wdg2::GW_Group4_Sizes & sizes_n ) const
{
	unsigned int iwidth ( 0 );
	const unsigned int group3_hgap ( _sizes.group3_hgap );
	for ( int ii=0; ii < _gw_groups.size(); ++ii ) {
		::Wdg2::GW_Group3 * grp3 (  _gw_groups[ii] );
		const unsigned int grp_width (
			grp3->int_width_probe ( gw_group3_sizes ( sizes_n ) ) );
		if ( grp_width > 0 ) {
			if ( iwidth > 0 ) {
				iwidth += group3_hgap;
			}
			iwidth += grp_width;
		}
	}
	return iwidth;
}


} // End of namespace
