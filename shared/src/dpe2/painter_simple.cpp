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
	::dpe2::Pixmap & pxmap_n,
	const ::dpe2::Key_Values & kvals_n )
{
	(void) kvals_n;
	pxmap_n.qimage().fill ( 0 );

	QRectF prect ( 0.0, 0.0, pxmap_n.width(), pxmap_n.height() );
	double pen_width ( 1.0 );
	double pwhalf ( pen_width / 2.0 );
	prect.adjust ( pwhalf, pwhalf, -pwhalf, -pwhalf );

	{
		QPainter pnt ( &pxmap_n.qimage() );
		pnt.setRenderHints ( QPainter::Antialiasing );
		{
			QPen ppen;
			ppen.setWidth ( pen_width );
			ppen.setColor ( random_color() );
			pnt.setPen ( ppen );
		}
		{
			QBrush brush ( Qt::SolidPattern );
			brush.setColor ( random_color() );
			pnt.setBrush ( brush );
		}
		const double crad ( 2.0 );
		pnt.drawRoundedRect ( prect, crad, crad );
	}
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

