//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "pixmap_ref.hpp"
#include "pixmap.hpp"
#include <iostream>
#include <cassert>

namespace dpe2
{


Pixmap_IRef0::Pixmap_IRef0 (
	::dpe2::Painter * painter_n ) :
_painter ( painter_n ),
_waiter ( 0 )
{
}

::dpe2::Pixmap_IRef1 *
Pixmap_IRef0::create_iref1 (
	const ::dpe2::Key_Values & kvals_n )
{
	::dpe2::Pixmap_IRef1 * res (
		new ::dpe2::Pixmap_IRef1 ( this, kvals_n ) );
	_iref1s.push_back ( res );
	return res;
}

void
Pixmap_IRef0::destroy_iref1 (
	::dpe2::Pixmap_IRef1 * iref1_n )
{
	IRef1_List::iterator it ( _iref1s.begin() );
	for ( ; it != _iref1s.end(); ++it ) {
		if ( *it == iref1_n ) {
			_iref1s.erase ( it );
			break;
		}
	}
}



Pixmap_Ref::Pixmap_Ref (
	::dpe2::Pixmap_IRef1 * iref1_n )
{
	set_iref1 ( iref1_n );
}

void
Pixmap_Ref::clear ( )
{
	_pixmap = 0;
	_iref1 = 0;
}

void
Pixmap_Ref::set_iref1 (
	::dpe2::Pixmap_IRef1 * iref1_n )
{
	_pixmap = 0;
	_iref1 = iref1_n;
	if ( _iref1 != 0 ) {
		_pixmap = &_iref1->pixmap();
	}
}


} // End of namespace
