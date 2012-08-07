//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "theme_painters.hpp"
#include <iostream>

namespace Wdg2
{


bool
Theme_Painter_Slider::is_responsible (
	const ::dpe2::Key_Values & vset_n )
{
	bool vgood ( true );
	const unsigned int wgt (
		vset_n.val_uint ( ::Wdg2::PRK_WIDGET_TYPE, &vgood ) );
	if ( wgt != ::Wdg2::WGT_SLIDER ) {
		vgood = false;
	}
	return vgood;
}

void
Theme_Painter_Slider::paint (
	::dpe2::Pixmap & pxmap_n,
	const ::dpe2::Key_Values & kvals_n )
{
	bool vgood ( true );
	const unsigned int part (
		kvals_n.val_uint ( ::Wdg2::PRK_WIDGET_PART , &vgood ) );
	if ( vgood ) {
		switch ( part ) {
			case ::Wdg2::WGP_SLIDER_HANDLE:
				this->paint_handle ( pxmap_n, kvals_n );
				break;
			case ::Wdg2::WGP_SLIDER_RAIL:
				this->paint_rail ( pxmap_n, kvals_n );
				break;
			default:
				break;
		}
	}
}

bool
Theme_Painter_Scrollbar::is_responsible (
	const ::dpe2::Key_Values & vset_n )
{
	bool vgood ( true );
	const unsigned int wgt (
		vset_n.val_uint ( ::Wdg2::PRK_WIDGET_TYPE, &vgood ) );
	if ( wgt != ::Wdg2::WGT_SCROLLBAR ) {
		vgood = false;
	}
	return vgood;
}


} // End of namespace
