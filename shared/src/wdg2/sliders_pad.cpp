//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "sliders_pad.hpp"

#include <QEvent>
#include <QKeyEvent>
#include <QFocusEvent>

#include <iostream>


namespace Wdg2
{


Sliders_Pad::Sliders_Pad (
	QWidget * parent_n ) :
QGraphicsView ( parent_n ),
_snd_controls ( 0 )
{
	setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Expanding );
}

Sliders_Pad::~Sliders_Pad ( )
{
}

void
Sliders_Pad::set_snd_controls (
	::QSnd2::Controls * controls_n )
{
	if ( _snd_controls != 0 ) {
		destroy_scene_items();
	}

	_snd_controls = controls_n;

	if ( _snd_controls != 0 ) {
		build_scene_items();
	}
}

void
Sliders_Pad::destroy_scene_items ( )
{
	_scene.clear();
}

void
Sliders_Pad::build_scene_items ( )
{

}


} // End of namespace
