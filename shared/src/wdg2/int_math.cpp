//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "int_math.hpp"
#include <iostream>
#include <algorithm>


namespace Wdg2
{


unsigned int
integer_distance (
	int int_min_n,
	int int_max_n )
{
	unsigned int res ( 0 );

	if ( int_min_n > int_max_n ) {
		::std::swap ( int_min_n, int_max_n );
	}

	if ( ( int_min_n >= 0 ) || ( int_max_n < 0 ) ) {
		// No integer overflow possible
		res = int ( int_max_n - int_min_n );
	} else {
		// Integer overflow possible
		--int_max_n;
		++int_min_n;
		res = int_max_n;
		res += int ( -int_min_n );
		res += 2;
	}

	return res;
}

unsigned long
integer_distance (
	long int_min_n,
	long int_max_n )
{
	unsigned long res;

	if ( int_min_n > int_max_n ) {
		long tmp = int_max_n;
		int_max_n = int_min_n;
		int_min_n = tmp;
	}

	if ( ( int_min_n > 0 ) || ( int_max_n < 0 ) ) {
		// No integer overflow possible
		res = long ( int_max_n - int_min_n );
	} else {
		// Integer overflow possible
		--int_max_n;
		++int_min_n;
		res = int_max_n;
		res += long ( -int_min_n );
		res += 2;
	}

	return res;
}


} // End of namespace
