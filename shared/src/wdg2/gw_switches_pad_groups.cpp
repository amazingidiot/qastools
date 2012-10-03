//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_switches_pad_groups.hpp"
#include <iostream>
#include <cmath>

namespace Wdg2
{


GW_SwPad_Group2::GW_SwPad_Group2 (
	::QSnd2::Proxies_Group2 & proxies_group_n,
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
GW_Widget ( scene_db_n, parent_n ),
_proxies_group ( proxies_group_n ),
_gw_switches ( 0 ),
_label_item ( 0 )
{
	setFlags ( QGraphicsItem::ItemHasNoContents );

	// Switches
	{
		::QSnd2::Proxies_Group_Switches * pswitches ( _proxies_group.switches() );
		if ( pswitches != 0 ) {
			_gw_switches = new ::Wdg2::GW_QSnd2_Switches_Joinable ( *pswitches, scene_db(), this );

			// select joined/separate
			if ( pswitches->values_equal() ) {
				_gw_switches->select_joined();
			} else {
				_gw_switches->select_separate();
			}
		}
	}

	// Label string
	_proxies_group.string_val ( _str_label, ::QSnd2::SK_NAME_L10N );

	if ( !_str_label.isEmpty() ) {
		_label_item = new ::Wdg2::GW_Label ( scene_db(), this );
		_label_item->set_text ( _str_label );
	}

	update_geometries();
}

GW_SwPad_Group2::~GW_SwPad_Group2 ( )
{
}

void
GW_SwPad_Group2::set_sizes (
	const ::Wdg2::GW_SwPad_Group2_Sizes & sizes_n )
{
	_sizes = sizes_n;
	::Wdg2::GW_Widget::set_size ( QSize ( int_width(), _sizes.height ) );
	update_geometries();
}

inline
::Wdg2::GW_Switches_Joinable_Settings
GW_SwPad_Group2::gw_switches_settings (
	const ::Wdg2::GW_SwPad_Group2_Sizes & sizes_n ) const
{
	::Wdg2::GW_Switches_Joinable_Settings lsizes;
	lsizes.area_height = sizes_n.switch_height;
	lsizes.switch_width = sizes_n.switch_height;
	lsizes.channels_gap = sizes_n.channels_gap;
	return lsizes;
}

void
GW_SwPad_Group2::update_geometries ( )
{
	double pos_y ( 0.0 );

	// Label
	if ( _label_item != 0 ) {
		_label_item->setPos ( QPointF ( 0, 0 ) );
		pos_y = _label_item->size().height();
	}
	pos_y += _sizes.label_gap;

	// Switches
	if ( _gw_switches != 0 ) {
		_gw_switches->load_settings ( gw_switches_settings ( _sizes ) );
		QPointF spos ( 0.0, pos_y );
		_gw_switches->setPos ( spos );
	}
}

unsigned int
GW_SwPad_Group2::int_width ( ) const
{
	return int_width_probe ( _sizes );
}

unsigned int
GW_SwPad_Group2::int_width_probe (
	const ::Wdg2::GW_SwPad_Group2_Sizes & sizes_n ) const
{
	unsigned int iwidth ( 0 );
	if ( _gw_switches != 0 ) {
		const unsigned int giwidth (
			_gw_switches->int_width_probe (
				gw_switches_settings ( sizes_n ) ) );
		iwidth = giwidth;
	}
	if ( iwidth > 0 ) {
		if ( _label_item != 0 ) {
			unsigned int lwidth ( _label_item->size().width() );
			if ( lwidth > iwidth ) {
				iwidth = lwidth;
			}
		}
	}
	return iwidth;
}



GW_SwPad_Group3::GW_SwPad_Group3 (
	::QSnd2::Proxies_Group3 & proxies_group_n,
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
GW_Widget ( scene_db_n, parent_n ),
_proxies_group ( proxies_group_n )
{
	setFlags ( QGraphicsItem::ItemHasNoContents );

	for ( unsigned int ii=0; ii < _proxies_group.num_children(); ++ii ) {
		::QSnd2::Proxies_Group2 * pgrp2 ( _proxies_group.child_group ( ii ) );
		if ( ( pgrp2->sliders() == 0 ) && ( pgrp2->switches() != 0 ) ) {
			::Wdg2::GW_SwPad_Group2 * grp2 (
				new ::Wdg2::GW_SwPad_Group2 ( *pgrp2, scene_db(), this ) );
			_gw_groups.append ( grp2 );
		}
	}
}

GW_SwPad_Group3::~GW_SwPad_Group3 ( )
{
}

void
GW_SwPad_Group3::set_sizes (
	const ::Wdg2::GW_SwPad_Group3_Sizes & sizes_n )
{
	_sizes = sizes_n;
	::Wdg2::GW_Widget::set_size ( QSize ( int_width(), _sizes.height ) );
	update_geometries();
}

inline
::Wdg2::GW_SwPad_Group2_Sizes
GW_SwPad_Group3::gw_group2_sizes (
	const ::Wdg2::GW_SwPad_Group3_Sizes & sizes_n ) const
{
	::Wdg2::GW_SwPad_Group2_Sizes lsizes;
	lsizes.height = sizes_n.height;
	lsizes.channels_gap = sizes_n.channels_gap;
	return lsizes;
}

void
GW_SwPad_Group3::update_geometries ( )
{
	unsigned int xpos ( 0 );
	const unsigned int group2_hgap ( _sizes.group2_gap );
	for ( int ii=0; ii < _gw_groups.size(); ++ii ) {
		unsigned int grp_width;
		{
			::Wdg2::GW_SwPad_Group2 * grp2 ( _gw_groups[ii] );
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
GW_SwPad_Group3::int_width ( ) const
{
	return int_width_probe ( _sizes );
}

unsigned int
GW_SwPad_Group3::int_width_probe (
	const ::Wdg2::GW_SwPad_Group3_Sizes & sizes_n ) const
{
	unsigned int iwidth ( 0 );
	const unsigned int group2_hgap ( sizes_n.group2_gap );
	for ( int ii=0; ii < _gw_groups.size(); ++ii ) {
		::Wdg2::GW_SwPad_Group2 * grp2 ( _gw_groups[ii] );
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



GW_SwPad_Group4::GW_SwPad_Group4 (
	::QSnd2::Proxies_Group4 & proxies_group_n,
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget ( scene_db_n, parent_n ),
_proxies_group ( proxies_group_n )
{
	setFlags ( QGraphicsItem::ItemHasNoContents );

	for ( unsigned int ii=0; ii < _proxies_group.num_children(); ++ii ) {
		::QSnd2::Proxies_Group3 * pgrp3 ( _proxies_group.child_group ( ii ) );
		QScopedPointer < ::Wdg2::GW_SwPad_Group3 > grp3 (
			new ::Wdg2::GW_SwPad_Group3 ( *pgrp3, scene_db(), this ) );
		if ( grp3->num_children() > 0 ) {
			_gw_groups.append ( grp3.take() );
		}
	}
}

GW_SwPad_Group4::~GW_SwPad_Group4 ( )
{
}

void
GW_SwPad_Group4::set_sizes (
	const ::Wdg2::GW_SwPad_Group4_Sizes & sizes_n )
{
	_sizes = sizes_n;
	::Wdg2::GW_Widget::set_size ( QSize ( int_width(), _sizes.height ) );
	update_geometries();
}

inline
::Wdg2::GW_SwPad_Group3_Sizes
GW_SwPad_Group4::gw_group3_sizes (
	const ::Wdg2::GW_SwPad_Group4_Sizes & sizes_n ) const
{
	::Wdg2::GW_SwPad_Group3_Sizes lsizes;
	lsizes.height = sizes_n.height;
	lsizes.channels_gap = sizes_n.channels_gap;
	lsizes.group2_gap = sizes_n.group2_gap;
	return lsizes;
}

void
GW_SwPad_Group4::update_geometries ( )
{
	unsigned int xpos ( 0 );
	const unsigned int group3_hgap ( _sizes.group3_gap );
	for ( int ii=0; ii < _gw_groups.size(); ++ii ) {
		unsigned int grp_width;
		{
			::Wdg2::GW_SwPad_Group3 * grp3 ( _gw_groups[ii] );
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
GW_SwPad_Group4::int_width ( ) const
{
	return int_width_probe ( _sizes );
}

unsigned int
GW_SwPad_Group4::int_width_probe (
	const ::Wdg2::GW_SwPad_Group4_Sizes & sizes_n ) const
{
	unsigned int iwidth ( 0 );
	const unsigned int group3_hgap ( sizes_n.group3_gap );
	for ( int ii=0; ii < _gw_groups.size(); ++ii ) {
		::Wdg2::GW_SwPad_Group3 * grp3 (  _gw_groups[ii] );
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
