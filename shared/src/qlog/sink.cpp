//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012, 2013
//

#include "sink.hpp"

namespace QLog
{


Sink::Sink (
	::QLog::Log_Level limit_level_n ) :
_limit_level ( limit_level_n )
{
	::pthread_mutex_init ( &_mutex, 0 );
}

Sink::~Sink ( )
{
	::pthread_mutex_destroy ( &_mutex );
}

void
Sink::set_limit_level (
	::QLog::Log_Level limit_level_n )
{
	mutex_lock();
	_limit_level = limit_level_n;
	mutex_unlock();
}


} // End of namespace
