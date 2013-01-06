//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_sliders_pad_footer_hpp__
#define __INC_sliders_pad_footer_hpp__

#include "sliders_pad_header.hpp"


namespace Wdg
{


///
/// @brief Sliders_Pad_Footer
///
class Sliders_Pad_Footer :
	public Sliders_Pad_Header
{
	Q_OBJECT

	// Public methods
	public:

	Sliders_Pad_Footer (
		Sliders_Pad_Data * sp_data_n,
		Sliders_Pad_Style * sp_style_n,
		QWidget * parent_n = 0 );

};


} // End of namespace


#endif
