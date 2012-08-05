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
_qpixmap ( 0 ),
_width ( 0 ),
_height ( 0 )
{
}

Pixmap::~Pixmap ( )
{
	reset_qpixmap();
}

void
Pixmap::clear ( )
{
	reset_qpixmap();
	_width = 0;
	_height = 0;
}

void
Pixmap::reset_qpixmap (
	QPixmap * pixmap_n )
{
	if ( _qpixmap != 0 ) {
		delete _qpixmap;
	}
	_qpixmap = pixmap_n;
}

void
Pixmap::set_size (
	unsigned int width_n,
	unsigned int height_n )
{
	if ( ( width() != width_n ) || ( height() != height_n ) ) {
		reset_qpixmap();
		_width = width_n;
		_height = height_n;
		_qimage = QImage ( _width, _height, QImage::Format_ARGB32_Premultiplied );
	}
}

void
Pixmap::convert_to_qpixmap ( )
{
	if ( qpixmap() == 0  ) {
		if ( ( _qimage.width() > 0 ) && ( _qimage.height() > 0 ) ) {
			reset_qpixmap ( new QPixmap ( QPixmap::fromImage ( _qimage ) ) );
			_qimage = QImage();
		}
	}
}


} // End of namespace
