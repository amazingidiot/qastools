//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_dpe_paint_job_hpp__
#define __INC_dpe_paint_job_hpp__


// Forward declaration
namespace dpe {
	class Image_Request;
	class Image_Set;
	class Image_Set_Meta;
	class Image_Set_State;
}


namespace dpe
{


/// @brief Paint_Job
///
class Paint_Job
{
	// Public methods
	public:

	Paint_Job ( );


	// Public attributes
	public:

	::dpe::Image_Set_Meta * meta;
	::dpe::Image_Set * img_set;
	unsigned int img_idx;
	::dpe::Image_Set_State * state;
};


} // End of namespace

#endif


