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


GW_Scrollbar::GW_Scrollbar (
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n )
{
	//setFlags ( QGraphicsItem::ItemHasNoContents );
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
		_size = size_n;
	}
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
