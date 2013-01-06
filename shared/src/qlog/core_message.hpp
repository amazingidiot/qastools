//
// libqlog: Text string logging infrastructure
//
// For license information check the LICENSE-libqlog.txt file 
// that comes with this source distribution.
//

#ifndef __INC_qlog_core_message_hpp__
#define __INC_qlog_core_message_hpp__

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


/// @brief Core_Message
///
class Core_Message
{
	// Public methods
	public:

	Core_Message (
		::QLog::Log_Level level_n = ::QLog::LOG_LEVEL_INFO );

	Core_Message (
		const QString & text_n,
		::QLog::Log_Level level_n = ::QLog::LOG_LEVEL_INFO );

	~Core_Message ( );


	void
	reset ( );


	::QLog::Log_Level
	log_level ( ) const;

	void
	set_log_level (
		::QLog::Log_Level level_n );


	QString &
	text ( );

	const QString &
	text ( ) const;

	void
	set_text (
		const QString & text_n );


	// Protected methods
	protected:


	// Private attributes
	private:

	QString _text;
	::QLog::Log_Level _log_level;
};

inline
Core_Message::Core_Message (
	::QLog::Log_Level level_n ) :
_log_level ( level_n )
{
}

inline
Core_Message::Core_Message (
	const QString & text_n,
	::QLog::Log_Level level_n ) :
_text ( text_n ),
_log_level ( level_n )
{
}

inline
Core_Message::~Core_Message ( )
{
}

inline
void
Core_Message::reset ( )
{
	_text.clear();
	_log_level = ::QLog::LOG_LEVEL_INFO;
}

inline
::QLog::Log_Level
Core_Message::log_level ( ) const
{
	return _log_level;
}

inline
void
Core_Message::set_log_level (
	::QLog::Log_Level level_n )
{
	_log_level = level_n;
}

inline
QString &
Core_Message::text ( )
{
	return _text;
}

inline
const QString &
Core_Message::text ( ) const
{
	return _text;
}

inline
void
Core_Message::set_text (
	const QString & text_n )
{
	_text = text_n;
}


} // End of namespace

#endif
