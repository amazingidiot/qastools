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
#include "pixmap_ref.hpp"
#include <QTime>

#include <iostream>
#include <cassert>


namespace dpe2
{


Pixmap_Server::Pixmap_Server ( )
{
	_shared.reset ( new ::dpe2::Pixmap_Server_Shared );
	_multithread = multithreading_is_safe();
}

Pixmap_Server::~Pixmap_Server ( )
{
	stop();
}

void
Pixmap_Server::install_painter (
	::dpe2::Painter * painter_n )
{
	if ( painter_n != 0 ) {
		_shared->install_painter ( painter_n );
	}
}

void
Pixmap_Server::start ( )
{
	if ( multithread() ) {
		_shared->start_threads();
	}
}

void
Pixmap_Server::stop ( )
{
	if ( multithread() ) {
		_shared->stop_threads();
	}
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
			stop();
		}
		_multithread = flag_n;
	}
}

const ::Context_Callback &
Pixmap_Server::one_done_callback ( ) const
{
	return _shared->one_done_callback();
}

void
Pixmap_Server::set_one_done_callback (
	const ::Context_Callback & cb_n )
{
	_shared->set_one_done_callback ( cb_n );
}

::dpe2::Pixmap_Request *
Pixmap_Server::acquire_request ( )
{
	return _shared->create_request();
}

void
Pixmap_Server::release_request (
	::dpe2::Pixmap_Request * request_n )
{
	_shared->release_request ( request_n );
}

void
Pixmap_Server::send_request (
	::dpe2::Pixmap_Request * request_n )
{
	_shared->send_request ( request_n );
}

void
Pixmap_Server::deliver_finished_requests ( )
{
	_shared->deliver_finished_requests();
}

void
Pixmap_Server::return_pixmap (
	::dpe2::Pixmap_Ref & ref_n )
{
	if ( ref_n.iref1() != 0 ) {
		::dpe2::Painter * pnt ( ref_n.iref1()->iref0()->painter() );
		pnt->iref1_deref ( ref_n.iref1() );
	}
	ref_n.clear();
}


} // End of namespace
