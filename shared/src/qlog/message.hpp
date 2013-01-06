//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012, 2013
//

#ifndef __INC_qlog_message_hpp__
#define __INC_qlog_message_hpp__

#include "context.hpp"
#include "core_message.hpp"

namespace QLog
{


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

	const ::QLog::Core_Message &
	core_message ( ) const;


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
	text ( );

	const QString &
	text ( ) const;


	void
	send ( );


	// Private attributes
	private:

	::QLog::Context & _log_context;
	::QLog::Core_Message _core_message;
};

inline
Message::Message (
	::QLog::Context & context_n,
	::QLog::Log_Level level_n ) :
_log_context ( context_n ),
_core_message ( level_n )
{
}

inline
Message::Message (
	::QLog::Context & context_n,
	const QString & message_n,
	::QLog::Log_Level level_n ) :
_log_context ( context_n ),
_core_message ( message_n, level_n )
{
}

inline
Message::~Message ( )
{
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
	return _core_message.log_level();
}

inline
void
Message::set_log_level (
	::QLog::Log_Level level_n )
{
	_core_message.set_log_level ( level_n );
}

inline
void
Message::append (
	const char * str_n )
{
	_core_message.text().append ( str_n );
}

inline
void
Message::append (
	const QString & str_n )
{
	_core_message.text().append ( str_n );
}

inline
void
Message::append (
	const ::std::string & str_n )
{
	_core_message.text().append ( str_n.c_str() );
}

inline
QString &
Message::text ( )
{
	return _core_message.text();
}

inline
const QString &
Message::text ( ) const
{
	return _core_message.text();
}

inline
void
Message::send ( )
{
	log_context().log_message ( *this );
}

inline
const ::QLog::Core_Message &
Message::core_message ( ) const
{
	return _core_message;
}


/// @brief Message_Sender
///
/// Sends the message on destruction.
class Message_Sender :
	public ::QLog::Message
{
	// Public methods
	public:

	Message_Sender (
		::QLog::Context & context_n,
		::QLog::Log_Level level_n = ::QLog::LOG_LEVEL_INFO );

	Message_Sender (
		::QLog::Context & context_n,
		const QString & message_n,
		::QLog::Log_Level level_n = ::QLog::LOG_LEVEL_INFO );

	~Message_Sender ( );
};

inline
Message_Sender::Message_Sender (
	::QLog::Context & context_n,
	::QLog::Log_Level level_n ) :
::QLog::Message ( context_n, level_n )
{
}

inline
Message_Sender::Message_Sender (
	::QLog::Context & context_n,
	const QString & message_n,
	::QLog::Log_Level level_n ) :
::QLog::Message ( context_n, message_n, level_n )
{
}

inline
Message_Sender::~Message_Sender ( )
{
	// Send on scope end / destruction
	send();
}


} // End of namespace

#endif
