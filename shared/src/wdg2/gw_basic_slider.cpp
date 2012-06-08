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
::Wdg2::GW_Widget ( parent_n ),
_rail_size ( 0.0, 0.0 )
{
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
		_brect.setSize ( QSizeF ( _rail_size ) );
	}
}



GW_Slider_Handle::GW_Slider_Handle (
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget ( parent_n ),
_handle_size ( 0.0, 0.0 )
{
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
		_brect.setSize ( QSizeF ( _handle_size ) );
	}
}



GW_Slider::GW_Slider (
	::QSnd2::Proxy_Slider & slider_proxy_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_slider_proxy ( slider_proxy_n ),
_brect ( 0.0, 0.0, 0.0, 0.0 ),
_orientation ( Qt::Vertical ),
_rail_span ( 0 ),
_handle_pos ( 0 ),
_handle_pos_span ( 0 ),
_rail ( this ),
_handle ( this )
{
	setFlags ( QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemHasNoContents );
	_rail.setPos ( QPointF ( 0.0, 0.0 ) );
	_handle.setPos ( QPointF ( 0.0, 0.0 ) );
	_slider_proxy.set_val_change_callback (
		::Context_Callback ( this, ::Wdg2::GW_Slider::update_handle_pos_from_value_cb ) );

	update_handle_pos_from_value();
}

GW_Slider::~GW_Slider ( )
{
	_slider_proxy.set_val_change_callback ( ::Context_Callback() );
}

QRectF
GW_Slider::boundingRect ( ) const
{
	return _brect;
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
	prepareGeometryChange();
	_sizes = sizes_n;
	_brect.setSize ( QSizeF ( _sizes.size ) );

	update_geometries();
}

void
GW_Slider::set_orientation (
	Qt::Orientation orientation_n )
{
	if ( _orientation != orientation_n ) {
		_orientation = orientation_n;
		update_geometries();
	}
}

void
GW_Slider::update_geometries ( )
{
	QSize rail_size ( _sizes.size );
	QSize handle_size;

	unsigned int rail_length;
	if ( orientation() == Qt::Horizontal ) {
		handle_size = QSize ( _sizes.handle_length, _sizes.size.height() );
		rail_length = _sizes.size.width();
	} else {
		handle_size = QSize ( _sizes.size.width(), _sizes.handle_length );
		rail_length = _sizes.size.height();
	}

	_rail_span = rail_length - 1;
	_handle.set_handle_size ( handle_size );
	_handle_pos_span = ( rail_length - _sizes.handle_length );
	_handle_pos = ( _handle_pos_span + 1 ); // Invalid value to enforce update
	_rail.set_rail_size ( _sizes.size );

	update_handle_pos_from_value();
}

void
GW_Slider::update_handle_pos_from_value ( )
{
	if ( !_handle.state_flags().has_any ( ::Wdg2::GW_IS_GRABBED ) ) {
		const unsigned int pos (
			value_map().px_from_value ( _slider_proxy.int_value() ) );
		set_handle_pos ( pos );
	}
}

void
GW_Slider::update_handle_pos_from_value_cb (
	void * context_n )
{
	::Wdg2::GW_Slider & gw_slider (
		*reinterpret_cast < ::Wdg2::GW_Slider * > ( context_n ) );
	gw_slider.update_handle_pos_from_value();
}

void
GW_Slider::update_proxy_value_from_handle_pos ( )
{
	_slider_proxy.set_int_value (
		value_map().value_from_px ( _handle_pos ) );
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
			QPointF pos;
			if ( orientation() == Qt::Horizontal ) {
				pos.setX ( _handle_pos );
				pos.setY ( 0.0 );
			} else {
				pos.setX ( 0.0 );
				pos.setY ( _sizes.size.height() - _sizes.handle_length - _handle_pos );
			}
			_handle.setPos ( pos );
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
		hwidth = _sizes.size.height();
	} else {
		plen   = _sizes.size.height() - 1 - (int)point_n.y();
		pwidth = point_n.x();
		hwidth = _sizes.size.width();
	}
	plen -= _handle_pos;

	const bool res =
		( plen >= 0 ) &&
		( pwidth >= 0 ) &&
		( plen < (int)_sizes.handle_length ) &&
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
		update_proxy_value_from_handle_pos();
	}
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
		update_handle_pos_from_value();
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
			int lfrom;
			int lto;
			if ( orientation() == Qt::Horizontal ) {
				lfrom = event_n->lastPos().x();
				lto   = event_n->pos().x();
			} else {
				const double lheight ( _sizes.size.height() - 1 );
				lfrom = lheight - event_n->lastPos().y();
				lto   = lheight - event_n->pos().y();
			}

			// Dead zones at and beyond the rail ends
			// for a movement direction towards the rail center
			int lim ( _sizes.handle_length / 2 );
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
	::std::cout << "GW_Slider::wheelEvent" << "\n";
}


} // End of namespace
