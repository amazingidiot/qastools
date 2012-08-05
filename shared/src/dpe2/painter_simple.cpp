//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "painter_simple.hpp"
#include "sys/time.h"
#include "pixmap.hpp"
#include <iostream>

namespace dpe2
{


bool
Painter_Simple::is_responsible (
	const ::dpe2::Key_Values & vset_n )
{
	(void) vset_n;
	return true;
}

void
Painter_Simple::paint (
	QPainter & painter_n,
	const ::dpe2::Key_Values & vals_n )
{
	unsigned int iwidth ( 0 );
	unsigned int iheight ( 0 );
	vals_n.value_uint ( iwidth, ::dpe2::PMK_WIDTH );
	vals_n.value_uint ( iheight, ::dpe2::PMK_HEIGHT );

	QRectF prect ( QPointF ( 0.0, 0.0 ), QSizeF ( iwidth, iheight ) );
	double pen_width ( 1.0 );
	double pwhalf ( pen_width / 2.0 );
	prect.adjust ( pwhalf, pwhalf, -pwhalf, -pwhalf );
	{
		QPen ppen;
		ppen.setWidth ( pen_width );
		ppen.setColor ( random_color() );
		painter_n.setPen ( ppen );
	}
	{
		QBrush brush ( Qt::SolidPattern );
		brush.setColor ( random_color() );
		painter_n.setBrush ( brush );
	}
	const double crad ( 2.0 );
	painter_n.drawRoundedRect ( prect, crad, crad );
}

QColor
Painter_Simple::random_color ( ) const
{
	QColor col;
	{
		unsigned long seed;
		{
			struct timeval tval;
			gettimeofday ( &tval, NULL );
			seed = tval.tv_usec;
		}
		qsrand ( seed );
		col.setRgb ( qrand() % 256, qrand() % 256, qrand() % 256, 220 );
	}
	return col;
}


} // End of namespace

