//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_switches_joinable.hpp"
#include "gw_switch.hpp"
#include "gw_switch_multi.hpp"
#include <iostream>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


namespace Wdg2
{


GW_Switches_Joinable::GW_Switches_Joinable (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget ( scene_db_n, parent_n ),
_switch_multi ( 0 ),
_num_switches ( 0 )
{
	setFlags ( QGraphicsItem::ItemHasNoContents );
}

GW_Switches_Joinable::~GW_Switches_Joinable ( )
{
	destroy_switches();
}

void
GW_Switches_Joinable::set_num_switches (
	unsigned int num_n )
{
	if ( _num_switches != num_n ) {
		destroy_switches();
		_state_flags.unset ( SF_SEPARATE | SF_JOINED );
		_num_switches = num_n;
	}
}

void
GW_Switches_Joinable::load_settings (
	const ::Wdg2::GW_Switches_Joinable_Settings & settings_n )
{
	_settings = settings_n;
	set_bounding_rect ( QSizeF ( int_width(), _settings.area_height ) );

	{
		QSize ssize ( _settings.switch_width, _settings.area_height );
		for ( int ii=0; ii < _switches.size(); ++ii ) {
			_switches[ii]->set_switch_size ( ssize );
		}
	}

	update_geometries();
}

void
GW_Switches_Joinable::update_geometries ( )
{
	if ( _switches.size() > 0 ) {
		const double delta_x ( _settings.switch_width + _settings.channels_gap );
		QPointF spos ( 0.0, 0.0 );
		QSize sw_size ( _settings.switch_width, _settings.area_height );

		for ( int ii=0; ii < _switches.size(); ++ii ) {
			::Wdg2::GW_Switch * gw_switch ( _switches[ii] );
			gw_switch->set_switch_size ( sw_size );
			gw_switch->setPos ( spos );
			spos.rx() += delta_x;
		}
	}
	if ( _switch_multi != 0 ) {
		{
			::Wdg2::GW_Switch_Multi_Settings sw_settings;
			sw_settings.num_switches = num_switches();
			sw_settings.area_height = _settings.area_height;
			sw_settings.switch_width = _settings.switch_width;
			sw_settings.channels_gap = _settings.channels_gap;

			_switch_multi->load_settings_multi ( sw_settings );
		}
		_switch_multi->setPos ( QPointF ( 0.0, 0.0 ) );
	}
}

unsigned int
GW_Switches_Joinable::int_width ( ) const
{
	return int_width_probe ( settings() );
}

unsigned int
GW_Switches_Joinable::int_width_probe (
	const ::Wdg2::GW_Switches_Joinable_Settings & settings_n ) const
{
	unsigned int iwidth ( 0 );
	if ( num_switches() > 0 ) {
		iwidth += settings_n.switch_width * num_switches();
		iwidth += settings_n.channels_gap * ( num_switches() - 1 );
	}
	return iwidth;
}

void
GW_Switches_Joinable::select_joined (
	bool flag_n )
{
	if ( flag_n ) {
		select_joined();
	} else {
		select_separate();
	}
}

void
GW_Switches_Joinable::select_separate ( )
{
	if ( ( num_switches() > 0 ) &&
		!_state_flags.has_any ( SF_SEPARATE ) )
	{
		destroy_switches();
		_state_flags.unset ( SF_JOINED );
		_state_flags.set ( SF_SEPARATE );
		init_switches_single();
		update_geometries();
	}
}

void
GW_Switches_Joinable::select_joined ( )
{
	if ( ( num_switches() > 0 ) &&
		!_state_flags.has_any ( SF_JOINED ) )
	{
		destroy_switches();
		_state_flags.unset ( SF_SEPARATE );
		_state_flags.set ( SF_JOINED );
		init_switch_multi();
		update_geometries();
	}
}

void
GW_Switches_Joinable::destroy_switches ( )
{
	if ( _switches.size() > 0 ) {
		for ( int ii=0; ii < _switches.size(); ++ii ) {
			delete _switches[ii];
		}
		_switches.clear();
	}
	if ( _switch_multi != 0 ) {
		delete _switch_multi;
		_switch_multi = 0;
	}
}

void
GW_Switches_Joinable::init_switches_single ( )
{
	for ( unsigned int ii=0; ii < num_switches(); ++ii ) {
		::Wdg2::GW_Switch * gw_switch ( this->create_switch_single ( ii ) );
		gw_switch->setParentItem ( this );
		_switches.append ( gw_switch );
	}
}

void
GW_Switches_Joinable::init_switch_multi ( )
{
	_switch_multi = this->create_switch_multi();
	_switch_multi->setParentItem ( this );
}


} // End of namespace
