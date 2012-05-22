//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_qsnd_mixer_hpp__
#define __INC_qsnd_mixer_hpp__

#include "proxies.hpp"

namespace QSnd2
{


/// @brief Mixer
///
class Mixer
{
	// Public methods
	public:

	Mixer ( );

	virtual
	~Mixer ( );


	unsigned int
	num_groups ( ) const;

	::QSnd2::Mixer_Group2 *
	group (
		unsigned int idx_n ) const;


	// Private attributes
	private:

	QList < ::QSnd2::Mixer_Group2 * > _groups;
};

inline
unsigned int
Mixer::num_groups ( ) const
{
	return _groups.size();
}

inline
::QSnd2::Mixer_Group2 *
Mixer::group (
	unsigned int idx_n ) const
{
	return _groups[idx_n];
}


} // End of namespace

#endif
