//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "painter_simple.hpp"

#include "paint_job.hpp"
#include "image_set.hpp"
#include "image_set_meta.hpp"
#include <QImage>
#include <QColor>


namespace dpe
{


Painter_Simple::Painter_Simple ( ) :
::dpe::Painter ( 0 )
{
}


Painter_Simple::~Painter_Simple ( )
{
}


int
Painter_Simple::paint_image (
	::dpe::Paint_Job * pjob_n )
{
	int res ( 0 );

	::dpe::Image & img ( pjob_n->img_set->image ( pjob_n->img_idx ) );
	::dpe::Image_Set_Meta * meta ( pjob_n->meta );

	img.set_size (
		meta->size.width(),
		meta->size.height(),
		meta->size.width() * 4 );

	{
		QImage qimg (
			img.data(), img.width(), img.height(), img.stride(),
			QImage::Format_ARGB32_Premultiplied );
		QColor col;
		{
			unsigned int seed ( pjob_n->img_set->num_images() + pjob_n->img_idx * 10 );
			seed += time(0);
			qsrand ( seed );
			col.setRgb ( qrand() % 256, qrand() % 256, qrand() % 256, 128 );
		}
		qimg.fill ( col.rgba() );
	}

	return res;
}


} // End of namespace
