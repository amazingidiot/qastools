//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "color_methods.hpp"


namespace Wdg
{


QColor
col_mix (
	const QColor & col_1,
	const QColor & col_2,
	int w_1,
	int w_2 )
{
	const int wsum ( w_1 + w_2 );

	unsigned int com1[4] = {
		col_1.alpha(),
		col_1.red(),
		col_1.green(),
		col_1.blue()
	};
	for ( int ii=0; ii<4; ++ii ) {
		com1[ii] *= w_1;
	}

	{
		unsigned int com2[4] = {
			col_2.alpha(),
			col_2.red(),
			col_2.green(),
			col_2.blue()
		};

		for ( int ii=0; ii<4; ++ii ) {
			com2[ii] *= w_2;
		}

		// Mix
		for ( int ii=0; ii<4; ++ii ) {
			com1[ii] += com2[ii];
		}
	}

	// Rescale
	for ( int ii=0; ii<4; ++ii ) {
		com1[ii] /= wsum;
	}

	QColor res ( com1[1], com1[2], com1[3], com1[0] );
	return res;
}


} // End of namespace
