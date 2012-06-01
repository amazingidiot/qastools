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
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_switch_size ( 0.0, 0.0 )
{
}

GW_Switch::~GW_Switch ( )
{
}

QRectF
GW_Switch::boundingRect ( ) const
{
	return QRectF ( QPointF ( 0.0, 0.0 ), _switch_size );
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
}

void
GW_Switch::set_switch_size (
	const QSize & size_n )
{
	prepareGeometryChange();
	_switch_size = size_n;
}


} // End of namespace
