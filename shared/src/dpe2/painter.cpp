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
Painter::process_request (
	::dpe2::Pixmap_Request & request_n )
{
	const ::dpe2::Key_Values & kvals ( request_n.kvals );
	if ( !this->is_responsible ( kvals ) ) {
		return false;
	}

	bool paint_new ( false );
	::dpe2::Pixmap_IRef0 * iref0;
	::dpe2::Pixmap_IRef1 * iref1;

	_mutex.lock();
	iref1 = find_match ( kvals );
	if ( iref1 == 0 ) {
		// Create new references
		iref0 = create_iref0();
		iref0->create_waiter();
		iref1 = iref0->create_iref1 ( kvals );
		iref1->ref_one();
		paint_new = true;
	} else {
		// Take existing reference
		iref0 = iref1->iref0();
		iref1->ref_one();

		// Wait until painting finished
		::dpe2::Pixmap_Paint_Waiter * waiter ( iref0->waiter() );
		if ( waiter != 0 ) {
			if ( !waiter->finished ) {
				++waiter->num_waiters;
				waiter->wait_cond.wait ( &_mutex );
				--waiter->num_waiters;
				if ( waiter->num_waiters == 0 ) {
					iref0->destroy_waiter();
				}
			}
		}
	}
	_mutex.unlock();

	// Paint pixmap new on demand
	if ( paint_new ) {
		paint_pixmap ( iref0->pixmap(), kvals );
		// Notify waiters on demand
		_mutex.lock();
		iref0->waiter()->finished = true;
		if ( iref0->waiter()->num_waiters > 0 ) {
			iref0->waiter()->wait_cond.wakeAll();
		} else {
			iref0->destroy_waiter();
		}
		_mutex.unlock();
	}

	// Set reference and update reference counter
	request_n.pxm_ref.set_iref1 ( iref1 );

	return true;
}

void
Painter::iref1_deref (
	::dpe2::Pixmap_IRef1 * iref1_n )
{
	_mutex.lock();
	iref1_n->deref_one();
	if ( iref1_n->num_refs() == 0 ) {
		::dpe2::Pixmap_IRef0 * iref0 ( iref1_n->iref0() );
		iref0->destroy_iref1 ( iref1_n );
		if ( iref0->num_iref1() == 0 ) {
			destroy_iref0 ( iref0 );
		}
	}
	_mutex.unlock();
}

::dpe2::Pixmap_IRef0 *
Painter::create_iref0 ( )
{
	::dpe2::Pixmap_IRef0 * res (
		new ::dpe2::Pixmap_IRef0 ( this ) );
	_iref0s.push_back ( res );
	return res;
}

void
Painter::destroy_iref0 (
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

void
Painter::paint_pixmap (
	::dpe2::Pixmap & pxmap_n,
	const ::dpe2::Key_Values & kvals_n )
{
	const unsigned int area_max ( 4096*4096 );
	unsigned int iwidth ( 0 );
	unsigned int iheight ( 0 );
	kvals_n.value_uint ( iwidth, ::dpe2::PMK_WIDTH );
	kvals_n.value_uint ( iheight, ::dpe2::PMK_HEIGHT );
	if ( iwidth*iheight > area_max ) {
		iwidth = 0;
		iheight = 0;
	}
	// Paint if the size is valid
	if ( ( iwidth > 0 ) && ( iheight > 0 ) ) {
		pxmap_n.set_size ( iwidth, iheight );
		pxmap_n.qimage().fill ( 0 );
		{
			QPainter pnt ( &pxmap_n.qimage() );
			pnt.setRenderHints ( QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform );
			this->paint ( pnt, kvals_n );
		}
	}
}


} // End of namespace

