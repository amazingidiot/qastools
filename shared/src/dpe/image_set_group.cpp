//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "image_set_group.hpp"

#include "image_set.hpp"


namespace dpe
{


Image_Set_Group::Image_Set_Group (
	unsigned int num_sets_n ) :
img_sets ( num_sets_n, 0 )
{
}


void
Image_Set_Group::convert_to_pixmap ( )
{
	for ( unsigned int ii=0; ii < img_sets.size(); ++ii ) {
		img_sets[ii]->convert_to_pixmap();
	}
}


} // End of namespace
