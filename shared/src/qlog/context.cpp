//
// libqlog: Text string logging infrastructure
//
// For license information check the LICENSE-libqlog.txt file 
// that comes with this source distribution.
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
