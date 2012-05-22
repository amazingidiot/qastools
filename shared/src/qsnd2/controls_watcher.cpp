//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "controls_watcher.hpp"

namespace QSnd2
{


Controls_Watcher::Controls_Watcher (
	::QSnd2::Controls * ctl_n ) :
_controls ( ctl_n )
{
	if ( _controls != 0 ) {
		create_socket_nofifiers();
	}
}

Controls_Watcher::~Controls_Watcher ( )
{
	set_controls ( 0 );
}

void
Controls_Watcher::set_controls (
	::QSnd2::Controls * ctl_n )
{
	if ( _controls != ctl_n ) {
		clear_socket_notifiers();

		_controls = ctl_n;

		if ( _controls != 0 ) {
			create_socket_nofifiers();
		}
	}
}

void
Controls_Watcher::clear_socket_notifiers ( )
{
	if ( _socket_notifiers.size() > 0 ) {
		for ( int ii=0; ii < _socket_notifiers.size(); ++ii ) {
			delete _socket_notifiers[ii];
		}
		_socket_notifiers.clear();
	}
}

void
Controls_Watcher::create_socket_nofifiers ( )
{
	if ( _controls != 0 ) {
		clear_socket_notifiers();

		for ( unsigned int ii=0; ii < _controls->num_pollfds(); ++ii ) {
			int fd ( _controls->pollfds()[ii].fd );
			if ( fd != 0 ) {
				QSocketNotifier * snot (
					new QSocketNotifier ( fd, QSocketNotifier::Read ) );

				connect ( snot, SIGNAL ( activated ( int ) ),
					this, SLOT ( socket_data ( int ) ) );

				_socket_notifiers.append ( snot );
			}
		}
	}
}

void
Controls_Watcher::socket_data (
	int socket_id_n )
{
	_controls->socket_data ( socket_id_n );
}


} // End of namespace
