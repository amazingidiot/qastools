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
#include "gw_widget.hpp"
#include <QPalette>

namespace Wdg2
{

// Type definitions

enum Pixmap_Request_Keys
{
	PRK_NONE = ::dpe2::PMK_USER,
	PRK_WIDGET_TYPE,
	PRK_WIDGET_PART,
	PRK_WIDGET_STATE_FLAGS
};

enum Widget_Types
{
	WGT_NONE = 0,
	WGT_SWITCH,
	WGT_SLIDER,
	WGT_SCROLLBAR,
};

enum Widget_Parts
{
	WGP_NONE = 0,
	WGP_SWITCH_GROUND,
	WGP_SWITCH_HANDLE,
	WGP_SLIDER_RAIL,
	WGP_SLIDER_HANDLE,
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


} // End of namespace

#endif
