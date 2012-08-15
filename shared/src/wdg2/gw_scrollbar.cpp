//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_scrollbar.hpp"
#include "scene_database.hpp"
#include "theme_painters.hpp"
#include <limits.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

namespace Wdg2
{


GW_Scrollbar_Button::GW_Scrollbar_Button (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Pixmaps ( scene_db_n, 2, parent_n )
{
	set_pxm_type_part (
		::Wdg2::WGT_SCROLLBAR,
		::Wdg2::WGP_SCROLLBAR_BTN_LEFT );
}

GW_Scrollbar_Button::~GW_Scrollbar_Button ( )
{
}

inline
::Wdg2::GW_Scrollbar *
GW_Scrollbar_Button::scrollbar ( ) const
{
	return static_cast < ::Wdg2::GW_Scrollbar * > ( parentItem() );
}

void
GW_Scrollbar_Button::update_pxm_idx ( )
{
	unsigned int idx ( 0 );
	if ( state_flags().has_any ( ::Wdg2::GW_IS_GRABBED ) ) {
		idx = 1;
	}
	set_pxm_idx ( idx );
}

bool
GW_Scrollbar_Button::setup_request (
	unsigned int idx_n,
	::dpe2::Key_Values & kvals_n )
{
	bool res ( ::Wdg2::GW_Pixmaps::setup_request ( idx_n, kvals_n ) );
	if ( res ) {
		::Flags sflags;
		if ( idx_n == 1 ) {
			sflags.set ( ::Wdg2::GW_IS_GRABBED );
		}
		kvals_n.set_uint ( ::Wdg2::PRK_WIDGET_STATE_FLAGS, sflags.flags() );
	}
	return res;
}

void
GW_Scrollbar_Button::mousePressEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	set_state_flags ( ::Wdg2::GW_IS_GRABBED, true );
	const bool forward = (
		( pxm_keys().wdg_part == ::Wdg2::WGP_SCROLLBAR_BTN_RIGHT ) ||
		( pxm_keys().wdg_part == ::Wdg2::WGP_SCROLLBAR_BTN_TOP ) );
	scrollbar()->begin_move ( forward );
}

void
GW_Scrollbar_Button::mouseReleaseEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	set_state_flags ( ::Wdg2::GW_IS_GRABBED, false );
	const bool forward = (
		( pxm_keys().wdg_part == ::Wdg2::WGP_SCROLLBAR_BTN_RIGHT ) ||
		( pxm_keys().wdg_part == ::Wdg2::WGP_SCROLLBAR_BTN_TOP ) );
	scrollbar()->end_move ( forward );
}

void
GW_Scrollbar_Button::wheelEvent (
	QGraphicsSceneWheelEvent * event_n )
{

}



