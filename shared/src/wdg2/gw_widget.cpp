//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_widget.hpp"
#include <cassert>

namespace Wdg2
{


GW_Widget::GW_Widget (
	::Wdg2::Scene_Database * scene_db_n,
	QGraphicsItem * parent_n ) :
QGraphicsItem ( parent_n ),
_brect ( 0.0, 0.0, 0.0, 0.0 ),
_scene_db ( scene_db_n )
{
}

GW_Widget::~GW_Widget ( )
{
}

QRectF
GW_Widget::boundingRect ( ) const
{
	return _brect;
}

void
GW_Widget::set_bounding_rect (
	const QRectF & rect_n )
{
	if ( _brect != rect_n ) {
		prepareGeometryChange();
		_brect = rect_n;
	}
}

void
GW_Widget::set_bounding_rect (
	const QSizeF & size_n )
{
	set_bounding_rect ( QRectF ( QPointF ( 0.0, 0.0 ), size_n ) );
}

void
GW_Widget::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	// Default implementation
	(void) painter_n;
	(void) option_n;
	(void) widget_n;
}


} // End of namespace
