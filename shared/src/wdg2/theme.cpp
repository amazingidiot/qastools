//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "theme.hpp"
#include "dpe2/painter.hpp"

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
	::dpe2::Painter * painter_n )
{
	if ( painter_n != 0 ) {
		_painters.append ( painter_n );
	}
}

void
Theme::take_painter (
	::dpe2::Painter * painter_n )
{
	_painters.removeOne ( painter_n );
}


} // End of namespace
