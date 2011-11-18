//
// C++ Interface:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INC_dpe_image_set_group_hpp__
#define __INC_dpe_image_set_group_hpp__

#include <vector>


// Forward declaration
namespace dpe {
	class Image_Set;
}


namespace dpe
{


/// @brief Image_Set_Group
///
class Image_Set_Group
{
	// Public methods
	public:

	Image_Set_Group (
		unsigned int num_sets_n = 0 );

	bool
	ready ( ) const;

	void
	convert_to_pixmap ( );


	// Public attributes
	public:

	::std::vector < ::dpe::Image_Set * > img_sets;
};


inline
bool
Image_Set_Group::ready ( ) const
{
	for ( unsigned int ii=0; ii < img_sets.size(); ++ii ) {
		if ( img_sets[ii] == 0 ) {
			return false;
		}
	}
	return true;
}

} // End of namespace


#endif
