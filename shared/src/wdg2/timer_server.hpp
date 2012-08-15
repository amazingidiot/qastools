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
#include "timer_callback.hpp"
#include <QList>
#include <vector>

// Forward declaration
namespace Wdg2
{
	class Timer;
}

namespace Wdg2
{


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
	acquire_callback (
		unsigned int timer_id_n );

	void
	release_callback (
		unsigned int cback_id_n );

	void
	cback_set_callback (
		unsigned int cback_id_n,
		const ::Context_Callback_UInt & cback_n );


	void
	cback_request (
		unsigned int cback_id_n,
		bool repeating_n );

	void
	cback_request (
		unsigned int cback_id_n,
		unsigned int interval_msec_n,
		bool repeating_n );

	/// @brief Calls back on the next timeout with ( t_timeout - t_request ) >= timer->intervall_msec()
	///
	void
	cback_request_single (
		unsigned int cback_id_n );

	/// @brief Calls back on the next timeout with ( t_timeout - t_request ) >= interval_msec_n
	///
	/// Setting interval_msec_n to zero calls back on every timeout
	void
	cback_request_single (
		unsigned int cback_id_n,
		unsigned int interval_msec_n );

	/// @brief Calls back on every timeout with ( t_timeout - t_prev_call ) >= timer->intervall_msec()
	///
	/// Setting interval_msec_n to zero calls back on every timeout
	void
	cback_request_interval (
		unsigned int cback_id_n );

	/// @brief Calls back on every timeout with ( t_timeout - t_prev_call ) >= interval_msec_n
	///
	/// Setting interval_msec_n to zero calls back on every timeout
	void
	cback_request_interval (
		unsigned int cback_id_n,
		unsigned int interval_msec_n );

	void
	cback_abort_request (
		unsigned int cback_id_n );

	bool
	cback_is_running (
		unsigned int cback_id_n );



	bool
	process_timeout (
		unsigned int cback_idx_n,
		const QTime timeout_n );


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
