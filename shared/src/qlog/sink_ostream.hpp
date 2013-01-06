//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012, 2013
//

#ifndef __INC_qlog_sink_ostream_hpp__
#define __INC_qlog_sink_ostream_hpp__

#include "sink.hpp"
#include <ostream>

namespace QLog
{


/// @brief Sink_OStream
///
class Sink_OStream :
	public ::QLog::Sink
{
	// Public methods
	public:

	Sink_OStream (
		::std::ostream * ostream_n = 0,
		::QLog::Log_Level limit_level_n = ::QLog::LOG_LEVEL_INFO );

	virtual
	~Sink_OStream ( );


	::std::ostream *
	ostream ( ) const;

	void
	set_ostream (
		::std::ostream * ostr_n );


	virtual
	void
	log_message (
		const ::QLog::Message & msg_n );


	// Private attributes
	private:

	::std::ostream * _ostream;
};

inline
::std::ostream *
Sink_OStream::ostream ( ) const
{
	return _ostream;
}


/// @brief Log_Sink_StdOut
///
class Log_Sink_StdOut :
	public ::QLog::Sink_OStream
{
	// Public methods
	public:

	Log_Sink_StdOut (
		::QLog::Log_Level limit_level_n = ::QLog::LOG_LEVEL_INFO );
};


/// @brief Log_Sink_File
///
class Log_Sink_File :
	public ::QLog::Sink_OStream
{
	// Public methods
	public:

	Log_Sink_File (
		::QLog::Log_Level limit_level_n = ::QLog::LOG_LEVEL_INFO );
};


} // End of namespace

#endif
