//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_dpe2_pixmap_request_hpp__
#define __INC_dpe2_pixmap_request_hpp__

#include "callbacks.hpp"
#include "values_set.hpp"
#include "pixmap.hpp"
#include <vector>


namespace dpe2
{


/// @brief Pixmap_Request
///
class Pixmap_Request
{
	// Public methods
	public:

	Pixmap_Request (
		::dpe2::Pixmap * pixmap_n = 0 );

	// Public attributes
	public:

	::dpe2::Values_Set values;
	::Context_Callback finished_callback;
	::dpe2::Pixmap * pixmap;
};

inline
Pixmap_Request::Pixmap_Request (
	::dpe2::Pixmap * pixmap_n ) :
pixmap ( pixmap_n )
{
}


} // End of namespace

#endif
