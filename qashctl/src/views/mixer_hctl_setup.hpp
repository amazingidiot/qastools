//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_mv_mixer_hctl_setup_hpp__
#define __INC_mv_mixer_hctl_setup_hpp__

#include "views/view_base_setup.hpp"

#include <QString>


namespace Views
{


class Mixer_HCTL_Setup :
	public ::Views::View_Base_Setup
{
	// Public methods
	public:

	Mixer_HCTL_Setup ( );


	// Public attributes
	public:

	// Public attributes
	QString iface_name;
	QString elem_grp_name;
	unsigned int elem_grp_index;
};


} // End of namespace

#endif
