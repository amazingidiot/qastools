//
// libqlog: Text string logging infrastructure
//
// For license information check the LICENSE-libqlog.txt file
// that comes with this source distribution.
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


	virtual
	void
	log_message (
		const ::QLog::Server_Message & msg_n );


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


/// @brief Sink_StdOut
///
class Sink_StdOut :
	public ::QLog::Sink_OStream
{
	// Public methods
	public:

	Sink_StdOut (
		::QLog::Log_Level limit_level_n = ::QLog::LOG_LEVEL_INFO );
};


/// @brief Sink_File
///
class Sink_File :
	public ::QLog::Sink_OStream
{
	// Public methods
	public:

	Sink_File (
		::QLog::Log_Level limit_level_n = ::QLog::LOG_LEVEL_INFO );
};


} // End of namespace

#endif
