//
// libqlog: Text string logging infrastructure
//
// For license information check the LICENSE-libqlog.txt file 
// that comes with this source distribution.
//

#include "server_message.hpp"

namespace QLog
{


void
Server_Message::reset ( )
{
	if ( _server_context != 0 ) {
		_server_context->unref();
		_server_context = 0;
	}
	_core_message.reset();
}

void
Server_Message::from_message (
	const ::QLog::Message & msg_n )
{
	if ( _server_context != 0 ) {
		_server_context->unref();
	}
	_server_context = msg_n.log_context().server_context();
	_server_context->ref();

	_core_message = msg_n.core_message();
}


} // End of namespace
