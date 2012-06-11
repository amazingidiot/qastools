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
#include <vector>


// Forward declaration
namespace dpe {
	class Image_Set_Meta;
	class Image_Set_Group;
	class Image_Set_State;
}


namespace dpe2
{


/*
/// @brief Pixmap_Request
///
class Pixmap_Request
{
	// Public methods
	public:

	Pixmap_Request (
		::dpe::Image_Set_Group * group_n );

	~Pixmap_Request ( );


	/// @brief To be called by the requesting thread
	///
	/// This method must be called at some point after
	/// sending this request to an Image_Allocator.
	void
	wait_for_finish ( );


	// Public attributes
	public:

	::dpe::Image_Set_Group * group;
	::std::vector < ::dpe::Image_Set_Meta * > meta;
	::std::vector < ::dpe::Image_Set_State * > states;
};
*/



} // End of namespace


#endif
