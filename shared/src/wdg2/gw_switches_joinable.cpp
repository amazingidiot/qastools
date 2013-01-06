//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "gw_switches_joinable.hpp"
#include "gw_switch.hpp"
#include "gw_switch_multi.hpp"
#include "wdg2/scene_database.hpp"
#include <QEvent>
#include <QKeyEvent>
#include <iostream>

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
	setFiltersChildEvents ( true );
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
	::Wdg2::GW_Widget::set_size ( QSize ( int_width(), _settings.area_height ) );
	{
		const QSize ssize ( _settings.switch_width, _settings.area_height );
		for ( int ii=0; ii < _switches.size(); ++ii ) {
			_switches[ii]->set_size ( ssize );
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
			gw_switch->set_size ( sw_size );
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
	if ( num_switches() > 0 ) {
		unsigned int flag_set ( SF_JOINED );
		unsigned int flag_unset ( SF_SEPARATE );
		if ( !flag_n ) {
			::std::swap ( flag_set, flag_unset );
		}
		if ( !_state_flags.has_any ( flag_set ) ) {
			const bool had_focus ( has_focus() );
			destroy_switches();
			_state_flags.unset ( flag_unset );
			_state_flags.set ( flag_set );
			init_switches();
			update_geometries();
			if ( had_focus ) {
				set_focus();
			}
		}
	}
}

void
GW_Switches_Joinable::select_separate ( )
{
	select_joined ( false );
}

void
GW_Switches_Joinable::select_joined ( )
{
	select_joined ( true );
}

void
GW_Switches_Joinable::toggle_joined ( )
{
	if ( _state_flags.has_any ( SF_JOINED | SF_SEPARATE ) ) {
		if ( _state_flags.has_any ( SF_JOINED ) ) {
			select_separate();
		} else {
			select_joined();
		}
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
GW_Switches_Joinable::init_switches ( )
{
	if ( _state_flags.has_any ( SF_SEPARATE ) ) {
		for ( unsigned int ii=0; ii < num_switches(); ++ii ) {
			::Wdg2::GW_Switch * gw_switch ( this->create_switch_single ( ii ) );
			gw_switch->setParentItem ( this );
			_switches.append ( gw_switch );
		}
	} else if ( _state_flags.has_any ( SF_JOINED ) ) {
		_switch_multi = this->create_switch_multi();
		_switch_multi->setParentItem ( this );
	}
}


bool
GW_Switches_Joinable::has_focus ( )
{
	bool res ( false );
	if ( _switch_multi != 0 ) {
		res = _switch_multi->hasFocus();
	} else if ( _switches.size() > 0 ) {
		for ( int ii=0; ii < _switches.size(); ++ii ) {
			if ( _switches[ii]->hasFocus() ) {
				res = true;
				break;
			}
		}
	}
	return res;
}

void
GW_Switches_Joinable::set_focus (
	Qt::FocusReason focusReason_n )
{
	if ( _switch_multi != 0 ) {
		_switch_multi->setFocus ( focusReason_n );
	} else if ( _switches.size() > 0 ) {
		_switches[0]->setFocus ( focusReason_n );
	}
}

bool
GW_Switches_Joinable::sceneEventFilter (
	QGraphicsItem * watched_n,
	QEvent * event_n )
{
	(void) watched_n;
	bool res ( false );
	if ( event_n->type() == QEvent::KeyPress ) {
		QKeyEvent * kev ( static_cast < QKeyEvent * > ( event_n ) );
		if ( scene_db()->inputs_db()->action_key_match ( scene_db()->ikid_joinable_toggle_joined, kev->key() ) ) {
			toggle_joined();
			res = true;
		}
	}
	return res;
}


} // End of namespace
