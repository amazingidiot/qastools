//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "pixmap_handle.hpp"
#include "pixmap.hpp"
#include <iostream>
#include <cassert>


namespace dpe2
{


Pixmap_Handle::Pixmap_Handle (
	::dpe2::Painter * painter_n ) :
pixmap ( 0 ),
painter ( painter_n ),
num_users ( 0 )
{
}

Pixmap_Handle::~Pixmap_Handle ( )
{
}



Pixmap_Ref0::Pixmap_Ref0 ( ) :
_pixmap ( new ::dpe2::Pixmap )
{
}



Pixmap_Ref2::Pixmap_Ref2 (
	::dpe2::Pixmap_Ref1 * ref1_n )
{
	set_ref1 ( ref1_n );
}

void
Pixmap_Ref2::clear ( )
{
	_pixmap = 0;
	_ref1 = 0;
}

void
Pixmap_Ref2::set_ref1 (
	::dpe2::Pixmap_Ref1 * ref1_n )
{
	_pixmap = 0;
	_ref1 = ref1_n;
	if ( _ref1 != 0 ) {
		_pixmap = _ref1->pixmap();
	}
}


} // End of namespace
