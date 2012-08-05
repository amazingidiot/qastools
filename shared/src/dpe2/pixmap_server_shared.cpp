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

	for ( int ii=0; ii < _painters.size(); ++ii ) {
		delete _painters[ii];
	}
	_painters.clear();
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
	if ( threads_running() > 0 ) {
		_queue_new_mutex.lock();
		for ( int ii=0; ii < _threads.size(); ++ii ) {
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
				(*it)->wait();
				delete (*it);
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
	paint_request ( request_n );
	request_finished ( request_n );
}

void
Pixmap_Server_Shared::paint_request (
	::dpe2::Pixmap_Request * request_n )
{
	// Find painter and paint
	Painter_List::iterator it ( _painters.begin() );
	for ( ; it != _painters.end(); ++it ) {
		if ( (*it)->process_request ( *request_n ) ) {
			break;
		}
	}
}

void
Pixmap_Server_Shared::request_finished (
	::dpe2::Pixmap_Request * request_n )
{
	_queue_done_mutex.lock();
	_queue_done[0].enqueue ( request_n );
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


} // End of namespace

