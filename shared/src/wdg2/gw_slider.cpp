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
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QEvent>


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
	prepareGeometryChange();
	_rail_size = size_n;
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
	prepareGeometryChange();
	_handle_size = size_n;
}



GW_Slider::GW_Slider (
	::QSnd2::Proxy_Slider & slider_proxy_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_slider_proxy ( slider_proxy_n ),
_rail ( this ),
_handle ( this )
{
	_rail.setPos ( QPointF ( 0.0, 0.0 ) );
	_handle.setPos ( QPointF ( 0.0, 0.0 ) );
	_slider_proxy.set_val_change_callback (
		::QSnd2::Context_Callback ( this, ::Wdg2::GW_Slider::update_slider_position_cb ) );

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

::Wdg2::GW_Multi_Slider *
GW_Slider::multi_slider ( ) const
{
	return static_cast < ::Wdg2::GW_Multi_Slider * > ( parentItem() );
}

void
GW_Slider::set_sizes (
	const ::Wdg2::GW_Slider_Sizes & sizes_n )
{
	_sizes = sizes_n;
	_rail.set_rail_size ( _sizes.size );
	_handle.set_handle_size ( _sizes.handle_size );
	update_slider_position();
}

void
GW_Slider::update_slider_position ( )
{
	unsigned int pos ( _sizes.size.height() - _sizes.handle_size.height() );
	pos -= multi_slider()->value_map().px_from_value ( _slider_proxy.int_value() );
	_handle.setPos ( QPointF ( 0.0, double ( pos ) ) );
}

void
GW_Slider::update_slider_position_cb (
	void * context_n )
{
	::Wdg2::GW_Slider * slider (
		reinterpret_cast < ::Wdg2::GW_Slider * > ( context_n ) );
	slider->update_slider_position();
}

void
GW_Slider::focusInEvent (
	QFocusEvent * event_n )
{
	::std::cout << "GW_Slider::focusInEvent"  << "\n";
	_rail.state_flags().set ( ::Wdg2::GW_HAS_FOCUS );
	_handle.state_flags().set ( ::Wdg2::GW_HAS_FOCUS );
	_rail.update();
	_handle.update();
}

void
GW_Slider::focusOutEvent (
	QFocusEvent * event_n )
{
	::std::cout << "GW_Slider::focusOutEvent"  << "\n";
	_rail.state_flags().unset ( ::Wdg2::GW_HAS_FOCUS );
	_handle.state_flags().unset ( ::Wdg2::GW_HAS_FOCUS );
	_rail.update();
	_handle.update();
}


} // End of namespace
