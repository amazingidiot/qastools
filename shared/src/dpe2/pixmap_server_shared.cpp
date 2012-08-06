//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "pixmap_server_shared.hpp"
#include "painter_simple.hpp"
#include <cassert>
#include <iostream>

namespace dpe2
{


Pixmap_Server_Shared::Pixmap_Server_Shared ( ) :
_queue_done_notified ( false )
{
	// Default painter
	install_painter ( new ::dpe2::Painter_Simple );
}

Pixmap_Server_Shared::~Pixmap_Server_Shared ( )
{
	deliver_finished_requests();

	assert ( _queue_done[0].size() == 0 );
	assert ( _queue_done[1].size() == 0 );
	assert ( _released_requests.size() == 0 );

	// Destroy painters
	{
		Painter_List::iterator it ( _painters.begin() );
		for ( ; it != _painters.end(); ++it ) {
			delete (*it);
		}
	}

	// Destroy paint waiters
	{
		Paint_Waiters_List::iterator it ( _paint_waiters.begin() );
		for ( ; it != _paint_waiters.end(); ++it ) {
			delete (*it);
		}
	}
}

void
Pixmap_Server_Shared::install_painter (
	::dpe2::Painter * painter_n )
{
	_painters.insert ( _painters.begin(), painter_n );
}

void
Pixmap_Server_Shared::start_threads ( )
{
	if ( threads_running() == 0 ) {
		int num_threads ( QThread::idealThreadCount() );
		if ( num_threads <= 0 ) {
			num_threads = 1;
		}

		for ( int ii = 0; ii < num_threads; ++ii ) {
			_threads.append ( new ::dpe2::Paint_Thread ( *this ) );
		}
		{
			Thread_List::iterator it ( _threads.begin() );
			for ( ; it != _threads.end(); ++it ) {
				(*it)->start();
			}
		}
	}
}

void
Pixmap_Server_Shared::abort_threads ( )
{
	const unsigned int num ( threads_running() );
	if ( num > 0 ) {
		_queue_new_mutex.lock();
		for ( unsigned int ii=0; ii < num; ++ii ) {
			_queue_new.append ( 0 );
		}
		_queue_new_mutex.unlock();

		_queue_new_cond.wakeAll();
	}
}

void
Pixmap_Server_Shared::join_threads ( )
{
	if ( threads_running() > 0 ) {
		{
			Thread_List::iterator it ( _threads.begin() );
			for ( ; it != _threads.end(); ++it ) {
				::dpe2::Paint_Thread * pth ( *it );
				pth->wait();
				delete pth;
			}
		}
		_threads.clear();
	}
}

void
Pixmap_Server_Shared::stop_threads ( )
{
	abort_threads();
	join_threads();
}

void
Pixmap_Server_Shared::set_one_done_callback (
	const ::Context_Callback & cb_n )
{
	_done_callback = cb_n;
}

::dpe2::Pixmap_Request *
Pixmap_Server_Shared::create_request ( )
{
	::dpe2::Pixmap_Request * req ( new ::dpe2::Pixmap_Request );
	return req;
}

void
Pixmap_Server_Shared::destroy_request (
	::dpe2::Pixmap_Request * request_n )
{
	delete request_n;
}

::dpe2::Pixmap_Request *
Pixmap_Server_Shared::acquire_request ( )
{
	return create_request();
}

void
Pixmap_Server_Shared::release_request (
	::dpe2::Pixmap_Request * request_n )
{
	if ( request_n->state.has_any ( ::dpe2::RS_PROCESSING ) ) {
		_released_requests.append ( request_n );
	} else {
		destroy_request ( request_n );
	}
}

bool
Pixmap_Server_Shared::destroy_request_on_demand (
	::dpe2::Pixmap_Request * request_n )
{
	bool res ( false );
	{
		Request_Queue::iterator it_d ( _released_requests.begin() );
		for ( ; it_d != _released_requests.end(); ++it_d ) {
			if ( *it_d == request_n ) {
				_released_requests.erase ( it_d );
				destroy_request ( request_n );
				res = true;
				break;
			}
		}
	}
	return res;
}

void
Pixmap_Server_Shared::send_request (
	::dpe2::Pixmap_Request * request_n )
{
	if ( request_n != 0 ) {
		request_n->state.set ( ::dpe2::RS_PROCESSING );
		if ( threads_running() > 0 ) {
			// Enqueue request for another thread
			enqueue_request ( request_n );
		} else {
			// Paint blocking and return
			process_request ( request_n );
		}
	}
}

void
Pixmap_Server_Shared::enqueue_request (
	::dpe2::Pixmap_Request * request_n )
{
	_queue_new_mutex.lock();
	_queue_new.enqueue ( request_n );
	_queue_new_mutex.unlock();

	_queue_new_cond.wakeOne();
}

::dpe2::Pixmap_Request *
Pixmap_Server_Shared::fetch_request ( )
{
	::dpe2::Pixmap_Request * request;

	_queue_new_mutex.lock();
	while ( _queue_new.size() == 0 ) {
		_queue_new_cond.wait ( &_queue_new_mutex );
	}
	request = _queue_new.dequeue();
	_queue_new_mutex.unlock();

	return request;
}

void
Pixmap_Server_Shared::process_request (
	::dpe2::Pixmap_Request * request_n )
{
	::dpe2::Painter * painter ( 0 );

	{
		const ::dpe2::Key_Values & kvals ( request_n->kvals );
		// Find responsible painter
		Painter_List::iterator it ( _painters.begin() );
		for ( ; it != _painters.end(); ++it ) {
			::dpe2::Painter * pnt ( *it );
			if ( pnt->is_responsible ( kvals ) ) {
				painter = pnt;
				break;
			}
		}
	}

	if ( painter != 0 ) {
		paint_request ( *painter, request_n );
	}
}

void
Pixmap_Server_Shared::paint_request (
	::dpe2::Painter & pnt_n,
	::dpe2::Pixmap_Request * request_n )
{
	const ::dpe2::Key_Values & kvals ( request_n->kvals );

	bool paint_new ( false );
	bool req_waiting ( false );
	::dpe2::Pixmap_IRef0 * iref0;
	::dpe2::Pixmap_IRef1 * iref1;

	pnt_n.mutex().lock();
	iref1 = pnt_n.find_match ( kvals );
	if ( iref1 == 0 ) {
		// Create new references
		iref0 = pnt_n.iref0_create();
		iref1 = iref0->create_iref1 ( kvals );
		iref0->set_waiter ( new ::dpe2::Pixmap_Paint_Waiter );
		paint_new = true;
	} else {
		// Enqueue to paint finish waiters list on demand
		::dpe2::Pixmap_Paint_Waiter * waiter ( iref1->iref0()->waiter() );
		if ( waiter != 0 ) {
			waiter->requests.enqueue ( request_n );
			req_waiting = true;
		}
	}
	iref1->ref_one();
	pnt_n.mutex().unlock();

	// Set pixmap reference in request
	request_n->pxm_ref.set_iref1 ( iref1 );

	if ( !req_waiting ) {
		::dpe2::Pixmap_Paint_Waiter * waiter ( 0 );

		// Paint pixmap new on demand
		if ( paint_new ) {
			pnt_n.paint_pixmap ( iref0->pixmap(), kvals );

			// Pick up waiting requests
			pnt_n.mutex().lock();
			waiter = iref0->waiter();
			iref0->set_waiter ( 0 );
			pnt_n.mutex().unlock();
		}

		// Push all finished requests to the output queue
		request_finished_begin();
		request_finished ( request_n );
		if ( waiter != 0 ) {
			while ( !waiter->requests.isEmpty() ) {
				request_finished ( waiter->requests.dequeue() );
			}
		}
		request_finished_end();

		if ( waiter != 0 ) {
			delete waiter;
		}
	}
}

inline
void
Pixmap_Server_Shared::request_finished_begin ( )
{
	_queue_done_mutex.lock();
}

inline
void
Pixmap_Server_Shared::request_finished (
	::dpe2::Pixmap_Request * request_n )
{
	_queue_done[0].enqueue ( request_n );
}

inline
void
Pixmap_Server_Shared::request_finished_end ( )
{
	const bool is_notified ( _queue_done_notified );
	_queue_done_notified = true;
	_queue_done_mutex.unlock();

	// Call back request issue server
	if ( !is_notified ) {
		_done_callback.call_if_valid();
	}
}

void
Pixmap_Server_Shared::deliver_finished_requests ( )
{
	_queue_done_mutex.lock();
	_queue_done[0].swap ( _queue_done[1] );
	_queue_done_notified = false;
	_queue_done_mutex.unlock();

	while ( !_queue_done[1].isEmpty() ) {
		::dpe2::Pixmap_Request * req ( _queue_done[1].dequeue() );
		req->state.unset ( ::dpe2::RS_PROCESSING );
		if ( !destroy_request_on_demand ( req ) ) {
			req->call_back();
		}
	}
}

::dpe2::Pixmap_Paint_Waiter *
Pixmap_Server_Shared::acquire_waiter ( )
{
	::dpe2::Pixmap_Paint_Waiter * res ( 0 );

	_shared_mutex.lock();
	if ( _paint_waiters.size() > 0 ) {
		res = _paint_waiters.takeLast();
	}
	_shared_mutex.unlock();

	if ( res == 0 ) {
		res = new ::dpe2::Pixmap_Paint_Waiter;
	}
	return res;
}

void
Pixmap_Server_Shared::release_waiter (
	::dpe2::Pixmap_Paint_Waiter * waiter_n )
{
	_shared_mutex.lock();
	_paint_waiters.append ( waiter_n );
	_shared_mutex.unlock();
}


} // End of namespace

