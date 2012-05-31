//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_int_math_hpp__
#define __INC_wdg2_int_math_hpp__


namespace Wdg2
{

///
/// @return The distance between the values as unsigned int
unsigned int
integer_distance (
	int int_min_n,
	int int_max_n );

///
/// @return The distance between the values as unsigned long
unsigned long
integer_distance (
	long int_min_n,
	long int_max_n );

unsigned int
permille (
	unsigned int current_n,
	unsigned int total_n );

unsigned int
permille (
	unsigned long current_n,
	unsigned long total_n );


inline
unsigned int
permille (
	unsigned int current_n,
	unsigned int total_n )
{
	unsigned int res;
	if ( total_n > 16000 ) {
		res = current_n / ( total_n / 1000 );
	} else {
		res = ( current_n * 1000 ) / total_n;
	}
	return res;
}

inline
unsigned int
permille (
	unsigned long current_n,
	unsigned long total_n )
{
	unsigned int res;
	if ( total_n > 16000 ) {
		res = current_n / ( total_n / 1000 );
	} else {
		res = ( current_n * 1000 ) / total_n;
	}
	return res;
}


} // End of namespace

#endif
