//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012, 2013
//

#include "context.hpp"
#include "server.hpp"

namespace QLog
{


Context::Context (
	::QLog::Server & log_server_n,
	const QString & context_name_n )
{
	_server_context = log_server_n.context_create ( context_name_n );
	_server_context->ref();
}

Context::Context (
	const ::QLog::Context & parent_n,
	const QString & context_name_n )
{
	_server_context = parent_n.log_server().context_create (
		context_name_n,
		parent_n._server_context );
	_server_context->ref();
}

Context::~Context ( )
{
	_server_context->unref();
}


} // End of namespace
