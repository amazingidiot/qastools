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
#include <vector>
#include <pthread.h>

// Forward declarations
namespace QLog
{
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


	// Private methods
	private:

	void
	destroy_sinks ( );


	// Private attributes
	private:

	typedef ::std::vector < ::QLog::Sink * > Sink_List;

	pthread_rwlock_t _sink_list_rwlock;
	Sink_List _log_sinks;
};


} // End of namespace

#endif
