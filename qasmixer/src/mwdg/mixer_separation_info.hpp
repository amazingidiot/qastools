//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//


#ifndef __INC_mixer_separation_info_hpp__
#define __INC_mixer_separation_info_hpp__

#include <QTime>


namespace MWdg
{


///
/// @brief Mixer_Separation_Info
///
class Mixer_Separation_Info
{
	// Public methods
	public:

	Mixer_Separation_Info ( );


	// Separation

	bool
	requested ( ) const;

	void
	set_requested (
		bool flag_n );


	bool
	do_it ( ) const;

	void
	set_do_it (
		bool flag_n );


	const QTime &
	time ( ) const;

	void
	set_time (
		const QTime & time_n );


	// Private attributes
	private:

	QTime _time;

	bool _requested;

	bool _do_it;
};


inline
bool
Mixer_Separation_Info::requested ( ) const
{
	return _requested;
}


inline
void
Mixer_Separation_Info::set_requested (
	bool flag_n )
{
	_requested = flag_n;
}


inline
bool
Mixer_Separation_Info::do_it ( ) const
{
	return _do_it;
}


inline
void
Mixer_Separation_Info::set_do_it (
	bool flag_n )
{
	_do_it = flag_n;
}


inline
const QTime &
Mixer_Separation_Info::time ( ) const
{
	return _time;
}


inline
void
Mixer_Separation_Info::set_time (
	const QTime & time_n )
{
	_time = time_n;
}


} // End of namespace


#endif

