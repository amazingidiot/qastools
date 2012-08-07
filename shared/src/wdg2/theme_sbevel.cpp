//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "theme_sbevel.hpp"
#include "theme_painters.hpp"

namespace Wdg2
{


class Painter_SBevel_Slider :
	public ::Wdg2::Painter_Slider
{
	// Public methods
	public:

	void
	paint (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n );
};

void
Painter_SBevel_Slider::paint (
	::dpe2::Pixmap & pxmap_n,
	const ::dpe2::Key_Values & kvals_n )
{

}



class Painter_SBevel_Scrollbar :
	public ::Wdg2::Painter_Slider
{
	// Public methods
	public:

	void
	paint (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n );
};

void
Painter_SBevel_Scrollbar::paint (
	::dpe2::Pixmap & pxmap_n,
	const ::dpe2::Key_Values & kvals_n )
{

}



Theme_SBevel::Theme_SBevel ( )
{
	add_painter ( new Painter_SBevel_Slider );
	add_painter ( new Painter_SBevel_Scrollbar );
}


} // End of namespace
