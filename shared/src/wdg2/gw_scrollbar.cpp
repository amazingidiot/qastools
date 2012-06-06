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
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n )
{
}

QRectF
GW_Scrollbar_Button::boundingRect ( ) const
{
	return QRectF ( QPointF ( 0.0, 0.0 ), QSizeF ( _size ) );
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
	const QSizeF & size_n )
{
	if ( _size != size_n ) {
		prepareGeometryChange();
		_size = size_n;
	}
}

Qt::Orientation
GW_Scrollbar_Button::orientation ( ) const
{
	::Wdg2::GW_Scrollbar & sbar (
		*static_cast < ::Wdg2::GW_Scrollbar * > ( parentItem() ) );
	return sbar.orientation();
}



GW_Scrollbar_Rail::GW_Scrollbar_Rail (
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n )
{
}

QRectF
GW_Scrollbar_Rail::boundingRect ( ) const
{
	return QRectF ( QPointF ( 0.0, 0.0 ), _size );
}

void
GW_Scrollbar_Rail::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;

	QRectF prect ( QPointF ( 0.0, 0.0 ), _size );

	double pen_width ( 1.0 );
	double pwhalf ( pen_width / 2.0 );
	double delta_x;
	double delta_y;
	if ( orientation() == Qt::Horizontal ) {
		delta_x = pwhalf;
		delta_y = ::std::floor ( _size.height() / 3 ) + pwhalf;
	} else {
		delta_x = ::std::floor ( _size.width() / 3 ) + pwhalf;
		delta_y = pwhalf;
	}
	prect.adjust ( delta_x, delta_y, -delta_x, -delta_y );

	{
		QPen ppen ( option_n->palette.color ( QPalette::ButtonText ) );
		ppen.setWidth ( pen_width );
		painter_n->setPen ( ppen );
	}
	painter_n->setBrush ( option_n->palette.color ( QPalette::Button ) );
	painter_n->drawRect ( prect );
}

void
GW_Scrollbar_Rail::set_size (
	const QSizeF & size_n )
{
	if ( _size != size_n ) {
		prepareGeometryChange();
		_size = size_n;
	}
}

Qt::Orientation
GW_Scrollbar_Rail::orientation ( ) const
{
	::Wdg2::GW_Scrollbar & sbar (
		*static_cast < ::Wdg2::GW_Scrollbar * > ( parentItem() ) );
	return sbar.orientation();
}




GW_Scrollbar_Handle::GW_Scrollbar_Handle (
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n )
{
}

QRectF
GW_Scrollbar_Handle::boundingRect ( ) const
{
	return QRectF ( QPointF ( 0.0, 0.0 ), _size );
}

void
GW_Scrollbar_Handle::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;

	double pen_width ( 1.0 );
	if ( state_flags().has_any ( ::Wdg2::GW_IS_GRABBED ) ) {
		pen_width = 1.5;
	}
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
GW_Scrollbar_Handle::set_size (
	const QSizeF & size_n )
{
	if ( _size != size_n ) {
		prepareGeometryChange();
		_size = size_n;
	}
}

Qt::Orientation
GW_Scrollbar_Handle::orientation ( ) const
{
	::Wdg2::GW_Scrollbar & sbar (
		*static_cast < ::Wdg2::GW_Scrollbar * > ( parentItem() ) );
	return sbar.orientation();
}



