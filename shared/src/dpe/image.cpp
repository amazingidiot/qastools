//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "image.hpp"

#include <QPixmap>


namespace dpe
{


Image::Image ( ) :
_width ( 0 ),
_height ( 0 ),
_stride ( 0 )
{
}


Image::~Image ( )
{
}


void
Image::clear ( )
{
	_pixmap.reset();
	_data.reset();
	_width = 0;
	_height = 0;
	_stride = 0;
}


void
Image::set_size (
	unsigned int width_n,
	unsigned int height_n,
	unsigned int stride_n )
{
	clear();
	_width = width_n;
	_height = height_n;
	_stride = stride_n;
	const unsigned int bc ( byte_count() );
	if ( bc > 0 ) {
		_data.reset ( new unsigned char[bc] );
	}
}


QPixmap *
Image::convert_to_pixmap ( )
{
	if ( ( pixmap() == 0 ) || ( data() != 0 ) ) {
		_pixmap.reset ( new QPixmap );
		{
			QImage img ( data(), width(), height(), stride(),
				QImage::Format_ARGB32_Premultiplied );
			*pixmap() = QPixmap::fromImage ( img );
		}
		// Clear data
		_data.reset();
	}
	return pixmap();
}


} // End of namespace
