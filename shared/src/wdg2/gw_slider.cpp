//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_slider.hpp"
#include "theme_painters.hpp"
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <cmath>
#include <iostream>

namespace Wdg2
{


GW_Slider::GW_Slider (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget ( scene_db_n, parent_n )
{
	init();
}

GW_Slider::GW_Slider (
	::Wdg2::Scene_Database * scene_db_n,
	::Wdg2::GW_Widget_Element * rail_n,
	::Wdg2::GW_Widget_Element * handle_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget ( scene_db_n, parent_n )
{
	init ( rail_n, handle_n );
}

void
GW_Slider::init (
	::Wdg2::GW_Widget_Element * rail_n,
	::Wdg2::GW_Widget_Element * handle_n )
{
	setFlags ( QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemHasNoContents );

	_rail_span = 0;
	_handle_pos = 0;
	_handle_pos_span = 0;
	_int_value = 0;
	_value_map = 0;

	if ( rail_n == 0 ) {
		rail_n = new ::Wdg2::GW_Slider_Rail ( scene_db(), this );
	} else {
		rail_n->gw_widget()->setParentItem ( this );
	}
	_rail.reset ( rail_n );

	if ( handle_n == 0 ) {
		handle_n = new ::Wdg2::GW_Slider_Handle ( scene_db(), this );
	} else {
		handle_n->gw_widget()->setParentItem ( this );
	}
	_handle.reset ( handle_n );
}

GW_Slider::~GW_Slider ( )
{
}

void
GW_Slider::load_settings (
	const ::Wdg2::GW_Slider_Settings & settings_n )
{
	_settings = settings_n;
	set_bounding_rect ( _settings.size );
	update_geometries();
}

void
GW_Slider::set_orientation (
	Qt::Orientation orientation_n )
{
	::Wdg2::GW_Slider_Settings sl_settings ( settings() );
	if ( sl_settings.orientation != orientation_n ) {
		sl_settings.orientation = orientation_n;
		load_settings ( sl_settings );
	}
}

void
GW_Slider::set_value_map (
	::Wdg2::Slider_Value_Map * map_n )
{
	if ( _value_map != map_n ) {
		_value_map = map_n;
		update_geometries();
	}
}

void
GW_Slider::set_val_change_callback (
	const ::Context_Callback & cb_n )
{
	_val_change_cb = cb_n;
}

void
GW_Slider::update_geometries ( )
{
	QSize rail_size ( _settings.size );
	QSize handle_size ( 0, 0 );

	unsigned int rail_length;
	unsigned int handle_length ( _settings.handle_length );

	if ( orientation() == Qt::Horizontal ) {
		rail_length = _settings.size.width();
		if ( handle_length > rail_length ) {
			handle_length = rail_length / 2;
		}
		handle_size = QSize ( handle_length, _settings.size.height() );
	} else {
		rail_length = _settings.size.height();
		if ( handle_length > rail_length ) {
			handle_length = rail_length / 2;
		}
		handle_size = QSize ( _settings.size.width(), handle_length );
	}

	// update ranges
	_rail_span = 0;
	if ( rail_length > 0 ) {
		_rail_span = rail_length - 1;
	}
	_handle_pos_span = ( rail_length - handle_length );
	if ( value_map() != 0 ) {
		value_map()->set_px_span ( _handle_pos_span );
	}

	// resize widgets
	_rail->set_size ( rail_size );
	_handle->set_size ( handle_size );

	// update handle position
	_handle_pos = ( _handle_pos_span + 1 ); // Invalid value to enforce update
	update_handle_pos_from_value();
}

void
GW_Slider::set_int_value (
	long value_n )
{
	if ( value_map() != 0 ) {
		if ( value_n < value_map()->value_min() ) {
			value_n = value_map()->value_min();
		}
		if ( value_n > value_map()->value_max() ) {
			value_n = value_map()->value_max();
		}
		if ( value_n != _int_value ) {
			_int_value = value_n;
			update_handle_pos_from_value();
		}
	}
}

void
GW_Slider::update_handle_pos_from_value ( )
{
	if ( value_map() != 0 ) {
		if ( !_handle->state_flags().has_any ( ::Wdg2::GW_IS_GRABBED ) ) {
			set_handle_pos ( value_map()->px_from_value ( _int_value ) );
		}
	}
}

void
GW_Slider::update_value_from_handle_pos ( )
{
	if ( value_map() != 0 ) {
		const long cval ( value_map()->value_from_px ( _handle_pos ) );
		if ( _int_value != cval ) {
			_int_value = cval;
			_val_change_cb.call_if_valid();
		}
	}
}

void
GW_Slider::set_handle_pos (
	unsigned int pos_n )
{
	if ( pos_n > _handle_pos_span ) {
		pos_n = _handle_pos_span;
	}
	if ( _handle_pos != pos_n ) {
		_handle_pos = pos_n;
		{
			QPoint pos ( 0, 0 );
			if ( orientation() == Qt::Horizontal ) {
				pos.setX ( _handle_pos );
			} else {
				pos.setY ( _handle_pos_span - _handle_pos );
			}
			_handle->set_pos ( pos );
		}
	}
}

bool
GW_Slider::point_in_handle (
	const QPointF & point_n ) const
{
	int plen;
	int pwidth;
	int hwidth;
	if ( orientation() == Qt::Horizontal ) {
		plen   = point_n.x();
		pwidth = point_n.y();
		hwidth = _settings.size.height();
	} else {
		plen   = (int)_rail_span - (int)point_n.y();
		pwidth = point_n.x();
		hwidth = _settings.size.width();
	}
	plen -= _handle_pos;

	const bool res =
		( plen >= 0 ) &&
		( pwidth >= 0 ) &&
		( plen < (int)_settings.handle_length ) &&
		( pwidth < hwidth );

	return res;
}

void
GW_Slider::move_handle (
	int amount_n )
{
	if ( amount_n != 0 ) {
		const int amount_min ( (-(int)_handle_pos) );
		const int amount_max ( _handle_pos_span - _handle_pos );
		if ( amount_n < amount_min ) {
			amount_n = amount_min;
		}
		if ( amount_n > amount_max ) {
			amount_n = amount_max;
		}
		set_handle_pos ( _handle_pos + amount_n );
		update_value_from_handle_pos();
	}
}

void
GW_Slider::focusInEvent (
	QFocusEvent * event_n )
{
	(void) event_n;
	//::std::cout << "GW_Slider::focusInEvent"  << "\n";
	_rail->set_state_flags ( ::Wdg2::GW_HAS_FOCUS );
	_handle->set_state_flags ( ::Wdg2::GW_HAS_FOCUS );
}

void
GW_Slider::focusOutEvent (
	QFocusEvent * event_n )
{
	(void) event_n;
	//::std::cout << "GW_Slider::focusOutEvent"  << "\n";
	_rail->unset_state_flags ( ::Wdg2::GW_HAS_FOCUS );
	_handle->unset_state_flags ( ::Wdg2::GW_HAS_FOCUS );
}

void
GW_Slider::mousePressEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	//::std::cout << "GW_Slider::mousePressEvent"  << "\n";
	if ( point_in_handle ( event_n->pos() ) ) {
		_handle->set_state_flags ( ::Wdg2::GW_IS_GRABBED );
	}
}

void
GW_Slider::mouseReleaseEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	(void) event_n;
	//::std::cout << "GW_Slider::mouseReleaseEvent"  << "\n";
	if ( _handle->state_flags().has_any ( ::Wdg2::GW_IS_GRABBED ) ) {
		_handle->unset_state_flags ( ::Wdg2::GW_IS_GRABBED );
		update_handle_pos_from_value();
	}
}

void
GW_Slider::mouseMoveEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	//::std::cout << "GW_Slider::mouseMoveEvent" << "\n";
	if ( _handle->state_flags().has_any ( ::Wdg2::GW_IS_GRABBED ) ) {
		int delta;
		{
			int lfrom;
			int lto;
			if ( orientation() == Qt::Horizontal ) {
				lfrom = event_n->lastPos().x();
				lto   = event_n->pos().x();
			} else {
				const double lheight ( _rail_span );
				lfrom = lheight - event_n->lastPos().y();
				lto   = lheight - event_n->pos().y();
			}

			// Dead zones at and beyond the rail ends
			// for a movement direction towards the rail center
			int lim ( _settings.handle_length / 2 );
			if ( lto > lfrom ) {
				if ( lfrom < lim ) {
					lfrom = lim;
				}
				if ( lto < lim ) {
					lto = lim;
				}
			} else {
				lim = ( _rail_span - lim );
				if ( lfrom > lim ) {
					lfrom = lim;
				}
				if ( lto > lim ) {
					lto = lim;
				}
			}
			delta = ( lto - lfrom );
		}
		move_handle ( delta );
	}
}

