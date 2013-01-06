//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012, 2013
//

#ifndef __INC_qlog_server_hpp__
#define __INC_qlog_server_hpp__

#include <QString>
#include <QList>
#include <pthread.h>

// Forward declarations
namespace QLog
{
	class Server_Context;
	class Context;
	class Message;
	class Sink;
}

namespace QLog
{


/// @brief Central logging server
///
class Server
{
	// Public methods
	public:

	Server ( );

	~Server ( );


	void
	add_sink (
		::QLog::Sink * sink_n );

	void
	take_sink (
		::QLog::Sink * sink_n );

	void
	log_message (
		const ::QLog::Message & msg_n );


	::QLog::Server_Context *
	context_create (
		const QString & context_name_n = QString(),
		::QLog::Server_Context * parent_context_n = 0 );

	void
	context_destroy (
		::QLog::Server_Context * context_n );


	// Private methods
	private:

	void
	destroy_sinks ( );


	// Private attributes
	private:

	typedef QList < ::QLog::Sink * > Sink_List;
	typedef QList < ::QLog::Server_Context * > Context_List;

	pthread_rwlock_t _sink_list_rwlock;
	Sink_List _sinks;

	pthread_mutex_t _context_list_mutex;
	Context_List _server_contexts;
};


} // End of namespace

#endif
