//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_dpe2_pixmap_server_hpp__
#define __INC_dpe2_pixmap_server_hpp__

#include <QList>
#include <QScopedPointer>

// Forward declaration
namespace dpe2 {
	class Pixmap;
	class Pixmap_Request;
	class Pixmap_Handle;
	class Painter;
	class Paint_Thread_Shared;
	class Paint_Thread;
}


namespace dpe2
{


/// @brief Pixmap_Server
///
class Pixmap_Server
{
	// Public methods
	public:

	Pixmap_Server ( );

	~Pixmap_Server ( );


	void
	install_painter (
		::dpe2::Painter * painter_n );


	// Multithreading

	bool
	multithreading_is_safe ( ) const;

	bool
	multithread ( ) const;

	void
	set_multithread (
		bool flag_n );


	/// @brief processes and waits until finished
	///
	void
	send_request (
		::dpe2::Pixmap_Request * request_n );

	/// @brief just enqueues the request and returns
	///
	void
	post_request (
		::dpe2::Pixmap_Request * request_n );


	// Private methods
	private:

	void
	start_threads ( );

	void
	stop_threads ( );

	void
	process_handle (
		::dpe2::Pixmap_Handle * handle_n );

	void
	render_handle (
		::dpe2::Pixmap_Handle * handle_n );

	void
	enqueue_handle (
		::dpe2::Pixmap_Handle * handle_n );


	// Private attributes
	private:

	QList < ::dpe2::Pixmap_Handle * > _pixmap_handles;
	QScopedPointer < ::dpe2::Paint_Thread_Shared > _shared;
	QList < ::dpe2::Paint_Thread * > _threads;
	bool _multithread;
};


inline
bool
Pixmap_Server::multithread ( ) const
{
	return _multithread;
}


} // End of namespace

#endif
