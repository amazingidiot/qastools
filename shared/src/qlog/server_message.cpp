//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012, 2013
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
