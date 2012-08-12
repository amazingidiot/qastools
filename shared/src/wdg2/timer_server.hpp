//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_timer_server_hpp__
#define __INC_wdg2_timer_server_hpp__

#include "callbacks.hpp"
#include <QList>
#include <vector>

// Forward declaration
namespace Wdg2
{
	class Timer;
}

namespace Wdg2
{



/// @brief Timer_Callback
///
class Timer_Callback
{
	// Public methods
	public:

	Timer_Callback ( );


	unsigned int
	timer_idx ( ) const;

	void
	set_timer_idx (
		unsigned int idx_n );


	unsigned int
	msec_remain ( ) const;

	void
	set_msec_remain (
		unsigned int msec_n );

	/// @return The number of overtime msecs
	unsigned int
	decrement_msec_remain (
		unsigned int delta_n );


	const ::Context_Callback_UInt &
	callback ( ) const;

	void
	set_callback (
		const ::Context_Callback_UInt & cb_n );

	void
	set_callback_value (
		unsigned int value_n );


	bool
	is_used ( ) const;

	void
	set_is_used (
		bool flag_n );


	bool
	is_running ( ) const;

	void
	set_is_running (
		bool flag_n );


	bool
	is_repeating ( ) const;

	void
	set_is_repeating (
		bool flag_n );


	// Private attributes
	private:

	unsigned int _msec_remain;
	::Context_Callback_UInt _callback;
	unsigned int _timer_idx;
	bool _is_used;
	bool _is_running;
	bool _is_repeating;
};

inline
unsigned int
Timer_Callback::msec_remain ( ) const
{
	return _msec_remain;
}

inline
void
Timer_Callback::set_msec_remain (
	unsigned int msec_n )
{
	_msec_remain = msec_n;
}

inline
unsigned int
Timer_Callback::decrement_msec_remain (
	unsigned int msec_n )
{
	unsigned int overtime;
	if ( _msec_remain > msec_n ) {
		overtime = 0;
		_msec_remain -= msec_n;
	} else {
		overtime = msec_n - _msec_remain;
		_msec_remain = 0;
	}
	return overtime;
}

inline
const ::Context_Callback_UInt &
Timer_Callback::callback ( ) const
{
	return _callback;
}

inline
void
Timer_Callback::set_callback (
	const ::Context_Callback_UInt & cb_n )
{
	_callback = cb_n;
}

inline
void
Timer_Callback::set_callback_value (
	unsigned int value_n )
{
	_callback.set_value ( value_n );
}

inline
unsigned int
Timer_Callback::timer_idx ( ) const
{
	return _timer_idx;
}

inline
void
Timer_Callback::set_timer_idx (
	unsigned int timer_idx_n )
{
	_timer_idx = timer_idx_n;
}

inline
bool
Timer_Callback::is_used ( ) const
{
	return _is_used;
}

inline
void
Timer_Callback::set_is_used (
	bool flag_n )
{
	_is_used = flag_n;
}

inline
bool
Timer_Callback::is_running ( ) const
{
	return _is_running;
}

inline
void
Timer_Callback::set_is_running (
	bool flag_n )
{
	_is_running = flag_n;
}

inline
bool
Timer_Callback::is_repeating ( ) const
{
	return _is_repeating;
}

inline
void
Timer_Callback::set_is_repeating (
	bool flag_n )
{
	_is_repeating = flag_n;
}


/// @brief Timer_Server
///
class Timer_Server
{
	// Public methods
	public:

	Timer_Server ( );

	~Timer_Server ( );


	/// @brief Creates a timer and returns its id
	unsigned int
	create_timer (
		unsigned int interval_msec_n );

	void
	destroy_timer (
		unsigned int timer_id_n );


	unsigned int
	aquire_callback (
		unsigned int timer_id_n );

	void
	release_callback (
		unsigned int cback_id_n );

	void
	cback_set_callback (
		unsigned int cback_id_n,
		const ::Context_Callback & cback_n );


	void
	cback_request_single (
		unsigned int cback_id_n );

	void
	cback_request_interval (
		unsigned int cback_id_n );

	void
	cback_abort_request (
		unsigned int cback_id_n );


	bool
	process_timeout (
		unsigned int cback_idx_n,
		unsigned int interval_msec_n,
		unsigned int msec_delta_n );


	// Private methods
	private:

	::Wdg2::Timer_Callback *
	find_callback (
		unsigned int cback_id_n );

	::Wdg2::Timer *
	find_timer (
		unsigned int timer_id_n );

	::Wdg2::Timer *
	create_timer_obj ( );

	void
	destroy_timer_obj (
		::Wdg2::Timer * timer_n );


	// Private attributes
	private:

	::std::vector < ::Wdg2::Timer_Callback > _callbacks;
	QList < ::Wdg2::Timer * > _timers;
};

inline
::Wdg2::Timer_Callback *
Timer_Server::find_callback (
	unsigned int cback_id_n )
{
	::Wdg2::Timer_Callback * cback ( 0 );
	if ( cback_id_n != 0 ) {
		--cback_id_n;
		if ( cback_id_n < _callbacks.size() ) {
			cback = &_callbacks[cback_id_n];
		}
	}
	return cback;
}

inline
::Wdg2::Timer *
Timer_Server::find_timer (
	unsigned int timer_id_n )
{
	::Wdg2::Timer * timer ( 0 );
	if ( timer_id_n != 0 ) {
		--timer_id_n;
		if ( (int)timer_id_n < _timers.size() ) {
			timer = _timers[timer_id_n];
		}
	}
	return timer;
}


} // End of namespace

#endif
