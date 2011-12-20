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

#include "image_set.hpp"


namespace dpe
{


Image_Set::Image_Set (
	unsigned int num_images_n ) :
_num_images ( num_images_n ),
_images ( new ::dpe::Image[num_images_n] )
{
}


Image_Set::~Image_Set ( )
{
}


unsigned int
Image_Set::byte_count ( ) const
{
	unsigned int cnt ( 0 );
	for ( unsigned int ii=0; ii < num_images(); ++ii ) {
		cnt += image ( ii ).byte_count();
	}
	return cnt;
}


void
Image_Set::convert_to_pixmap ( )
{
	for ( unsigned int ii=0; ii < num_images(); ++ii ) {
		image ( ii ).convert_to_pixmap();
	}
}


} // End of namespace
