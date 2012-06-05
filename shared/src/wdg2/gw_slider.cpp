//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_slider.hpp"
#include "gw_multi_slider.hpp"
#include <iostream>
#include <cmath>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

namespace Wdg2
{


GW_Slider_Rail::GW_Slider_Rail (
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_rail_size ( 0.0, 0.0 )
{
}

QRectF
GW_Slider_Rail::boundingRect ( ) const
{
	return QRectF ( QPointF ( 0.0, 0.0 ), QSizeF ( _rail_size ) );
}

void
GW_Slider_Rail::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;

	double pen_width ( 1.0 );
	if ( state_flags().has_any ( ::Wdg2::GW_HAS_FOCUS ) ) {
		pen_width = 2;
	}
	double pwhalf ( pen_width / 2.0 );
	QRectF prect ( QPointF ( 0.0, 0.0 ), _rail_size );
	prect.adjust ( pwhalf, pwhalf, -pwhalf, -pwhalf );
	prect.adjust ( 3, 3, -3, -3 );

	{
		QPen ppen ( option_n->palette.color ( QPalette::ButtonText ) );
		ppen.setWidth ( pen_width );
		painter_n->setPen ( ppen );
	}
	painter_n->setBrush ( option_n->palette.color ( QPalette::Button ) );
	painter_n->drawRoundedRect ( prect, 2.0, 2.0 );
}

void
GW_Slider_Rail::set_rail_size (
	const QSize & size_n )
{
	if ( size_n != _rail_size ) {
		prepareGeometryChange();
		_rail_size = size_n;
	}
}



GW_Slider_Handle::GW_Slider_Handle (
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_handle_size ( 0.0, 0.0 )
{
}

QRectF
GW_Slider_Handle::boundingRect ( ) const
{
	return QRectF ( QPointF ( 0.0, 0.0 ), QSizeF ( _handle_size ) );
}

void
GW_Slider_Handle::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;

	double pen_width ( 2.0 );
	double pwhalf ( pen_width / 2.0 );
	QRectF prect ( QPointF ( 0.0, 0.0 ), _handle_size );
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
GW_Slider_Handle::set_handle_size (
	const QSize & size_n )
{
	if ( size_n != _handle_size ) {
		prepareGeometryChange();
		_handle_size = size_n;
	}
}



GW_Slider::GW_Slider (
	::QSnd2::Proxy_Slider & slider_proxy_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_slider_proxy ( slider_proxy_n ),
_px_span ( 0 ),
_rail ( this ),
_handle ( this )
{
	_rail.setPos ( QPointF ( 0.0, 0.0 ) );
	_handle.setPos ( QPointF ( 0.0, 0.0 ) );
	_slider_proxy.set_val_change_callback (
		::QSnd2::Context_Callback ( this, ::Wdg2::GW_Slider::read_proxy_value_cb ) );

	setFlags ( QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemHasNoContents );
}

GW_Slider::~GW_Slider ( )
{
	_slider_proxy.set_val_change_callback ( ::QSnd2::Context_Callback() );
}

QRectF
GW_Slider::boundingRect ( ) const
{
	return QRectF ( QPointF ( 0.0, 0.0 ), QSizeF ( _sizes.size ) );
}

void
GW_Slider::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;
}

const ::Wdg2::Slider_Value_Map &
GW_Slider::value_map ( ) const
{
	const ::Wdg2::GW_Multi_Slider & sgrp (
		*static_cast < ::Wdg2::GW_Multi_Slider * > ( parentItem() ) );
	return sgrp.value_map();
}

void
GW_Slider::set_sizes (
	const ::Wdg2::GW_Slider_Sizes & sizes_n )
{
	_sizes = sizes_n;
	_px_span = ( _sizes.size.height() - _sizes.handle_size.height() );

	_rail.set_rail_size ( _sizes.size );
	_handle.set_handle_size ( _sizes.handle_size );
	read_proxy_value();
}

void
GW_Slider::read_proxy_value ( )
{
	if ( !_handle.state_flags().has_any ( ::Wdg2::GW_IS_GRABBED ) ) {
		set_handle_px_pos (
			value_map().px_from_value ( _slider_proxy.int_value() ) );
	}
}

void
GW_Slider::read_proxy_value_cb (
	void * context_n )
{
	::Wdg2::GW_Slider & gw_slider (
		*reinterpret_cast < ::Wdg2::GW_Slider * > ( context_n ) );
	gw_slider.read_proxy_value();
}

inline
unsigned int
GW_Slider::handle_pos ( )
{
	return _handle.pos().y();
}

unsigned int
GW_Slider::handle_px_pos ( )
{
	return px_pos_from_handle_pos ( handle_pos() );
}

void
GW_Slider::set_handle_px_pos (
	int px_pos_n )
{
	double hpos ( handle_pos_from_px_pos ( px_pos_n ) );
	QPointF pos ( 0.0, hpos );
	_handle.setPos ( pos );
}

unsigned int
GW_Slider::px_pos_from_handle_pos (
	int slider_pos_n ) const
{
	if ( slider_pos_n < 0 ) {
		slider_pos_n = 0;
	}
	if ( slider_pos_n > (int)_px_span ) {
		slider_pos_n = _px_span;
	}
	unsigned int px_pos ( _px_span );
	px_pos -= slider_pos_n;
	return px_pos;
}

unsigned int
GW_Slider::handle_pos_from_px_pos (
	int px_pos_n ) const
{
	if ( px_pos_n < 0 ) {
		px_pos_n = 0;
	}
	if ( px_pos_n > (int)_px_span ) {
		px_pos_n = _px_span;
	}
	unsigned int hpos ( _px_span );
	hpos -= px_pos_n;
	return hpos;
}

bool
GW_Slider::point_in_handle (
	const QPointF & point_n ) const
{
	const QPointF hpos ( _handle.pos() );
	if ( point_n.y() > hpos.y() ) {
		if ( point_n.y() < ( hpos.y() + _sizes.handle_size.height() ) ) {
			return true;
		}
	}
	return false;
}

void
GW_Slider::focusInEvent (
	QFocusEvent * event_n )
{
	//::std::cout << "GW_Slider::focusInEvent"  << "\n";
	_rail.state_flags().set ( ::Wdg2::GW_HAS_FOCUS );
	_handle.state_flags().set ( ::Wdg2::GW_HAS_FOCUS );
	_rail.update();
	_handle.update();
}

void
GW_Slider::focusOutEvent (
	QFocusEvent * event_n )
{
	//::std::cout << "GW_Slider::focusOutEvent"  << "\n";
	_rail.state_flags().unset ( ::Wdg2::GW_HAS_FOCUS );
	_handle.state_flags().unset ( ::Wdg2::GW_HAS_FOCUS );
	_rail.update();
	_handle.update();
}

void
GW_Slider::mousePressEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	//::std::cout << "GW_Slider::mousePressEvent"  << "\n";
	if ( point_in_handle ( event_n->pos() ) ) {
		_handle.state_flags().set ( ::Wdg2::GW_IS_GRABBED );
		_handle.update();
	}
}

void
GW_Slider::mouseReleaseEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	//::std::cout << "GW_Slider::mouseReleaseEvent"  << "\n";
	if ( _handle.state_flags().has_any ( ::Wdg2::GW_IS_GRABBED ) ) {
		_handle.state_flags().unset ( ::Wdg2::GW_IS_GRABBED );
		read_proxy_value();
		_handle.update();
	}
}

void
GW_Slider::mouseMoveEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	//::std::cout << "GW_Slider::mouseMoveEvent" << "\n";
	if ( _handle.state_flags().has_any ( ::Wdg2::GW_IS_GRABBED ) ) {
		int delta;
		{
			float deltaf ( event_n->pos().y() - event_n->lastPos().y() );
			deltaf = ::std::floor ( deltaf + 0.5f );
			delta = deltaf;
		}
		if ( delta != 0 ) {
			set_handle_px_pos ( handle_px_pos() - delta );
			long value = value_map().value_from_px ( handle_px_pos() );
			_slider_proxy.set_int_value ( value );
		}
	}
}

void
GW_Slider::wheelEvent (
	QGraphicsSceneWheelEvent * event_n )
{
	::std::cout << "GW_Slider::wheelEvent" << "\n";
}


} // End of namespace
