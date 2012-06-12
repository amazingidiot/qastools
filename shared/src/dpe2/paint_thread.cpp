//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "paint_thread.hpp"
#include <iostream>

namespace dpe2
{


Paint_Thread_Shared::Paint_Thread_Shared ( )
{
	// TODO
	//painter_def.reset ( new ::dpe2::Painter_Simple );
	num_rendering = 0;
	_finished = false;
}

Paint_Thread_Shared::~Paint_Thread_Shared ( )
{
	for ( int ii=0; ii < painters.size(); ++ii ) {
		delete painters[ii];
	}
	painters.clear();
}
/*
void
Paint_Thread_Shared::enqueue_job (
	::dpe::Paint_Job * pjob_n )
{
	_queue_mutex.lock();
	_finished = false;
	_queue.enqueue ( pjob_n );
	_queue_mutex.unlock();
	_queue_cond.wakeOne();
}

::dpe::Paint_Job *
Paint_Thread_Shared::fetch_job ( )
{
	::dpe::Paint_Job * pjob;

	_queue_mutex.lock();
	while ( _queue.size() == 0 ) {
		_queue_cond.wait ( &_queue_mutex );
	}
	pjob = _queue.dequeue();
	_queue_mutex.unlock();

	return pjob;
}

void
Paint_Thread_Shared::job_finished (
	::dpe::Paint_Job * pjob_n )
{
	if ( !pjob_n->state->one_done() ) {
		if ( !num_rendering.deref() ) {
			bool do_wake ( false );

			_queue_mutex.lock();
			if ( _queue.size() == 0 ) {
				_finished = true;
				do_wake = true;
			}
			_queue_mutex.unlock();

			if ( do_wake ) {
				_finished_cond.wakeAll();
			}
		}
	}
	delete pjob_n;
}

void
Paint_Thread_Shared::wait_for_finish ( )
{
	_queue_mutex.lock();
	while ( !_finished ) {
		_finished_cond.wait ( &_queue_mutex );
	}
	_queue_mutex.unlock();
}


int
Paint_Thread_Shared::paint_job (
	::dpe::Paint_Job * pjob_n )
{
	// Find painter and paint
	::dpe::Painter * pnt ( find_painter ( pjob_n->meta ) );
	return pnt->check_and_paint ( pjob_n );
}


::dpe::Painter *
Paint_Thread_Shared::find_painter (
	::dpe::Image_Set_Meta * meta_n )
{
	::dpe::Painter * pnt ( 0 );

	{
		const unsigned int group_type ( meta_n->group_type );
		unsigned int group_variant ( meta_n->group_variant );

		while ( true ) {
			// Search painter
			for ( int ii=0; ii < painters.size(); ++ii ) {
				::dpe::Painter * pcur ( painters[ii] );
				if (
					( pcur->group_type() == group_type ) &&
					( pcur->group_variant() == group_variant ) )
				{
					pnt = pcur;
					break;
				}
			}

			if ( pnt != 0 ) {
				break;
			} else {
				// Search again with default variant on demand
				if ( group_variant == 0 ) {
					pnt = painter_def;
					break;
				} else {
					group_variant = 0;
				}
			}
		}
	}

	return pnt;
}

void
Paint_Thread_Shared::abort_threads (
	unsigned int num_n )
{
	_queue_mutex.lock();
	for ( unsigned int ii = 0; ii < num_n; ++ii ) {
		_queue.append ( 0 );
	}
	_queue_mutex.unlock();
	_queue_cond.wakeAll();
}
*/


Paint_Thread::Paint_Thread (
	::dpe2::Paint_Thread_Shared & shared_n ) :
_shared ( shared_n )
{
}

void
Paint_Thread::run ( )
{
	/* TODO
	while ( true ) {
		::dpe::Paint_Job * pjob ( _shared.fetch_job() );

		if ( pjob == 0 ) {
			// Exit loop on invalid job
			break;
		}

		_shared.paint_job ( pjob );
		_shared.job_finished ( pjob );
	}
	*/
}


} // End of namespace

