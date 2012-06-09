//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_scrollbar.hpp"
#include "limits.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

namespace Wdg2
{


GW_Scrollbar_Button::GW_Scrollbar_Button (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget ( scene_db_n, parent_n )
{
}

void
GW_Scrollbar_Button::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;

	double pen_width ( 1.0 );
	double pwhalf ( pen_width / 2.0 );
	QRectF prect ( QPointF ( 0.0, 0.0 ), _size );
	prect.adjust ( pwhalf, pwhalf, -pwhalf, -pwhalf );
	{
		QPen ppen ( option_n->palette.color ( QPalette::ButtonText ) );
		ppen.setWidth ( pen_width );
		painter_n->setPen ( ppen );
	}
	painter_n->setBrush ( option_n->palette.color ( QPalette::Button ) );
	painter_n->drawRoundedRect ( prect, 2.0, 2.0 );
}

void
GW_Scrollbar_Button::set_size (
	const QSize & size_n )
{
	if ( _size != size_n ) {
		_size = size_n;
		// Bounding rect
		{
			QRectF brect ( QPointF ( 0.0, 0.0 ), QSizeF ( _size ) );
			set_bounding_rect ( brect );
		}
	}
}

Qt::Orientation
GW_Scrollbar_Button::orientation ( ) const
{
	::Wdg2::GW_Scrollbar & sbar (
		*static_cast < ::Wdg2::GW_Scrollbar * > ( parentItem() ) );
	return sbar.orientation();
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
	_slider.set_value_map ( &_value_map );
	_slider.set_orientation ( _orientation );

	_slider.set_val_change_callback (
		::Context_Callback ( this, ::Wdg2::GW_Scrollbar::read_slider_value_cb ) );
}

GW_Scrollbar::~GW_Scrollbar ( )
{
}

void
GW_Scrollbar::set_size (
	const QSize & size_n )
{
	if ( _size != size_n ) {
		_size = size_n;
		// Bounding rect
		{
			QRectF brect ( QPointF ( 0.0, 0.0 ), QSizeF ( _size ) );
			set_bounding_rect ( brect );
		}
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
			btn_pos_low  = QPointF ( 0.0, 0.0 );
			btn_pos_high = QPointF ( len_total - len_btn, 0.0 );
			slider_pos   = QPointF ( len_btn, 0.0 );
			btn_size    = QSize ( len_btn, width_total );
			sl_sizes.size = QSize ( len_slider, width_total );
			sl_sizes.handle_length = len_handle;
		} else {
			btn_pos_low  = QPointF ( 0.0, len_total - len_btn );
			btn_pos_high = QPointF ( 0.0, 0.0 );
			slider_pos   = QPointF ( 0.0, len_btn );
			btn_size    = QSize ( width_total, len_btn );
			sl_sizes.size = QSize ( width_total, len_slider );
			sl_sizes.handle_length = len_handle;
		}
	}

	_btn_low.setPos ( btn_pos_low );
	_btn_low.set_size ( btn_size );
	_btn_high.setPos ( btn_pos_high );
	_btn_high.set_size ( btn_size );
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


} // End of namespace
