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
#include "wdg2/scene_database.hpp"
#include <QEvent>
#include <QKeyEvent>
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
	setFiltersChildEvents ( true );
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
GW_Sliders_Joinable::load_settings (
	const ::Wdg2::GW_Joinable_Sliders_Settings & settings_n )
{
	_settings = settings_n;
	set_bounding_rect ( QSizeF ( int_width(), _settings.area_height ) );
	update_geometries();
}

void
GW_Sliders_Joinable::update_geometries ( )
{
	if ( _sliders.size() > 0 ) {
		const double delta_x ( _settings.slider_width + _settings.channels_gap );
		QPointF spos ( 0.0, 0.0 );

		::Wdg2::GW_Slider_Settings sl_settings;
		sl_settings.size.setWidth ( _settings.slider_width );
		sl_settings.size.setHeight ( _settings.area_height );
		sl_settings.handle_length = _settings.area_height / 10;

		_value_map.set_px_span ( sl_settings.size.height() - sl_settings.handle_length );

		for ( int ii=0; ii < _sliders.size(); ++ii ) {
			::Wdg2::GW_Slider * slider ( _sliders[ii] );
			slider->load_settings ( sl_settings );
			slider->setPos ( spos );
			spos.rx() += delta_x;
		}
	}
	if ( _slider_multi != 0 ) {
		{
			::Wdg2::GW_Slider_Multi_Settings sl_settings;
			sl_settings.num_sliders = num_sliders();
			sl_settings.area_height = _settings.area_height;
			sl_settings.slider_width = _settings.slider_width;
			sl_settings.channels_gap = _settings.channels_gap;

			_slider_multi->load_settings_multi ( sl_settings );
		}
		_slider_multi->setPos ( QPointF ( 0.0, 0.0 ) );
	}
}

unsigned int
GW_Sliders_Joinable::int_width_probe (
	const ::Wdg2::GW_Joinable_Sliders_Settings & settings_n ) const
{
	unsigned int iwidth ( 0 );
	const unsigned int num ( num_sliders() );
	if ( num > 0 ) {
		iwidth += settings_n.slider_width * num;
		iwidth += settings_n.channels_gap * ( num - 1 );
	}
	return iwidth;
}

unsigned int
GW_Sliders_Joinable::int_width ( ) const
{
	return int_width_probe ( settings() );
}

void
GW_Sliders_Joinable::select_joined (
	bool flag_n )
{
	if ( num_sliders() > 0 ) {
		unsigned int flag_set ( SF_JOINED );
		unsigned int flag_unset ( SF_SEPARATE );
		if ( !flag_n ) {
			::std::swap ( flag_set, flag_unset );
		}
		if ( !_state_flags.has_any ( flag_set ) ) {
			const bool had_focus ( has_focus() );
			destroy_sliders();
			_state_flags.unset ( flag_unset );
			_state_flags.set ( flag_set );
			init_sliders();
			update_geometries();
			if ( had_focus ) {
				set_focus();
			}
		}
	}
}

void
GW_Sliders_Joinable::select_joined ( )
{
	select_joined ( true );
}

void
GW_Sliders_Joinable::select_separate ( )
{
	select_joined ( false );
}

void
GW_Sliders_Joinable::toggle_joined ( )
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
GW_Sliders_Joinable::init_sliders ( )
{
	if ( _state_flags.has_any ( SF_SEPARATE ) ) {
		for ( unsigned int ii=0; ii < num_sliders(); ++ii ) {
			::Wdg2::GW_Slider * slider ( this->create_single_slider ( ii ) );
			slider->setParentItem ( this );
			_sliders.append ( slider );
		}
	} else if ( _state_flags.has_any ( SF_JOINED ) ) {
		_slider_multi = this->create_multi_slider();
		_slider_multi->setParentItem ( this );
	}
}

bool
GW_Sliders_Joinable::has_focus ( )
{
	bool res ( false );
	if ( _slider_multi != 0 ) {
		res = _slider_multi->hasFocus();
	} else if ( _sliders.size() > 0 ) {
		for ( int ii=0; ii < _sliders.size(); ++ii ) {
			if ( _sliders[ii]->hasFocus() ) {
				res = true;
				break;
			}
		}
	}
	return res;
}

void
GW_Sliders_Joinable::set_focus (
	Qt::FocusReason focusReason_n )
{
	if ( _slider_multi != 0 ) {
		_slider_multi->setFocus ( focusReason_n );
	} else if ( _sliders.size() > 0 ) {
		_sliders[0]->setFocus ( focusReason_n );
	}
}

bool
GW_Sliders_Joinable::sceneEventFilter (
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
