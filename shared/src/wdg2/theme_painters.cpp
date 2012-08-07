//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "theme_painters.hpp"
#include <QEvent>

namespace Wdg2
{

bool
Painter_Slider::is_responsible (
	const ::dpe2::Key_Values & vset_n )
{
	bool vgood ( true );
	const unsigned int wgt (
		vset_n.val_uint ( ::Wdg2::PRK_WIDGET_TYPE, &vgood ) );
	if ( wgt != WGT_SLIDER ) {
		vgood = false;
	}
	return vgood;
}

bool
Painter_Scrollbar::is_responsible (
	const ::dpe2::Key_Values & vset_n )
{
	bool vgood ( true );
	const unsigned int wgt (
		vset_n.val_uint ( ::Wdg2::PRK_WIDGET_TYPE, &vgood ) );
	if ( wgt != WGT_SCROLLBAR ) {
		vgood = false;
	}
	return vgood;
}

} // End of namespace
