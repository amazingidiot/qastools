//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_theme_painters_hpp__
#define __INC_wdg2_theme_painters_hpp__

#include "dpe2/painter.hpp"

namespace Wdg2
{

// Type definitions

enum Pixmap_Request_Keys
{
	PRK_NONE = ::dpe2::PMK_USER,
	PRK_WIDGET_TYPE,
	PRK_WIDGET_PART,
};

enum Widget_Types
{
	WGT_NONE = 0,
	WGT_SLIDER,
	WGT_SCROLLBAR,
};

enum Widget_Parts
{
	WGP_NONE = 0,
	WGP_SLIDER_RAIL,
	WGP_SLIDER_HANDLE,
};


/// @brief Painter_Slider
///
class Painter_Slider :
	public ::dpe2::Painter
{
	// Public methods
	public:

	bool
	is_responsible (
		const ::dpe2::Key_Values & vset_n );
};


/// @brief Painter_Scrollbar
///
class Painter_Scrollbar :
	public ::dpe2::Painter
{
	// Public methods
	public:

	bool
	is_responsible (
		const ::dpe2::Key_Values & vset_n );
};


} // End of namespace

#endif
