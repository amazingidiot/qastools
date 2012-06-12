//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "painter.hpp"
#include "sys/time.h"
#include <QPainter>

namespace dpe2
{


Painter::Painter ( )
{
}

Painter::~Painter ( )
{
}

void
Painter::paint_handle (
	::dpe2::Pixmap_Handle * handle_n )
{
	handle_n->pixmap.reset ( new ::dpe2::Pixmap );
	{
		const ::dpe2::Values_Set & vals ( handle_n->id_values );
		unsigned int iwidth ( 0 );
		unsigned int iheight ( 0 );
		vals.value_uint ( iwidth, ::dpe2::PMK_WIDTH );
		vals.value_uint ( iheight, ::dpe2::PMK_HEIGHT );
		const unsigned int smax ( 4096*4096 );
		if ( iwidth*iheight < smax ) {
			handle_n->pixmap->set_size ( iwidth, iheight );
		}
	}

	if ( ( handle_n->pixmap->width() != 0 ) &&
		( handle_n->pixmap->height() != 0 ) )
	{
		QPainter pnt ( &handle_n->pixmap->qimage() );
		this->paint ( pnt, handle_n->id_values );
	}
}


void
Painter_Simple::paint (
	QPainter & painter_n,
	const ::dpe2::Values_Set & vals_n )
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
		QBrush brush;
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
		col.setRgb ( qrand() % 256, qrand() % 256, qrand() % 256, 128 );
	}
	return col;
}


} // End of namespace

