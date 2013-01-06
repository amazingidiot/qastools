//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012, 2013
//

#include "server.hpp"
#include "context.hpp"
#include "message.hpp"
#include "sink.hpp"
#include <iostream>

namespace QLog
{


Server::Server ( )
{
	::pthread_rwlock_init ( &_sink_list_rwlock, 0 );
}

Server::~Server ( )
{
	destroy_sinks();
	::pthread_rwlock_destroy ( &_sink_list_rwlock );
}

void
Server::destroy_sinks ( )
{
	::pthread_rwlock_wrlock ( &_sink_list_rwlock );
	Sink_List::iterator it ( _log_sinks.begin() );
	while ( it != _log_sinks.end() ) {
		delete (*it);
		++it;
	}
	_log_sinks.clear();
	::pthread_rwlock_unlock ( &_sink_list_rwlock );
}

void
Server::add_sink (
	::QLog::Sink * sink_n )
{
	if ( sink_n != 0 ) {
		::pthread_rwlock_wrlock ( &_sink_list_rwlock );
		_log_sinks.push_back ( sink_n );
		::pthread_rwlock_unlock ( &_sink_list_rwlock );
	}
}

void
Server::take_sink (
	::QLog::Sink * sink_n )
{
	if ( sink_n != 0 ) {
		::pthread_rwlock_wrlock ( &_sink_list_rwlock );
		Sink_List::iterator it ( _log_sinks.begin() );
		while ( it != _log_sinks.end() ) {
			if ( (*it) == sink_n ) {
				it = _log_sinks.erase ( it );
			} else {
				++it;
			}
		}
		::pthread_rwlock_unlock ( &_sink_list_rwlock );
	}
}

void
Server::log_message (
	const ::QLog::Message & msg_n )
{
	::pthread_rwlock_rdlock ( &_sink_list_rwlock );
	Sink_List::iterator it ( _log_sinks.begin() );
	while ( it != _log_sinks.end() ) {
		(*it)->log_message ( msg_n );
		++it;
	}
	::pthread_rwlock_unlock ( &_sink_list_rwlock );
}


} // End of namespace
