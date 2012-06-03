//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_switch.hpp"
#include <iostream>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

namespace Wdg2
{


GW_Switch::GW_Switch (
	::QSnd2::Proxy_Switch & switch_proxy_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_switch_proxy ( switch_proxy_n ),
_switch_size ( 0.0, 0.0 )
{
	_switch_proxy.set_val_change_callback (
		::QSnd2::Context_Callback ( this, ::Wdg2::GW_Switch::read_proxy_value_cb ) );
	setFlags ( QGraphicsItem::ItemIsFocusable );
}

GW_Switch::~GW_Switch ( )
{
}

QRectF
GW_Switch::boundingRect ( ) const
{
	return QRectF ( QPointF ( 0.0, 0.0 ), QSizeF ( _switch_size ) );
}

void
GW_Switch::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) painter_n;
	(void) option_n;
	(void) widget_n;

	double pen_width ( 1.0 );
	if ( _widget_flags.has_any ( ::Wdg2::GW_HAS_FOCUS ) ) {
		pen_width = 2.0;
	}
	double pwhalf ( pen_width / 2.0 );
	QRectF prect ( QPointF ( 0.0, 0.0 ), _switch_size );
	prect.adjust ( pwhalf, pwhalf, -pwhalf, -pwhalf );

	{
		QPen ppen ( option_n->palette.color ( QPalette::ButtonText ) );
		ppen.setWidth ( pen_width );
		painter_n->setPen ( ppen );
	}
	painter_n->setBrush ( option_n->palette.color ( QPalette::Button ) );
	painter_n->drawRoundedRect ( prect, 2.0, 2.0 );

	if ( _state_flags.has_any ( ::Wdg2::GW_Switch::SF_ON | ::Wdg2::GW_Switch::SF_DOWN ) ) {
		painter_n->setPen ( Qt::NoPen );
		painter_n->setBrush ( option_n->palette.color ( QPalette::ButtonText ) );
		QRectF hrect ( prect );
		hrect.adjust ( 3, 3, -3, -3 );
		painter_n->drawRoundedRect ( hrect, 2.0, 2.0 );
	}
}

void
GW_Switch::set_switch_size (
	const QSize & size_n )
{
	prepareGeometryChange();
	_switch_size = size_n;
	read_proxy_value();
}

void
GW_Switch::read_proxy_value ( )
{
	const bool pstate ( _switch_proxy.switch_state() );
	if ( pstate != _state_flags.has_any ( ::Wdg2::GW_Switch::SF_ON ) ) {
		_state_flags.set ( ::Wdg2::GW_Switch::SF_ON, pstate );
		update();
	}
}

void
GW_Switch::read_proxy_value_cb (
	void * context_n )
{
	::Wdg2::GW_Switch & gw_switch (
		*reinterpret_cast < ::Wdg2::GW_Switch * > ( context_n ) );
	gw_switch.read_proxy_value();
}

void
GW_Switch::focusInEvent (
	QFocusEvent * event_n )
{
	//::std::cout << "GW_Switch::focusInEvent"  << "\n";
	_widget_flags.set ( ::Wdg2::GW_HAS_FOCUS );
	update();
}

void
GW_Switch::focusOutEvent (
	QFocusEvent * event_n )
{
	//::std::cout << "GW_Switch::focusOutEvent"  << "\n";
	_widget_flags.unset ( ::Wdg2::GW_HAS_FOCUS );
	update();
}

void
GW_Switch::mousePressEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	//::std::cout << "GW_Switch::mousePressEvent"  << "\n";
	_state_flags.set ( ::Wdg2::GW_Switch::SF_DOWN );
	update();
}

void
GW_Switch::mouseReleaseEvent (
	QGraphicsSceneMouseEvent * event_n )
{
	//::std::cout << "GW_Switch::mouseReleaseEvent"  << "\n";
	_state_flags.unset ( ::Wdg2::GW_Switch::SF_DOWN );
	_switch_proxy.toggle_switch_state();
	read_proxy_value();
	update();
}


} // End of namespace
