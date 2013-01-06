//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012, 2013
//

#ifndef __INC_qlog_context_hpp__
#define __INC_qlog_context_hpp__

#include "server_context.hpp"

// Forward declarations
namespace QLog
{
	class Server;
	class Message;
}

namespace QLog
{


/// @brief Context
///
class Context
{
	// Public methods
	public:

	Context (
		::QLog::Server & log_server_n,
		const QString & context_name_n = QString() );

	Context (
		const ::QLog::Context & parent_n,
		const QString & context_name_n = QString() );

	~Context ( );


	::QLog::Server_Context *
	server_context ( ) const;

	::QLog::Server &
	log_server ( ) const;


	const QString &
	context_name ( ) const;

	void
	set_context_name (
		const QString & name_n );


	void
	log_message (
		const ::QLog::Message & msg_n );


	// Private attributes
	private:

	::QLog::Server_Context * _server_context;
};

inline
::QLog::Server_Context *
Context::server_context ( ) const
{
	return _server_context;
}

inline
::QLog::Server &
Context::log_server ( ) const
{
	return server_context()->log_server();
}

inline
const QString &
Context::context_name ( ) const
{
	return server_context()->context_name();
}

inline
void
Context::set_context_name (
	const QString & name_n )
{
	server_context()->set_context_name ( name_n );
}

inline
void
Context::log_message (
	const ::QLog::Message & msg_n )
{
	server_context()->log_message ( msg_n );
}


} // End of namespace

#endif
