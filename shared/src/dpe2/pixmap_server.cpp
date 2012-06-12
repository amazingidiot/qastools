//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "pixmap_server.hpp"
#include "pixmap_server_shared.hpp"
#include "pixmap_request.hpp"
#include "pixmap_handle.hpp"
#include "painter.hpp"
#include "paint_thread.hpp"
#include <QTime>

#include <iostream>
#include <cassert>


namespace dpe2
{


Pixmap_Server::Pixmap_Server ( )
{
	_shared.reset ( new ::dpe2::Pixmap_Server_Shared );
	_multithread = multithreading_is_safe();

	/* TODO:
	_stop_timer.setInterval ( 1500 );
	_stop_timer.setSingleShot ( true );
	connect ( &_stop_timer, SIGNAL ( timeout() ),
		this, SLOT ( stop_timeout() ) );
	*/
}

Pixmap_Server::~Pixmap_Server ( )
{
	stop_threads();
}

void
Pixmap_Server::install_painter (
	::dpe2::Painter * painter_n )
{
	_shared->painters.append ( painter_n );
}

bool
Pixmap_Server::multithreading_is_safe ( ) const
{
	bool res ( true );
	/// @see https://bugreports.qt.nokia.com/browse/QTBUG-14614
	if ( QT_VERSION < QT_VERSION_CHECK(4, 7, 2) ) {
		res = false;
	}
	return res;
}

void
Pixmap_Server::set_multithread (
	bool flag_n )
{
	flag_n = ( flag_n && multithreading_is_safe() );
	if ( flag_n != _multithread ) {
		if ( _multithread ) {
			// TODO:
			//_shared->wait_for_finish();
		}
		_multithread = flag_n;
	}
}

void
Pixmap_Server::start_threads ( )
{
	int num_threads ( QThread::idealThreadCount() );
	if ( num_threads <= 0 ) {
		num_threads = 1;
	}

	for ( int ii = 0; ii < num_threads; ++ii ) {
		_threads.append ( new ::dpe2::Paint_Thread ( *_shared ) );
	}
	for ( int ii = 0; ii < num_threads; ++ii ) {
		_threads[ii]->start();
	}
}

void
Pixmap_Server::stop_threads ( )
{
	if ( _threads.size() > 0 ) {
		// Send abort signal
		_shared->abort_threads ( _threads.size() );
		for ( int ii = 0; ii < _threads.size(); ++ii ) {
			_threads[ii]->wait();
			delete _threads[ii];
		}
		_threads.clear();
	}
}

/* TODO
void
Pixmap_Server::stop_timeout ( )
{
	_shared->num_rendering.ref();
	if ( !_shared->num_rendering.deref() ) {
		stop_threads();
	} else {
		_stop_timer.start();
	}
}
*/

::dpe2::Pixmap_Handle *
Pixmap_Server::find_handle (
	::dpe2::Values_Set & vals_n )
{
	::dpe2::Pixmap_Handle * res ( 0 );
	for ( int ii=0; ii < _pixmap_handles.size(); ++ii ) {
		::dpe2::Pixmap_Handle * phandle ( _pixmap_handles[ii] );
		if ( vals_n == phandle->id_values ) {
			res = phandle;
			break;
		}
	}
	return res;
}

void
Pixmap_Server::send_request (
	::dpe2::Pixmap_Request * request_n )
{
	if ( request_n == 0 ) {
		return;
	}

	/* TODO
	for ( unsigned int ii=0; ii < request_n->group->img_sets.size(); ++ii ) {
		::dpe::IS_Buffer_Handle * handle;

		handle = _buffer.acquire_return_handle (
			request_n->meta[ii], request_n->group->img_sets[ii] );
		if ( handle == 0 ) {
			// No matching image set found!
			// Create new handle and image set
			// and enqueue for rendering
			::dpe::IS_Buffer_Handle * hnew ( new ::dpe::IS_Buffer_Handle );
			hnew->meta = request_n->meta[ii]->new_copy();
			hnew->img_set = new ::dpe::Image_Set ( hnew->meta->num_images );
			_buffer.append_handle ( hnew );
			process_handle ( hnew );

			handle = hnew;
		}
		request_n->group->img_sets[ii] = handle->img_set;
		request_n->states[ii] = handle->state;
	}
	*/
}

void
Pixmap_Server::post_request (
	::dpe2::Pixmap_Request * request_n )
{
	if ( request_n == 0 ) {
		return;
	}

}

void
Pixmap_Server::process_handle (
	::dpe2::Pixmap_Handle * handle_n )
{
	/* TODO
	if ( _multithread ) {
		if ( _threads.size() <= 0 ) {
			start_threads();
		}
		_stop_timer.start();
		enqueue_handle ( handle_n );
	} else {
		render_handle ( handle_n );
	}
	*/
}

void
Pixmap_Server::render_handle (
	::dpe2::Pixmap_Handle * handle_n )
{
	/* TODO

	handle_n->state->init_todo ( 0 );

	// Create paint jobs
	unsigned int num ( handle_n->img_set->num_images() );
	for ( unsigned int ii=0; ii < num; ++ii ) {
		::dpe::Paint_Job pjob;
		pjob.meta = handle_n->meta;
		pjob.img_set = handle_n->img_set;
		pjob.img_idx = ii;
		pjob.state = handle_n->state;

		_shared->paint_job ( &pjob );
	}
	*/
}

void
Pixmap_Server::enqueue_handle (
	::dpe2::Pixmap_Handle * handle_n )
{
	/*
	// Increment todo counters
	handle_n->state->init_todo (
		handle_n->img_set->num_images() );
	_shared->num_rendering.ref();

	// Create paint jobs and enqueue
	unsigned int num ( handle_n->img_set->num_images() );
	for ( unsigned int ii=0; ii < num; ++ii ) {
		::dpe::Paint_Job * pjob ( new ::dpe::Paint_Job );
		pjob->meta = handle_n->meta;
		pjob->img_set = handle_n->img_set;
		pjob->img_idx = ii;
		pjob->state = handle_n->state;
		_shared->enqueue_job ( pjob );
	}
	*/
}


} // End of namespace
