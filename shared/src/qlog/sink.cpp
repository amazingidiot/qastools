//
// libqlog: Text string logging infrastructure
//
// For license information check the LICENSE-libqlog.txt file 
// that comes with this source distribution.
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
