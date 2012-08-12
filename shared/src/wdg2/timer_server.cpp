//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "timer_server.hpp"
#include "timer.hpp"
#include <cassert>

namespace Wdg2
{


//
// Timer_Callback
//

Timer_Callback::Timer_Callback ( ) :
_msec_remain ( 0 ),
_timer_idx ( 0 ),
_is_used ( false ),
_is_running ( false ),
_is_repeating ( false )
{
}


//
// Timer_Server
//

Timer_Server::Timer_Server ( )
{
}

Timer_Server::~Timer_Server ( )
{
	// Remove all timers
	for ( int ii=0; ii < _timers.size(); ++ii ) {
		destroy_timer_obj ( _timers[ii] );
	}
}

::Wdg2::Timer *
Timer_Server::create_timer_obj ( )
{
	::Wdg2::Timer * res ( new ::Wdg2::Timer ( this ) );
	return res;
}

void
Timer_Server::destroy_timer_obj (
	::Wdg2::Timer * timer_n )
{
	assert ( timer_n->num_callbacks() == 0 );
	delete timer_n;
}

/// @brief Creates a timer and returns its id
unsigned int
Timer_Server::create_timer (
	unsigned int interval_msec_n )
{
	{
		::Wdg2::Timer * timer ( create_timer_obj ( ) );
		timer->set_interval_msec ( interval_msec_n );
		_timers.append ( timer );
	}
	return _timers.size();
}

void
Timer_Server::destroy_timer (
	unsigned int timer_id_n )
{
	if ( timer_id_n != 0 ) {
		--timer_id_n;
		if ( (int)timer_id_n < _timers.size() ) {
			::Wdg2::Timer * timer ( _timers[timer_id_n] );
			if ( timer != 0 ) {
				destroy_timer_obj ( timer );
				_timers[timer_id_n] = 0;
			}
		}
	}
}

unsigned int
Timer_Server::aquire_callback (
	unsigned int timer_id_n )
{
	unsigned int res ( 0 );
	::Wdg2::Timer * timer ( find_timer ( timer_id_n ) );
	if ( timer != 0 ) {
		// Find existing free callback
		for ( unsigned int ii=0; ii < _callbacks.size(); ++ii ) {
			if ( !_callbacks[ii].is_used() ) {
				res = ii+1;
				break;
			}
		}
		// Create new callback on demand
		if ( res == 0 ) {
			_callbacks.push_back ( ::Wdg2::Timer_Callback() );
			res = _callbacks.size();
		}
		{
			::Wdg2::Timer_Callback & cback ( _callbacks[res-1] );
			cback.set_is_used ( true );
			cback.set_timer_idx ( timer_id_n-1 );
		}
		timer->append_callback_idx ( res-1 );
	}
	return res;
}

void
Timer_Server::release_callback (
	unsigned int cback_id_n )
{
	::Wdg2::Timer_Callback * cback ( find_callback ( cback_id_n ) );
	if ( cback != 0 ) {
		if ( cback->is_used() ) {
			if ( cback->is_running() ) {
				cback_abort_request ( cback_id_n );
			}
			cback->set_is_used ( false );
		}
	}
}

void
Timer_Server::cback_request_single (
	unsigned int cback_id_n )
{
	::Wdg2::Timer_Callback * cback ( find_callback ( cback_id_n ) );
	if ( cback != 0 ) {
		if ( cback->is_used() ) {
			cback->set_is_repeating ( false );

			::Wdg2::Timer * timer ( _timers[cback->timer_idx()] );
			if ( !cback->is_running() ) {
				cback->set_is_running ( true );
				timer->append_callback_idx ( cback_id_n-1 );
			}
			cback->set_msec_remain (
				timer->interval_msec() + timer->msec_latest() );
		}
	}
}

void
Timer_Server::cback_request_interval (
	unsigned int cback_id_n )
{
	::Wdg2::Timer_Callback * cback ( find_callback ( cback_id_n ) );
	if ( cback != 0 ) {
		if ( cback->is_used() ) {
			cback->set_is_repeating ( true );

			::Wdg2::Timer * timer ( _timers[cback->timer_idx()] );
			if ( !cback->is_running() ) {
				cback->set_is_running ( true );
				timer->append_callback_idx ( cback_id_n-1 );
			}
			cback->set_msec_remain (
				timer->interval_msec() + timer->msec_latest() );
		}
	}
}

void
Timer_Server::cback_abort_request (
	unsigned int cback_id_n )
{
	::Wdg2::Timer_Callback * cback ( find_callback ( cback_id_n ) );
	if ( cback != 0 ) {
		if ( cback->is_running() ) {
			cback->set_is_running ( false );
			// Remove from timer if running
			::Wdg2::Timer * timer ( _timers[cback->timer_idx()] );
			timer->remove_callback_idx ( cback_id_n-1 );
		}
	}
}

bool
Timer_Server::process_timeout (
	unsigned int cback_idx_n,
	unsigned int interval_msec_n,
	unsigned int msec_delta_n )
{
	::Wdg2::Timer_Callback & cback ( _callbacks[cback_idx_n] );
	if ( cback.is_running() ) {
		unsigned int overtime =
			cback.decrement_msec_remain ( msec_delta_n );
		if ( cback.msec_remain() == 0 ) {
			// Set any values before calling the callback
			// because it may change values as well.
			if ( cback.is_repeating() ) {
				overtime = overtime % interval_msec_n;
				cback.set_msec_remain ( interval_msec_n - overtime );
			} else {
				cback.set_is_running ( false );
			}

			// Invoke callback
			if ( cback.callback().is_valid() ) {
				cback.set_callback_value ( msec_delta_n );
				cback.callback().call();
			}
		}
	}
	return cback.is_running();
}


} // End of namespace
