//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012, 2013
//

#include "sink_ostream.hpp"
#include <iostream>

namespace QLog
{


Sink_OStream::Sink_OStream (
	::std::ostream * ostream_n,
	::QLog::Log_Level limit_level_n ) :
::QLog::Sink ( limit_level_n ),
_ostream ( ostream_n )
{
}

Sink_OStream::~Sink_OStream ( )
{
}

void
Sink_OStream::set_ostream (
	::std::ostream * ostr_n )
{
	mutex_lock();
	_ostream = ostr_n;
	mutex_unlock();
}

void
Sink_OStream::log_message (
	const ::QLog::Message & msg_n )
{
	QByteArray barr;
	// Compose text string
	{
		QString txt;
		if ( !msg_n.log_context().context_name().isEmpty() ) {
			txt.append ( msg_n.log_context().context_name() );
			txt.append ( ": " );
		}
		txt.append ( msg_n.message() );
		barr = txt.toLocal8Bit();
	}

	mutex_lock();
	if ( ( ostream() != 0 ) && ( msg_n.log_level() <= limit_level() ) ) {
		ostream()->write ( barr.constData(), barr.size() );
	}
	mutex_unlock();
}


Log_Sink_StdOut::Log_Sink_StdOut (
	::QLog::Log_Level limit_level_n ) :
::QLog::Sink_OStream ( &::std::cout, limit_level_n )
{
}


} // End of namespace
