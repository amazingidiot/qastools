//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_dpe_is_buffer_handle_hpp__
#define __INC_dpe_is_buffer_handle_hpp__

#include <QTime>


// Forward declaration
namespace dpe {
	class Image_Set;
	class Image_Set_Meta;
	class Image_Set_State;
}


namespace dpe
{


/// @brief IS_Buffer_Handle
///
class IS_Buffer_Handle
{
	// Public methods
	public:

	IS_Buffer_Handle ( );

	~IS_Buffer_Handle ( );

	::dpe::Image_Set * img_set;
	::dpe::Image_Set_Meta * meta;

	unsigned int num_users;
	QTime remove_time;

	::dpe::Image_Set_State * state;
};


} // End of namespace


#endif
