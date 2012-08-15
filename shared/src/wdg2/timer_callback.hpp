//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_timer_callback_hpp__
#define __INC_wdg2_timer_callback_hpp__

#include "callbacks.hpp"
#include <QTime>

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
	msec_interval ( ) const;

	void
	set_msec_interval (
		unsigned int msec_n );


	const QTime &
	start_time ( ) const;

	void
	set_start_time (
		const QTime & time_n );


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

	unsigned int _msec_interval;
	QTime _start_time;
	unsigned int _timer_idx;
	::Context_Callback_UInt _callback;
	bool _is_used;
	bool _is_running;
	bool _is_repeating;
};

inline
unsigned int
Timer_Callback::msec_interval ( ) const
{
	return _msec_interval;
}

inline
void
Timer_Callback::set_msec_interval (
	unsigned int msec_n )
{
	_msec_interval = msec_n;
}

inline
const QTime &
Timer_Callback::start_time ( ) const
{
	return _start_time;
}

inline
void
Timer_Callback::set_start_time (
	const QTime & time_n )
{
	_start_time = time_n;
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


} // End of namespace

#endif
