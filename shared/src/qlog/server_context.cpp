//
// libqlog: Text string logging infrastructure
//
// For license information check the LICENSE-libqlog.txt file 
// that comes with this source distribution.
//

#include "server_context.hpp"
#include "server.hpp"

namespace QLog
{


Server_Context::Server_Context (
	::QLog::Server & log_server_n,
	const QString & context_name_n,
	::QLog::Server_Context * parent_n ) :
_context_name ( context_name_n ),
_log_server ( log_server_n ),
_context_parent ( parent_n )
{
}

Server_Context::~Server_Context ( )
{
}

void
Server_Context::set_context_name (
	const QString & name_n )
{
	_context_name = name_n;
}

void
Server_Context::log_message (
	const ::QLog::Message & msg_n )
{
	log_server().log_message ( msg_n );
}

void
Server_Context::unref ( )
{
	if ( !_num_users.deref() ) {
		log_server().context_destroy ( this );
	}
}


} // End of namespace