void
GW_Slider::wheelEvent (
	QGraphicsSceneWheelEvent * event_n )
{
	// TODO
	(void) event_n;
	::std::cout << "GW_Slider::wheelEvent" << "\n";
}



GW_Slider_Rail::GW_Slider_Rail (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget_Element_Pixmaps ( scene_db_n, 2, parent_n )
{
	gw_pixmaps()->pxm_kvals().set_uint ( ::Wdg2::PRK_WIDGET_TYPE, ::Wdg2::WGT_SLIDER );
	gw_pixmaps()->pxm_kvals().set_uint ( ::Wdg2::PRK_WIDGET_PART, ::Wdg2::WGP_SLIDER_RAIL );
}

void
GW_Slider_Rail::state_flags_changed ( )
{
	unsigned int idx ( 0 );
	if ( state_flags().has_any ( ::Wdg2::GW_HAS_FOCUS ) ) {
		idx = 1;
	}
	gw_pixmaps()->set_pxm_idx ( idx );
}



GW_Slider_Handle::GW_Slider_Handle (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget_Element_Pixmaps ( scene_db_n, 2, parent_n )
{
	gw_pixmaps()->pxm_kvals().set_uint ( ::Wdg2::PRK_WIDGET_TYPE, ::Wdg2::WGT_SLIDER );
	gw_pixmaps()->pxm_kvals().set_uint ( ::Wdg2::PRK_WIDGET_PART, ::Wdg2::WGP_SLIDER_HANDLE );
}

void
GW_Slider_Handle::state_flags_changed ( )
{
	unsigned int idx ( 0 );
	if ( state_flags().has_any ( ::Wdg2::GW_HAS_FOCUS ) ) {
		idx = 1;
	}
	gw_pixmaps()->set_pxm_idx ( idx );
}


} // End of namespace
