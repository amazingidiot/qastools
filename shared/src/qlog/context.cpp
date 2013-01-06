//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012, 2013
//

#include "context.hpp"
#include "server.hpp"

namespace QLog
{


Context::Context (
	::QLog::Server & log_server_n,
	const QString & context_name_n ) :
_log_server ( log_server_n ),
_context_name ( context_name_n )
{
}

Context::Context (
	const ::QLog::Context & parent_n,
	const QString & context_name_n ) :
_log_server ( parent_n.log_server() ),
_context_name ( context_name_n )
{
}

Context::~Context ( )
{
}

void
Context::set_context_name (
	const QString & name_n )
{
	_context_name = name_n;
}

void
Context::log_message (
	const ::QLog::Message & msg_n )
{
	log_server().log_message ( msg_n );
}


} // End of namespace
