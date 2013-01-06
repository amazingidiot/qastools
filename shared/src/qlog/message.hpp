//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012, 2013
//

#ifndef __INC_qlog_message_hpp__
#define __INC_qlog_message_hpp__

#include "context.hpp"
#include <QString>

namespace QLog
{


enum Log_Level
{
	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_INFO,
	LOG_LEVEL_PROGRESS,
	LOG_LEVEL_DEBUG_0,
	LOG_LEVEL_DEBUG_1,
	LOG_LEVEL_DEBUG_2,
};


/// @brief Message
///
class Message
{
	// Public methods
	public:

	Message (
		::QLog::Context & context_n,
		::QLog::Log_Level level_n = ::QLog::LOG_LEVEL_INFO );

	Message (
		::QLog::Context & context_n,
		const QString & message_n,
		::QLog::Log_Level level_n = ::QLog::LOG_LEVEL_INFO );

	~Message ( );


	::QLog::Context &
	log_context ( );

	const ::QLog::Context &
	log_context ( ) const;


	::QLog::Log_Level
	log_level ( ) const;

	void
	set_log_level (
		::QLog::Log_Level level_n );


	void
	append (
		const char * str_n );

	void
	append (
		const QString & str_n );

	void
	append (
		const ::std::string & str_n );

	QString &
	message ( );

	const QString &
	message ( ) const;


	// Protected methods
	protected:


	// Private attributes
	private:

	::QLog::Context & _log_context;
	QString _message;
	::QLog::Log_Level _log_level;
};

inline
Message::Message (
	::QLog::Context & context_n,
	::QLog::Log_Level level_n ) :
_log_context ( context_n ),
_log_level ( level_n )
{
}

inline
Message::Message (
	::QLog::Context & context_n,
	const QString & message_n,
	::QLog::Log_Level level_n ) :
_log_context ( context_n ),
_message ( message_n ),
_log_level ( level_n )
{
}

inline
Message::~Message ( )
{
	log_context().log_message ( *this );
}

inline
::QLog::Context &
Message::log_context ( )
{
	return _log_context;
}

inline
const ::QLog::Context &
Message::log_context ( ) const
{
	return _log_context;
}

inline
::QLog::Log_Level
Message::log_level ( ) const
{
	return _log_level;
}

inline
void
Message::set_log_level (
	::QLog::Log_Level level_n )
{
	_log_level = level_n;
}

inline
void
Message::append (
	const char * str_n )
{
	message().append ( str_n );
}

inline
void
Message::append (
	const QString & str_n )
{
	message().append ( str_n );
}

inline
void
Message::append (
	const ::std::string & str_n )
{
	message().append ( str_n.c_str() );
}

inline
QString &
Message::message ( )
{
	return _message;
}

inline
const QString &
Message::message ( ) const
{
	return _message;
}


} // End of namespace

#endif
