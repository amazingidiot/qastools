//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_views_view_base_setup_hpp__
#define __INC_views_view_base_setup_hpp__


// Forward declaration
namespace dpe { class Image_Allocator; }
namespace Wdg { class DS_Widget_Style_Db; }


namespace Views
{


/// @brief View_Base general static setup variables
///
class View_Base_Setup
{
	// Public methods
	public:

	View_Base_Setup ( );

	virtual
	~View_Base_Setup ( );


	// Public attributes
	public:

	const ::Wdg::DS_Widget_Style_Db * wdg_style_db;
	::dpe::Image_Allocator * image_alloc;
};


} // End of namespace

#endif
