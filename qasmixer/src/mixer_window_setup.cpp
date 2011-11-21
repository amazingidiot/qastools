//
// C++ Implementation:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "mixer_window_setup.hpp"


Mixer_Window_Setup::Mixer_Window_Setup ( ) :
show_dev_select ( false ),
kseq_dev_select ( Qt::Key_F6 ),
kseq_vtype_select ( Qt::Key_F8 )
{
	dev_select.kseq_toggle_vis = kseq_dev_select;
}