GW_Scrollbar::GW_Scrollbar (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget ( scene_db_n, parent_n ),
_size ( 0.0, 0.0 ),
_orientation ( Qt::Horizontal ),
_int_span ( 0 ),
_int_value ( 0 ),
_btn_low ( scene_db(), this ),
_btn_high ( scene_db(), this ),
_slider ( scene_db(), this )
{
	setFlags ( QGraphicsItem::ItemHasNoContents );

	_anim_timer_id = scene_db()->timer_server()->acquire_callback (
		scene_db()->tid_animation );
	scene_db()->timer_server()->cback_set_callback (
		_anim_timer_id,
		::Context_Callback_UInt ( this, &move_animation_cb ) );
	_anim_forward = 0;
	_anim_backward = 0;
	_anim_micro_step = 0;
	_anim_speed = 400.0; // units / per sec

	_slider.set_value_map ( &_value_map );
	_slider.set_orientation ( _orientation );
	_slider.rail().set_pxm_type_part (
		::Wdg2::WGT_SCROLLBAR,
		::Wdg2::WGP_SCROLLBAR_RAIL );
	_slider.handle().set_pxm_type_part (
		::Wdg2::WGT_SCROLLBAR,
		::Wdg2::WGP_SCROLLBAR_HANDLE );

	_slider.set_val_change_callback (
		::Context_Callback ( this, ::Wdg2::GW_Scrollbar::read_slider_value_cb ) );
}

GW_Scrollbar::~GW_Scrollbar ( )
{
	scene_db()->timer_server()->release_callback (
		_anim_timer_id );
}

void
GW_Scrollbar::set_size (
	const QSize & size_n )
{
	if ( _size != size_n ) {
		_size = size_n;
		set_bounding_rect ( _size );
		update_geometries();
	}
}

void
GW_Scrollbar::set_orientation (
	Qt::Orientation orientation_n )
{
	if ( _orientation != orientation_n ) {
		_orientation = _orientation;
		_slider.set_orientation ( _orientation );
		update_geometries();
	}
}

void
GW_Scrollbar::set_int_span (
	unsigned int span_n )
{
	if ( span_n != _int_span ) {
		_int_span = span_n;
		if ( _int_span > INT_MAX ) {
			_int_span = INT_MAX;
		}
		_value_map.set_value_range ( 0, _int_span );
		_slider.set_value_map ( &_value_map );

		if ( _int_value > _int_span ) {
			_int_value = _int_span;
			_slider.set_int_value ( _int_value );
		}
		update_geometries();
	}
}

void
GW_Scrollbar::set_int_value (
	unsigned int value_n )
{
	if ( value_n > _int_span ) {
		value_n = _int_span;
	}
	if ( value_n != _int_value ) {
		_int_value = value_n;
		_slider.set_int_value ( _int_value );
		_val_change_cb.call_if_valid();
	}
}

void
GW_Scrollbar::set_val_change_callback (
	const ::Context_Callback & cb_n )
{
	_val_change_cb = cb_n;
}

void
GW_Scrollbar::update_geometries ( )
{
	unsigned char btn_low_part;
	unsigned char btn_high_part;
	QPointF btn_pos_low;
	QPointF btn_pos_high;
	QPointF slider_pos;
	QSize btn_size;
	::Wdg2::GW_Slider_Sizes sl_sizes;

	{
		unsigned int len_total;
		unsigned int width_total;
		unsigned int len_btn;
		unsigned int len_slider;
		unsigned int len_handle;

		if ( _orientation == Qt::Horizontal ) {
			len_total = _size.width();
			width_total = _size.height();
		} else {
			len_total = _size.height();
			width_total = _size.width();
		}

		len_btn = ( 5*width_total ) / 3;
		len_slider = len_total - 2*len_btn;
		len_handle = len_btn;
		if ( _int_span < len_slider ) {
			unsigned int hlen ( len_slider - _int_span );
			if ( hlen > len_handle ) {
				len_handle = hlen;
			}
		}

		// Set sizes
		if ( _orientation == Qt::Horizontal ) {
			btn_low_part = ::Wdg2::WGP_SCROLLBAR_BTN_LEFT;
			btn_high_part = ::Wdg2::WGP_SCROLLBAR_BTN_RIGHT;
			btn_pos_low  = QPointF ( 0.0, 0.0 );
			btn_pos_high = QPointF ( len_total - len_btn, 0.0 );
			slider_pos   = QPointF ( len_btn, 0.0 );
			btn_size    = QSize ( len_btn, width_total );
			sl_sizes.size = QSize ( len_slider, width_total );
			sl_sizes.handle_length = len_handle;
		} else {
			btn_low_part = ::Wdg2::WGP_SCROLLBAR_BTN_BOTTOM;
			btn_high_part = ::Wdg2::WGP_SCROLLBAR_BTN_TOP;
			btn_pos_low  = QPointF ( 0.0, len_total - len_btn );
			btn_pos_high = QPointF ( 0.0, 0.0 );
			slider_pos   = QPointF ( 0.0, len_btn );
			btn_size    = QSize ( width_total, len_btn );
			sl_sizes.size = QSize ( width_total, len_slider );
			sl_sizes.handle_length = len_handle;
		}
	}

	_btn_low.set_pxm_type_part ( ::Wdg2::WGT_SCROLLBAR, btn_low_part );
	_btn_low.set_pxm_size ( btn_size );
	_btn_low.setPos ( btn_pos_low );

	_btn_high.set_pxm_type_part ( ::Wdg2::WGT_SCROLLBAR, btn_high_part );
	_btn_high.set_pxm_size ( btn_size );
	_btn_high.setPos ( btn_pos_high );

	_slider.setPos ( slider_pos );
	_slider.set_sizes ( sl_sizes );
}

void
GW_Scrollbar::read_slider_value ( )
{
	const long sval ( _slider.int_value() );
	if ( sval != _int_value ) {
		_int_value = sval;
		_val_change_cb.call_if_valid();
	}
}

void
GW_Scrollbar::read_slider_value_cb (
	void * context_n )
{
	::Wdg2::GW_Scrollbar * sbar (
		reinterpret_cast < ::Wdg2::GW_Scrollbar * > ( context_n ) );
	sbar->read_slider_value();
}

void
GW_Scrollbar::set_anim_speed (
	float speed_n )
{
	_anim_speed = speed_n;
}

void
GW_Scrollbar::begin_move (
	bool forward_n )
{
	if ( forward_n ) {
		++_anim_forward;
	} else {
		++_anim_backward;
	}

	animation_update();
}

void
GW_Scrollbar::end_move (
	bool forward_n )
{
	if ( forward_n ) {
		--_anim_forward;
	} else {
		--_anim_backward;
	}

	animation_update();
}

void
GW_Scrollbar::animation_start ( )
{
	::Wdg2::Timer_Server * tserv ( scene_db()->timer_server() );
	if ( !tserv->cback_is_running ( _anim_timer_id ) ) {
		tserv->cback_request_interval ( _anim_timer_id );
	}
}

void
GW_Scrollbar::animation_stop ( )
{
	::Wdg2::Timer_Server * tserv ( scene_db()->timer_server() );
	tserv->cback_abort_request ( _anim_timer_id );
	_anim_micro_step = 0;
}

void
GW_Scrollbar::animation_update ( )
{
	::Wdg2::Timer_Server & tserv ( *scene_db()->timer_server() );
	const int speed ( (int)_anim_forward - (int)_anim_backward );
	if ( speed != 0 ) {
		if ( !tserv.cback_is_running ( _anim_timer_id ) ) {
			tserv.cback_request_interval ( _anim_timer_id );
			_anim_micro_step = 0;
		}
	} else {
		tserv.cback_abort_request ( _anim_timer_id );
	}
}

void
GW_Scrollbar::move_animation_cb (
	void * context_n,
	unsigned int msec_n )
{
	::Wdg2::GW_Scrollbar * cref (
		reinterpret_cast < ::Wdg2::GW_Scrollbar * > ( context_n ) );
	cref->move_animation ( msec_n );
}

void
GW_Scrollbar::move_animation (
	unsigned int msec_n )
{
	int steps;
	{
		double fspeed ( double ( _anim_speed ) / 1000.0 );
		fspeed *= double ( (int)_anim_forward - (int)_anim_backward );
		fspeed *= double ( msec_n );

		steps = fspeed;

		const int mega ( 1000*1000 );
		fspeed -= double ( steps );
		fspeed *= double ( mega );
		int usteps = fspeed;
		usteps += (int)_anim_micro_step;
		{
			const int mstep ( usteps / mega );
			if ( mstep != 0 ) {
				usteps -= ( mstep * mega );
				steps += mstep;
			}
		}
		_anim_micro_step = usteps;
	}

	if ( steps != 0 ) {
		unsigned int vsteps;
		if ( steps > 0 ) {
			vsteps = steps;
			unsigned int vsteps_max ( int_span() - int_value() );
			if ( vsteps > vsteps_max ) {
				vsteps = vsteps_max;
			}
			vsteps += int_value();
		} else {
			vsteps = -steps;
			if ( vsteps > int_value() ) {
				vsteps = int_value();
			}
			vsteps = int_value() - vsteps;
		}
		set_int_value ( vsteps );
	}
}


} // End of namespace
