//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012, 2013
//

#ifndef __INC_qlog_server_context_hpp__
#define __INC_qlog_server_context_hpp__

#include <QString>
#include <QAtomicInt>

// Forward declarations
namespace QLog
{
	class Server;
	class Message;
}

namespace QLog
{


/// @brief Server_Context
///
class Server_Context
{
	// Public methods
	public:

	Server_Context (
		::QLog::Server & log_server_n,
		const QString & context_name_n = QString(),
		::QLog::Server_Context * parent_n = 0 );

	~Server_Context ( );


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


	void
	ref ( );

	void
	unref ( );


	// Private attributes
	private:

	QString _context_name;
	::QLog::Server & _log_server;
	::QLog::Server_Context * _context_parent;
	QAtomicInt _num_users;
};

inline
::QLog::Server &
Server_Context::log_server ( ) const
{
	return _log_server;
}

inline
const QString &
Server_Context::context_name ( ) const
{
	return _context_name;
}

inline
void
Server_Context::ref ( )
{
	_num_users.ref();
}


} // End of namespace

#endif
