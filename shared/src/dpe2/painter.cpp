//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "painter.hpp"
#include "sys/time.h"
#include "pixmap.hpp"

namespace dpe2
{


Painter::Painter ( )
{
}

Painter::~Painter ( )
{
}

bool
Painter::key_values_match (
	const ::dpe2::Key_Values & vset1_n,
	const ::dpe2::Key_Values & vset2_n ) const
{
	return ( vset1_n == vset2_n );
}

::dpe2::Pixmap_Ref1 *
Painter::find_match (
	const ::dpe2::Key_Values & vset1_n ) const
{
	::dpe2::Pixmap_Ref1 * res ( 0 );
	for ( int ii=0; ii < _refs1.size(); ++ii ) {
		::dpe2::Pixmap_Ref1 * ref ( _refs1[ii] );
		if ( this->key_values_match ( vset1_n, res->key_values() ) ) {
			res = ref;
			break;
		}
	}
	return res;
}

bool
Painter::process_request (
	::dpe2::Pixmap_Request & request_n )
{
	const ::dpe2::Key_Values & kvals ( request_n.key_values );
	if ( !this->is_responsible ( kvals ) ) {
		return false;
	}

	bool paint_new ( false );

	_mutex.lock();
	::dpe2::Pixmap_Ref0 * ref0 ( 0 );
	::dpe2::Pixmap_Ref1 * ref1 ( find_match ( kvals ) );
	if ( ref1 == 0 ) {
		ref0 = new ::dpe2::Pixmap_Ref0;
		ref1 = new ::dpe2::Pixmap_Ref1 ( ref0 );
		paint_new = true;
	}
	_mutex.unlock();

	if ( paint_new ) {
		// Adjust pixmap size
		{
			unsigned int iwidth ( 0 );
			unsigned int iheight ( 0 );
			kvals.value_uint ( iwidth, ::dpe2::PMK_WIDTH );
			kvals.value_uint ( iheight, ::dpe2::PMK_HEIGHT );
			const unsigned int smax ( 4096*4096 );
			if ( iwidth*iheight > smax ) {
				iwidth = 0;
				iheight = 0;
			}
			// Paint if the size is valid
			if ( ( iwidth > 0 ) && ( iheight > 0 ) ) {
				ref0->pixmap()->set_size ( iwidth, iheight );
				QPainter pnt ( &ref0->pixmap()->qimage() );
				this->paint ( pnt, kvals );
			}
		}
	}

	// Set reference and update reference counter
	request_n.pixmap_ref.set_ref1 ( ref1 );
	ref1->num_ref2().ref();

	return true;
}



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

