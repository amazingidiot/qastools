//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012, 2013
//

#ifndef __INC_qlog_context_hpp__
#define __INC_qlog_context_hpp__

#include <QString>

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

	::QLog::Server & _log_server;
	QString _context_name;
};

inline
::QLog::Server &
Context::log_server ( ) const
{
	return _log_server;
}

inline
const QString &
Context::context_name ( ) const
{
	return _context_name;
}


} // End of namespace

#endif
