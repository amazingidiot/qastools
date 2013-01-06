//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "timer.hpp"
#include "timer_server.hpp"
#include <QTimerEvent>
#include <cassert>

namespace Wdg2
{

Timer::Timer (
	::Wdg2::Timer_Server * server_n ) :
_interval_msec ( 0 ),
_timer_id ( -1 ),
_server ( server_n )
{
}

Timer::~Timer ( )
{
	stop();
}

void
Timer::set_interval_msec (
	unsigned int msec_n )
{
	if ( _interval_msec != msec_n ) {
		stop();
		_interval_msec = msec_n;
	}
}

void
Timer::append_callback_idx (
	unsigned int cback_idx_n )
{
	_callbacks.append ( cback_idx_n );
	start();
}

void
Timer::remove_callback_idx (
	unsigned int cback_idx_n )
{
	_callbacks.removeOne ( cback_idx_n );
	if ( _callbacks.size() == 0 ) {
		stop();
	}
}

void
Timer::start ( )
{
	if ( _timer_id < 0 ) {
		_time.start();
		_timer_id = startTimer ( _interval_msec );
	}
}

void
Timer::stop ( )
{
	if ( _timer_id >= 0 ) {
		killTimer ( _timer_id );
		_timer_id = -1;
	}
}

void
Timer::timerEvent (
	QTimerEvent * event_n )
{
	(void) event_n;
	{
		const QTime timeout ( _time );
		_time.start(); // Restart current time
		{
			CBack_List::iterator it ( _callbacks.begin() );
			while ( it != _callbacks.end() ) {
				if ( _server->process_timeout ( *it, timeout ) ) {
					++it;
				} else {
					it = _callbacks.erase ( it );
				}
			}
		}
	}
	if ( _callbacks.size() == 0 ) {
		stop();
	}
}


} // End of namespace
