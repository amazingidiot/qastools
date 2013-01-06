//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "theme.hpp"
#include "theme_painters.hpp"

namespace Wdg2
{


Theme::Theme ( )
{
}

Theme::~Theme ( )
{
	// Delete painters
	{
		Painter_List::iterator it ( _painters.begin() );
		for ( ; it != _painters.end(); ++it ) {
			delete (*it);
		}
	}
}

void
Theme::add_painter (
	::Wdg2::Theme_Painter * painter_n )
{
	if ( painter_n != 0 ) {
		painter_n->set_qpalette ( qpalette() );
		_painters.append ( painter_n );
	}
}

void
Theme::take_painter (
	::Wdg2::Theme_Painter * painter_n )
{
	_painters.removeOne ( painter_n );
}

void
Theme::set_qpalette (
	const QPalette & palette_n )
{
	_qpalette = palette_n;
	{
		Painter_List::iterator it ( _painters.begin() );
		for ( ; it != _painters.end(); ++it ) {
			(*it)->set_qpalette (_qpalette );
		}
	}
}


} // End of namespace
