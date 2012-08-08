//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "painter.hpp"
#include "pixmap.hpp"
#include <iostream>

namespace dpe2
{


Painter::Painter ( )
{
}

Painter::~Painter ( )
{
}

void
Painter::iref1_deref (
	::dpe2::Pixmap_IRef1 * iref1_n )
{
	mutex().lock();
	iref1_n->deref_one();
	if ( iref1_n->num_refs() == 0 ) {
		::dpe2::Pixmap_IRef0 * iref0 ( iref1_n->iref0() );
		iref0->destroy_iref1 ( iref1_n );
		if ( iref0->num_iref1() == 0 ) {
			iref0_destroy ( iref0 );
		}
	}
	mutex().unlock();
}

::dpe2::Pixmap_IRef0 *
Painter::iref0_create ( )
{
	::dpe2::Pixmap_IRef0 * res (
		new ::dpe2::Pixmap_IRef0 ( this ) );
	_iref0s.push_back ( res );
	return res;
}

void
Painter::iref0_destroy (
	::dpe2::Pixmap_IRef0 * iref0_n )
{
	IRef0_List::iterator it ( _iref0s.begin() );
	for ( ; it != _iref0s.end(); ++it ) {
		if ( *it == iref0_n ) {
			_iref0s.erase ( it );
			break;
		}
	}
}

bool
Painter::key_values_match (
	const ::dpe2::Key_Values & vset1_n,
	const ::dpe2::Key_Values & vset2_n ) const
{
	return ( vset1_n == vset2_n );
}

::dpe2::Pixmap_IRef1 *
Painter::find_match (
	const ::dpe2::Key_Values & vset1_n )
{
	::dpe2::Pixmap_IRef1 * res ( 0 );
	IRef0_List::iterator it0 ( _iref0s.begin() );
	for ( ; it0 != _iref0s.end(); ++it0 ) {
		::dpe2::Pixmap_IRef0 & iref0 ( *(*it0) );
		for ( unsigned int ii=0; ii < iref0.num_iref1(); ++ii ) {
			::dpe2::Pixmap_IRef1 & iref1 ( iref0.iref1 ( ii ) );
			if ( this->key_values_match ( vset1_n, iref1.key_values() ) ) {
				res = &iref1;
				break;
			}
		}
		if ( res != 0 ) {
			break;
		}
	}
	return res;
}

bool
Painter::valid_size (
	unsigned int width_n,
	unsigned int height_n )
{
	const unsigned int area ( width_n*height_n );
	if ( ( width_n <= _img_area_max ) &&
		( height_n <= _img_area_max ) &&
		( area <= _img_area_max ) &&
		( area != 0 ) )
	{
		return true;
	}
	return false;
}


} // End of namespace

