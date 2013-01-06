//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_wdg2_timer_hpp__
#define __INC_wdg2_timer_hpp__

#include <QObject>
#include <QTime>

// Forward declaration
namespace Wdg2
{
	class Timer_Server;
}

namespace Wdg2
{


/// @brief Timer
///
class Timer :
	public QObject
{
	// Public methods
	public:

	Timer (
		::Wdg2::Timer_Server * server_n );

	~Timer ( );


	unsigned int
	interval_msec ( ) const;

	void
	set_interval_msec (
		unsigned int msec_n );


	bool
	is_running ( ) const;

	/// @brief Time of the latest timeout
	const QTime &
	time ( ) const;

	void
	append_callback_idx (
		unsigned int cback_idx_n );

	void
	remove_callback_idx (
		unsigned int cback_idx_n );

	unsigned int
	num_callbacks ( ) const;

	unsigned int
	callback_idx (
		unsigned int idx_n );


	// Protected methods
	protected:

	void
	start ( );

	void
	stop ( );

	void
	timerEvent (
		QTimerEvent * event_n );


	// Private attributes
	private:

	typedef QList < unsigned int > CBack_List;

	CBack_List _callbacks;
	QTime _time;
	unsigned int _interval_msec;
	int _timer_id;
	::Wdg2::Timer_Server * _server;
};

inline
const QTime &
Timer::time ( ) const
{
	return _time;
}

inline
unsigned int
Timer::interval_msec ( ) const
{
	return _interval_msec;
}

inline
unsigned int
Timer::num_callbacks ( ) const
{
	return _callbacks.size();
}

inline
unsigned int
Timer::callback_idx (
	unsigned int idx_n )
{
	return _callbacks[idx_n];
}

inline
bool
Timer::is_running ( ) const
{
	return ( _timer_id >= 0 );
}


} // End of namespace

#endif
