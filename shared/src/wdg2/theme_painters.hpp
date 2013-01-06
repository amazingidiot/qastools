//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_wdg2_theme_painters_hpp__
#define __INC_wdg2_theme_painters_hpp__

#include "dpe2/painter.hpp"
#include "gw_widget.hpp"
#include <QPalette>

namespace Wdg2
{

// Type definitions

enum Pixmap_Request_Keys
{
	PRK_WIDTH = ::dpe2::PMK_WIDTH,
	PRK_HEIGHT = ::dpe2::PMK_HEIGHT,
	PRK_PXM_INDEX = ::dpe2::PMK_USER,
	PRK_WIDGET_TYPE,
	PRK_WIDGET_PART,
	PRK_WIDGET_STATE_FLAGS,
	PRK_FONT,
	PRK_TEXT,
	PRK_TEXT_POS_X,
	PRK_TEXT_POS_Y,
	PRK_USER
};

enum Widget_Types
{
	WGT_NONE = 0,
	WGT_LABEL,
	WGT_SWITCH,
	WGT_SLIDER,
	WGT_SCROLLBAR
};

enum Widget_Parts_Label
{
	WGP_LABEL_NONE = 0,
	WGP_LABEL_GROUND
};

enum Widget_Parts_Switch
{
	WGP_SWITCH_NONE = 0,
	WGP_SWITCH_GROUND,
	WGP_SWITCH_HANDLE
};

enum Widget_Parts_Slider
{
	WGP_SLIDER_NONE = 0,
	WGP_SLIDER_RAIL,
	WGP_SLIDER_HANDLE
};

enum Widget_Parts_Scrollbar
{
	WGP_SCROLLBAR_NONE = 0,
	WGP_SCROLLBAR_RAIL,
	WGP_SCROLLBAR_HANDLE,
	WGP_SCROLLBAR_BTN_LEFT,
	WGP_SCROLLBAR_BTN_BOTTOM,
	WGP_SCROLLBAR_BTN_RIGHT,
	WGP_SCROLLBAR_BTN_TOP,
};



/// @brief Theme_Painter
///
class Theme_Painter :
	public ::dpe2::Painter
{
	// Public Methods
	public:

	const QPalette &
	qpalette ( ) const;

	void
	set_qpalette (
		const QPalette & palette_n );


	// Private attributes
	private:

	QPalette _qpalette;
};

inline
const QPalette &
Theme_Painter::qpalette ( ) const
{
	return _qpalette;
}

inline
void
Theme_Painter::set_qpalette (
	const QPalette & palette_n )
{
	_qpalette = palette_n;
}



/// @brief Theme_Painter_Label
///
class Theme_Painter_Label :
	public ::Wdg2::Theme_Painter
{
	// Public methods
	public:

	bool
	is_responsible (
		const ::dpe2::Key_Values & vset_n );

	void
	paint (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n );
};


/// @brief Theme_Painter_Switch
///
class Theme_Painter_Switch :
	public ::Wdg2::Theme_Painter
{
	// Public methods
	public:

	bool
	is_responsible (
		const ::dpe2::Key_Values & vset_n );

	void
	paint (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n );

	virtual
	void
	paint_ground (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n ) = 0;

	virtual
	void
	paint_handle (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n ) = 0;
};


/// @brief Theme_Painter_Slider
///
class Theme_Painter_Slider :
	public ::Wdg2::Theme_Painter
{
	// Public methods
	public:

	bool
	is_responsible (
		const ::dpe2::Key_Values & vset_n );

	void
	paint (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n );

	virtual
	void
	paint_rail (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n ) = 0;

	virtual
	void
	paint_handle (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n ) = 0;
};



/// @brief Theme_Painter_Scrollbar
///
class Theme_Painter_Scrollbar :
	public ::Wdg2::Theme_Painter
{
	// Public methods
	public:

	bool
	is_responsible (
		const ::dpe2::Key_Values & vset_n );

	void
	paint (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n );

	virtual
	void
	paint_rail (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n ) = 0;

	virtual
	void
	paint_handle (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n ) = 0;

	virtual
	void
	paint_button (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n,
		bool forward_n,
		bool vertical_n ) = 0;

};


} // End of namespace

#endif
