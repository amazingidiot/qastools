//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_dpe2_pixmap_server_shared_hpp__
#define __INC_dpe2_pixmap_server_shared_hpp__

#include "painter.hpp"
#include "paint_thread.hpp"
#include "callbacks.hpp"
#include <QScopedPointer>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>

// Forward declaration
namespace dpe2 {
	class Pixmap;
	class Pixmap_Request;
	class Painter;
}


namespace dpe2
{

// Type definitions
typedef QList < ::dpe2::Pixmap_Request * > Request_List;
typedef QQueue < ::dpe2::Pixmap_Request * > Request_Queue;


/// @brief Multiple equal request can wait here for a painting to be finished
///
struct Pixmap_Paint_Waiter
{
	Request_Queue requests;
};


/// @brief Pixmap_Server_Shared
///
/// Paint_Thread instances have access to this class
/// as well as the Pixmap_Server instance
class Pixmap_Server_Shared
{
	// Public methods
	public:

	Pixmap_Server_Shared ( );

	~Pixmap_Server_Shared ( );


	// User interface

	void
	install_painter (
		::dpe2::Painter * painter_n );

	void
	start_threads ( );

	unsigned int
	threads_running ( ) const;

	void
	abort_threads ( );

	void
	join_threads ( );

	void
	stop_threads ( );



	::dpe2::Pixmap_Request *
	acquire_request ( );

	void
	release_request (
		::dpe2::Pixmap_Request * request_n );


	void
	send_request (
		::dpe2::Pixmap_Request * request_n );

	void
	process_request (
		::dpe2::Pixmap_Request * request_n );


	const ::Context_Callback &
	one_done_callback ( ) const;

	void
	set_one_done_callback (
		const ::Context_Callback & cb_n );

	void
	deliver_finished_requests ( );


	// Threads interface

	::dpe2::Pixmap_Request *
	fetch_request ( );


	// Private methods;
	private:

	::dpe2::Pixmap_Request *
	create_request ( );

	void
	destroy_request (
		::dpe2::Pixmap_Request * request_n );

	bool
	destroy_request_on_demand (
		::dpe2::Pixmap_Request * request_n );


	void
	enqueue_request (
		::dpe2::Pixmap_Request * request_n );

	void
	paint_request (
		::dpe2::Painter & pnt_n,
		::dpe2::Pixmap_Request * request_n );

	void
	request_finished_begin ( );

	void
	request_finished (
		::dpe2::Pixmap_Request * request_n );

	void
	request_finished_end ( );


	::dpe2::Pixmap_Paint_Waiter *
	acquire_waiter ( );

	void
	release_waiter (
		::dpe2::Pixmap_Paint_Waiter * waiter_n );


	// Private attributes;
	private:

	typedef QList < ::dpe2::Painter * > Painter_List;
	typedef QList < ::dpe2::Paint_Thread * > Thread_List;
	typedef QList < ::dpe2::Pixmap_Paint_Waiter * > Paint_Waiters_List;

	QMutex _queue_new_mutex;
	Request_Queue _queue_new;
	QWaitCondition _queue_new_cond;

	QMutex _queue_done_mutex;
	bool _queue_done_notified;
	Request_Queue _queue_done[2];
	Request_List _released_requests;
	::Context_Callback _done_callback;

	// Painters
	Painter_List _painters;
	Thread_List _threads;

	QMutex _shared_mutex;
	Paint_Waiters_List _paint_waiters;
};


inline
const ::Context_Callback &
Pixmap_Server_Shared::one_done_callback ( ) const
{
	return _done_callback;
}

inline
unsigned int
Pixmap_Server_Shared::threads_running ( ) const
{
	return _threads.size();
}


} // End of namespace

#endif
