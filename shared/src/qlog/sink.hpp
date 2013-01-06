//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012, 2013
//

#ifndef __INC_qlog_sink_hpp__
#define __INC_qlog_sink_hpp__

#include "server_message.hpp"
#include <pthread.h>

namespace QLog
{


/// @brief Sink
///
class Sink
{
	// Public methods
	public:

	Sink (
		::QLog::Log_Level limit_level_n = ::QLog::LOG_LEVEL_INFO );

	virtual
	~Sink ( );


	::QLog::Log_Level
	limit_level ( ) const;

	void
	set_limit_level (
		::QLog::Log_Level limit_level_n );


	virtual
	void
	log_message (
		const ::QLog::Server_Message & msg_n ) = 0;


	// Protected methods
	protected:

	void
	mutex_lock ( );

	void
	mutex_unlock ( );


	// Private attributes
	private:

	::QLog::Log_Level _limit_level;
	::pthread_mutex_t _mutex;
};

inline
::QLog::Log_Level
Sink::limit_level ( ) const
{
	return _limit_level;
}

inline
void
Sink::mutex_lock ( )
{
	::pthread_mutex_lock ( &_mutex );
}

inline
void
Sink::mutex_unlock ( )
{
	::pthread_mutex_unlock ( &_mutex );
}


} // End of namespace

#endif
