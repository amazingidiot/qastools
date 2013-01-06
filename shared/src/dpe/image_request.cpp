//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "image_request.hpp"

#include "image_set_group.hpp"
#include "image_set_meta.hpp"
#include "image_set_state.hpp"


namespace dpe
{


Image_Request::Image_Request (
	::dpe::Image_Set_Group * group_n ) :
group ( group_n ),
meta ( group_n->img_sets.size(), 0 ),
states ( group_n->img_sets.size(), 0 )
{
}


Image_Request::~Image_Request ( )
{
}


void
Image_Request::wait_for_finish ( )
{
	for ( unsigned int ii=0; ii < states.size(); ++ii ) {
		::dpe::Image_Set_State * state ( states[ii] );
		if ( state != 0 ) {
			state->wait_for_finish();
		}
	}
}


} // End of namespace
