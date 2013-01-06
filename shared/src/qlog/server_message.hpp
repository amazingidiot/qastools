//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012, 2013
//

#ifndef __INC_qlog_server_message_hpp__
#define __INC_qlog_server_message_hpp__

#include "message.hpp"
#include "server_context.hpp"
#include <QString>

namespace QLog
{


/// @brief Server_Message
///
class Server_Message
{
	// Public methods
	public:

	Server_Message ( );

	Server_Message (
		const ::QLog::Message & msg_n );

	~Server_Message ( );


	void
	reset ( );

	void
	from_message (
		const ::QLog::Message & msg_n );


	::QLog::Server_Context *
	server_context ( ) const;

	const ::QLog::Core_Message &
	core_message ( ) const;


	// Private attributes
	private:

	::QLog::Server_Context * _server_context;
	::QLog::Core_Message _core_message;
};

inline
Server_Message::Server_Message ( ) :
_server_context ( 0 )
{
}

inline
Server_Message::Server_Message (
	const ::QLog::Message & msg_n ) :
_server_context ( msg_n.log_context().server_context() ),
_core_message ( msg_n.core_message() )
{
	_server_context->ref();
}

inline
Server_Message::~Server_Message ( )
{
	if ( _server_context != 0 ) {
		_server_context->unref();
	}
}

inline
::QLog::Server_Context *
Server_Message::server_context ( ) const
{
	return _server_context;
}

inline
const ::QLog::Core_Message &
Server_Message::core_message ( ) const
{
	return _core_message;
}


} // End of namespace

#endif
