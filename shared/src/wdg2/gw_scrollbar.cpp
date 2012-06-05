//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_scrollbar.hpp"
#include <iostream>
#include <cmath>
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
	const QSize & size_n )
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
	return QRectF ( QPointF ( 0.0, 0.0 ), QSizeF ( _size ) );
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
		delta_y = _size.height() / 3 + pwhalf;
	} else {
		delta_x = _size.width() / 3 + pwhalf;
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
	const QSize & size_n )
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
	return QRectF ( QPointF ( 0.0, 0.0 ), QSizeF ( _size ) );
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
	const QSize & size_n )
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
_size ( 0, 0 ),
_orientation ( Qt::Horizontal ),
_int_span ( 0 ),
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
	return QRectF ( QPointF ( 0.0, 0.0 ), QSizeF ( _size ) );
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
	const QSize & size_n )
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
		update_geometries();
	}
}

void
GW_Scrollbar::update_geometries ( )
{
	QPointF btn_pos_low;
	QPointF btn_pos_high;
	QPointF rail_pos;
	QSize btn_size;
	QSize rail_size;
	QSize handle_size;

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
	}

	_btn_low.setPos ( btn_pos_low );
	_btn_low.set_size ( btn_size );
	_btn_high.setPos ( btn_pos_high );
	_btn_high.set_size ( btn_size );
	_rail.setPos ( rail_pos );
	_rail.set_size ( rail_size );
	_handle.setPos ( rail_pos );
	_handle.set_size ( handle_size );
}

inline
unsigned int
GW_Scrollbar::handle_pos ( )
{
}

unsigned int
GW_Scrollbar::handle_px_pos ( )
{
}

void
GW_Scrollbar::set_handle_px_pos (
	int px_pos_n )
{
}

unsigned int
GW_Scrollbar::px_pos_from_handle_pos (
	int slider_pos_n ) const
{
}

unsigned int
GW_Scrollbar::handle_pos_from_px_pos (
	int px_pos_n ) const
{
}

bool
GW_Scrollbar::point_in_handle (
	const QPointF & point_n ) const
{
}

void
GW_Scrollbar::mousePressEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	::std::cout << "GW_Scrollbar::mousePressEvent"  << "\n";
}

void
GW_Scrollbar::mouseReleaseEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	::std::cout << "GW_Scrollbar::mouseReleaseEvent"  << "\n";
}

void
GW_Scrollbar::mouseMoveEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	::std::cout << "GW_Scrollbar::mouseMoveEvent" << "\n";
}

void
GW_Scrollbar::wheelEvent (
	QGraphicsSceneWheelEvent * event_n )
{
	::std::cout << "GW_Scrollbar::wheelEvent" << "\n";
}


} // End of namespace
