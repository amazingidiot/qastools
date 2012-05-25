//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_groups.hpp"
#include <iostream>


namespace Wdg2
{


GW_Group2::GW_Group2 (
	::QSnd2::Proxies_Group2 & proxies_group_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_proxies_group ( proxies_group_n ),
_gw_levels ( 0 ),
_gw_switches ( 0 )
{
	::QSnd2::Proxies_Group1_Slider * sliders ( _proxies_group.sliders() );
	::QSnd2::Proxies_Group1_Switch * switches ( _proxies_group.switches() );

	if ( sliders != 0 ) {
		_gw_levels = new ::Wdg2::GW_Levels ( *sliders, this );
	}
	if ( switches != 0 ) {
		_gw_switches = new ::Wdg2::GW_Switches ( *switches, this );
	}
}

GW_Group2::~GW_Group2 ( )
{
}

QRectF
GW_Group2::boundingRect ( ) const
{
	return QRectF();
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
	update_geometries();
}

void
GW_Group2::update_geometries ( )
{
	_switches_height = _sizes.slider_width;
	_switches_vgap = _sizes.channels_hgap;

	_levels_height = _sizes.height;
	_levels_height -= _switches_height;
	_levels_height -= _switches_vgap;

	if ( _gw_levels != 0 ) {
		{
			::Wdg2::GW_Levels_Sizes lsizes;
			lsizes.area_height = _levels_height;
			lsizes.slider_width = _sizes.slider_width;
			lsizes.channels_hgap = _sizes.channels_hgap;
			_gw_levels->set_sizes ( lsizes );
		}
		_gw_levels->setPos ( QPointF ( 0.0, 0.0 ) );
	}
	if ( _gw_switches != 0 ) {
		{
			::Wdg2::GW_Switches_Sizes lsizes;
			lsizes.area_height = _switches_height;
			lsizes.switch_width = _sizes.slider_width;
			lsizes.channels_hgap = _sizes.channels_hgap;
			_gw_switches->set_sizes ( lsizes );
		}
		_gw_switches->setPos ( 0.0, _levels_height + _switches_vgap );
	}
}

unsigned int
GW_Group2::int_width ( ) const
{
	unsigned int iwidth ( 0 );
	if ( _gw_levels != 0 ) {
		iwidth = _gw_levels->int_width();
	}
	if ( _gw_switches != 0 ) {
		const unsigned int giwidth ( _gw_switches->int_width() );
		if ( giwidth > iwidth ) {
			iwidth = giwidth;
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
		_gw_groups.append (
			new ::Wdg2::GW_Group2 ( *_proxies_group.group ( ii ), this ) );
	}
}

GW_Group3::~GW_Group3 ( )
{
}

QRectF
GW_Group3::boundingRect ( ) const
{
	return QRectF();
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

void
GW_Group3::update_geometries ( )
{
	unsigned int xpos ( 0 );
	const unsigned int group2_hgap ( _sizes.group2_hgap );
	for ( int ii=0; ii < _gw_groups.size(); ++ii ) {
		unsigned int grp_width;
		{
			::Wdg2::GW_Group2 * grp ( _gw_groups[ii] );
			{
				::Wdg2::GW_Group2_Sizes lsizes;
				lsizes.height = _sizes.height;
				lsizes.slider_width = _sizes.slider_width;
				lsizes.channels_hgap = _sizes.channels_hgap;
				grp->set_sizes ( lsizes );
			}
			grp->setPos ( xpos, 0.0 );
			grp_width = grp->int_width();
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
	unsigned int iwidth ( 0 );
	const unsigned int group2_hgap ( _sizes.group2_hgap );
	for ( int ii=0; ii < _gw_groups.size(); ++ii ) {
		const unsigned int grp_width ( _gw_groups[ii]->int_width() );
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
		_gw_groups.append (
			new ::Wdg2::GW_Group3 ( *_proxies_group.group ( ii ), this ) );
	}
}

GW_Group4::~GW_Group4 ( )
{
}

QRectF
GW_Group4::boundingRect ( ) const
{
	return QRectF();
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

void
GW_Group4::update_geometries ( )
{
	unsigned int xpos ( 0 );
	const unsigned int group3_hgap ( _sizes.group3_hgap );
	for ( int ii=0; ii < _gw_groups.size(); ++ii ) {
		unsigned int grp_width;
		{
			::Wdg2::GW_Group3 * grp ( _gw_groups[ii] );
			{
				::Wdg2::GW_Group3_Sizes lsizes;
				lsizes.height = _sizes.height;
				lsizes.slider_width = _sizes.slider_width;
				lsizes.channels_hgap = _sizes.channels_hgap;
				lsizes.group2_hgap = _sizes.group2_hgap;
				grp->set_sizes ( lsizes );
			}
			grp->setPos ( xpos, 0.0 );
			grp_width = grp->int_width();
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
	unsigned int iwidth ( 0 );
	const unsigned int group3_hgap ( _sizes.group3_hgap );
	for ( int ii=0; ii < _gw_groups.size(); ++ii ) {
		const unsigned int grp_width ( _gw_groups[ii]->int_width() );
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
