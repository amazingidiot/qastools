//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_dpe2_pixmap_server_hpp__
#define __INC_dpe2_pixmap_server_hpp__

#include "callbacks.hpp"
#include <QList>
#include <QScopedPointer>

// Forward declaration
namespace dpe2 {
	class Key_Values;
	class Pixmap;
	class Pixmap_Request;
	class Pixmap_Ref;
	class Pixmap_Server_Shared;
	class Painter;
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

	void
	remove_painter (
		::dpe2::Painter * painter_n );

	void
	start ( );

	void
	stop ( );

	unsigned int
	threads_running ( ) const;


	// Multithreading

	bool
	multithreading_is_safe ( ) const;

	bool
	multithread ( ) const;

	void
	set_multithread (
		bool flag_n );


	const ::Context_Callback &
	one_done_callback ( ) const;

	void
	set_one_done_callback (
		const ::Context_Callback & cb_n );

	void
	deliver_finished_requests ( );


	::dpe2::Pixmap_Request *
	acquire_request ( );

	void
	release_request (
		::dpe2::Pixmap_Request * request_n );


	/// @brief processes and waits until finished
	///
	void
	send_request (
		::dpe2::Pixmap_Request * request_n );

	/// @brief Return a pixmap
	///
	void
	return_pixmap (
		::dpe2::Pixmap_Ref & ref_n );


	// Private methods
	private:


	// Private attributes
	private:

	QScopedPointer < ::dpe2::Pixmap_Server_Shared > _shared;
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
