//
// C++ Implementation:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "image.hpp"

#include <QPixmap>


namespace dpe
{


Image::Image ( ) :
pixmap ( 0 ),
width ( 0 ),
height ( 0 ),
stride ( 0 ),
data ( 0 )
{
}


Image::~Image ( )
{
	clear();
}


void
Image::clear ( )
{
	if ( pixmap != 0 ) {
		delete pixmap;
		pixmap = 0;
	}
	width = 0;
	height = 0;
	stride = 0;
	clear_data();
}


void
Image::clear_data ( )
{
	if ( data != 0 ) {
		delete[] data;
		data = 0;
	}
}


unsigned int
Image::byte_count ( ) const
{
	return height*stride;
}


void
Image::convert_to_pixmap ( )
{
	if ( ( pixmap == 0 ) || ( data != 0 ) ) {
		pixmap = new QPixmap;
		{
			QImage img ( data, width, height, stride, QImage::Format_ARGB32_Premultiplied );
			*pixmap = QPixmap::fromImage ( img );
		}
		clear_data();
	}
}


} // End of namespace
