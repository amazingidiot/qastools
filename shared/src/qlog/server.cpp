//
// libqlog: Text string logging infrastructure
//
// For license information check the LICENSE-libqlog.txt file 
// that comes with this source distribution.
//

#include "server.hpp"
#include "server_context.hpp"
#include "context.hpp"
#include "message.hpp"
#include "sink.hpp"
#include <iostream>
#include <cassert>

namespace QLog
{


Server::Server ( )
{
	::pthread_rwlock_init ( &_sink_list_rwlock, 0 );
	::pthread_mutex_init ( &_context_list_mutex, 0 );
}

Server::~Server ( )
{
	destroy_sinks();
	assert ( _server_contexts.isEmpty() );

	::pthread_mutex_destroy ( &_context_list_mutex );
	::pthread_rwlock_destroy ( &_sink_list_rwlock );
}

void
Server::destroy_sinks ( )
{
	::pthread_rwlock_wrlock ( &_sink_list_rwlock );
	Sink_List::iterator it ( _sinks.begin() );
	while ( it != _sinks.end() ) {
		delete (*it);
		++it;
	}
	_sinks.clear();
	::pthread_rwlock_unlock ( &_sink_list_rwlock );
}

void
Server::add_sink (
	::QLog::Sink * sink_n )
{
	if ( sink_n != 0 ) {
		::pthread_rwlock_wrlock ( &_sink_list_rwlock );
		_sinks.push_back ( sink_n );
		::pthread_rwlock_unlock ( &_sink_list_rwlock );
	}
}

void
Server::take_sink (
	::QLog::Sink * sink_n )
{
	if ( sink_n != 0 ) {
		::pthread_rwlock_wrlock ( &_sink_list_rwlock );
		Sink_List::iterator it ( _sinks.begin() );
		while ( it != _sinks.end() ) {
			if ( (*it) == sink_n ) {
				it = _sinks.erase ( it );
			} else {
				++it;
			}
		}
		::pthread_rwlock_unlock ( &_sink_list_rwlock );
	}
}

::QLog::Server_Context *
Server::context_create (
	const QString & context_name_n,
	::QLog::Server_Context * parent_context_n )
{
	::QLog::Server_Context * res (
		new ::QLog::Server_Context ( *this, context_name_n, parent_context_n ) );

	::pthread_mutex_lock ( &_context_list_mutex );
	_server_contexts.append ( res );
	::pthread_mutex_unlock ( &_context_list_mutex );

	return res;
}

void
Server::context_destroy (
	::QLog::Server_Context * context_n )
{
	::pthread_mutex_lock ( &_context_list_mutex );
	const bool success ( _server_contexts.removeOne ( context_n ) );
	::pthread_mutex_unlock ( &_context_list_mutex );

	if ( success ) {
		delete context_n;
	}
}

void
Server::log_message (
	const ::QLog::Message & msg_n )
{
	::QLog::Server_Message srv_msg ( msg_n );

	::pthread_rwlock_rdlock ( &_sink_list_rwlock );
	{
		Sink_List::iterator it ( _sinks.begin() );
		while ( it != _sinks.end() ) {
			(*it)->log_message ( srv_msg );
			++it;
		}
	}
	::pthread_rwlock_unlock ( &_sink_list_rwlock );
}


} // End of namespace
