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

#include "switches_pad_widgets.hpp"


namespace Wdg
{


Switches_Pad_Widgets::Switches_Pad_Widgets ( )
{
}


Switches_Pad_Widgets::~Switches_Pad_Widgets ( )
{
	clear_widgets();
}


void
Switches_Pad_Widgets::clear_widgets ( )
{
	_label.reset();
	_input.reset();
}


void
Switches_Pad_Widgets::set_input_wdg (
	QWidget * wdg_n )
{
	_input.reset ( wdg_n );
}


void
Switches_Pad_Widgets::set_label_wdg (
	QLabel * wdg_n )
{
	_label.reset ( wdg_n );
}


} // End of namespace

