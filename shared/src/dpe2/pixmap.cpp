//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "pixmap.hpp"

namespace dpe2
{


Pixmap::Pixmap ( ) :
_width ( 0 ),
_height ( 0 )
{
}

Pixmap::~Pixmap ( )
{
}

void
Pixmap::clear ( )
{
	_pixmap.reset();
	_width = 0;
	_height = 0;
}

void
Pixmap::set_size (
	unsigned int width_n,
	unsigned int height_n )
{
	clear();
	_width = width_n;
	_height = height_n;
	_qimage = QImage ( _width, _height, QImage::Format_ARGB32_Premultiplied );
}

QPixmap *
Pixmap::convert_to_pixmap ( )
{
	if ( ( pixmap() == 0 ) || ( !qimage().isNull() ) ) {
		_pixmap.reset ( new QPixmap ( QPixmap::fromImage ( _qimage ) ) );
		_qimage = QImage();
	}
	return pixmap();
}


} // End of namespace