GW_Scrollbar::GW_Scrollbar (
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_size ( 0.0, 0.0 ),
_orientation ( Qt::Horizontal ),
_int_span ( 0 ),
_int_value ( 0 ),
_rail_start ( 0 ),
_handle_pos ( 0 ),
_handle_pos_span ( 0 ),
_handle_len ( 0 ),
_btn_low ( this ),
_btn_high ( this ),
_rail ( this ),
_handle ( this )
{
	setFlags ( QGraphicsItem::ItemHasNoContents );
}

GW_Scrollbar::~GW_Scrollbar ( )
{
}

QRectF
GW_Scrollbar::boundingRect ( ) const
{
	return QRectF ( QPointF ( 0.0, 0.0 ), _size );
}

void
GW_Scrollbar::paint (
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
GW_Scrollbar::set_size (
	const QSizeF & size_n )
{
	if ( _size != size_n ) {
		prepareGeometryChange();
		_size = size_n;
		update_geometries();
	}
}

void
GW_Scrollbar::set_orientation (
	Qt::Orientation orientation_n )
{
	if ( _orientation != orientation_n ) {
		_orientation = _orientation;
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
		if ( _int_value > _int_span ) {
			_int_value = _int_span;
		}
		_value_map.set_value_range ( 0, _int_span );
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
		update_handle_pos_from_value();
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
	QPointF rail_pos;
	QSizeF btn_size;
	QSizeF rail_size;
	QSizeF handle_size;

	{
		unsigned int len_total;
		unsigned int width_total;
		unsigned int len_btn;
		unsigned int len_rail;
		unsigned int len_handle;

		if ( _orientation == Qt::Horizontal ) {
			len_total = _size.width();
			width_total = _size.height();
		} else {
			len_total = _size.height();
			width_total = _size.width();
		}

		len_btn = ::std::floor ( len_total / 20 );
		unsigned int len_btn_max ( 5*width_total/2 );
		if ( len_btn > len_btn_max ) {
			len_btn = len_btn_max;
		}
		if ( len_btn < width_total ) {
			len_btn = width_total;
		}

		len_rail = len_total - 2*len_btn;
		len_handle = len_btn;
		if ( _int_span < len_rail ) {
			unsigned int len ( len_rail - _int_span );
			if ( len > len_handle ) {
				len_handle = len;
			}
		}

		// Set sizes
		if ( _orientation == Qt::Horizontal ) {
			btn_pos_low = QPointF ( 0.0, 0.0 );
			btn_pos_high = QPointF ( len_total - len_btn, 0.0 );
			rail_pos = QPointF ( len_btn, 0.0 );
			btn_size = QSize ( len_btn, width_total );
			rail_size = QSize ( len_rail, width_total );
			handle_size = QSize ( len_handle, width_total );
		} else {
			btn_pos_low = QPointF ( 0.0, len_total - len_btn );
			btn_pos_high = QPointF ( 0.0, 0.0 );
			rail_pos = QPointF ( 0.0, len_btn );
			btn_size = QSize ( width_total, len_btn );
			rail_size = QSize ( width_total, len_rail );
			handle_size = QSize ( width_total, len_handle );
		}

		_rail_start = len_btn;
		_handle_pos = 0;
		_handle_pos_span = len_rail - len_handle;
		_handle_len = len_handle;
		_value_map.set_px_span ( _handle_pos_span );
	}

	_btn_low.setPos ( btn_pos_low );
	_btn_low.set_size ( btn_size );
	_btn_high.setPos ( btn_pos_high );
	_btn_high.set_size ( btn_size );
	_rail.setPos ( rail_pos );
	_rail.set_size ( rail_size );
	_handle.setPos ( rail_pos );
	_handle.set_size ( handle_size );

	update_handle_pos_from_value();
}

void
GW_Scrollbar::update_handle_pos_from_value ( )
{
	set_handle_pos ( _value_map.px_from_value ( _int_value ) );
}

void
GW_Scrollbar::update_value_from_handle_pos ( )
{
	long val ( _value_map.value_from_px ( _handle_pos ) );
	if ( val != _int_value ) {
		_int_value = val;
		_val_change_cb.call_if_valid();
	}
}

void
GW_Scrollbar::set_handle_pos (
	unsigned int pos_n )
{
	if ( pos_n <= _handle_pos_span ) {
		if ( pos_n != _handle_pos ) {
			_handle_pos = pos_n;
			const double hlpos ( _rail_start + _handle_pos );
			QPointF hpos;
			if ( _orientation == Qt::Horizontal ) {
				hpos = QPointF ( hlpos, 0.0 );
			} else {
				hpos = QPointF ( 0.0, hlpos );
			}
			_handle.setPos ( hpos );
		}
	}
}

bool
GW_Scrollbar::point_in_handle (
	const QPointF & point_n ) const
{
	int plen;
	int pwidth;
	if ( orientation() == Qt::Horizontal ) {
		 plen = point_n.x();
		 pwidth = point_n.y();
	} else {
		 plen = _size.height() - point_n.y();
		 pwidth = point_n.y();
	}
	plen -= (int)_rail_start;
	plen -= (int)_handle_pos;

	const bool res (
		( plen >= 0 ) &&
		( pwidth >= 0 ) &&
		( plen < (int)_handle_len ) &&
		( pwidth < (int)_size.height() ) );
	return res;
}

void
GW_Scrollbar::move_handle (
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
GW_Scrollbar::mousePressEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	if ( point_in_handle ( event_n->pos() ) ) {
		_handle.state_flags().set ( ::Wdg2::GW_IS_GRABBED );
		_handle.update();
	}
}

void
GW_Scrollbar::mouseReleaseEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	if ( _handle.state_flags().has_any ( ::Wdg2::GW_IS_GRABBED ) ) {
		_handle.state_flags().unset ( ::Wdg2::GW_IS_GRABBED );
		_handle.update();
	}
}

void
GW_Scrollbar::mouseMoveEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	if ( _handle.state_flags().has_any ( ::Wdg2::GW_IS_GRABBED ) ) {
		int delta;
		{
			double deltaf;
			if ( orientation() == Qt::Horizontal ) {
				deltaf =  event_n->pos().x() - event_n->lastPos().x();
			} else {
				deltaf = event_n->lastPos().y() - event_n->pos().y();
			}
			delta = ::std::floor ( deltaf + 0.5 );
		}
		move_handle ( delta );
	}
}

void
GW_Scrollbar::wheelEvent (
	QGraphicsSceneWheelEvent * event_n )
{
	::std::cout << "GW_Scrollbar::wheelEvent" << "\n";
}


} // End of namespace
