//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "mixer_style.hpp"

namespace MWdg
{


QPalette
Mixer_Style::style_palette (
	::MWdg::Mixer_Style::Style_Type style_n )
{
	QPalette pal;

	const QPalette::ColorGroup grp_act ( QPalette::Active );
	const QPalette::ColorGroup grp_iact ( QPalette::Inactive );
	QColor col;

	if ( style_n == ::MWdg::Mixer_Style::PLAYBACK ) {
		// Playback palette
		col = QColor ( 15, 15, 242 );
		pal.setColor ( grp_act, QPalette::Window, col );
		pal.setColor ( grp_iact, QPalette::Window, col );

		col = QColor ( 0, 0, 60 );
		pal.setColor ( grp_act, QPalette::WindowText, col );
		pal.setColor ( grp_iact, QPalette::WindowText, col );

		col = QColor ( 255, 255, 180 );
		pal.setColor ( grp_act, QPalette::Light, col );
		pal.setColor ( grp_iact, QPalette::Light, col );

	} else if ( style_n == ::MWdg::Mixer_Style::CAPTURE ) {
		// Capture palette
		col = QColor ( 225, 15, 15 );
		pal.setColor ( grp_act, QPalette::Window, col );
		pal.setColor ( grp_iact, QPalette::Window, col );

		col = QColor ( 80, 0, 0 );
		pal.setColor ( grp_act, QPalette::WindowText, col );
		pal.setColor ( grp_iact, QPalette::WindowText, col );

		col = QColor ( 255, 255, 180 );
		pal.setColor ( grp_act, QPalette::Light, col );
		pal.setColor ( grp_iact, QPalette::Light, col );

	}

	return pal;
}


} // End of namespace

