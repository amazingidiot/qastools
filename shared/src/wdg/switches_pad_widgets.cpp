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


Switches_Pad_Widgets::Switches_Pad_Widgets ( ) :
_label ( 0 ),
_input ( 0 )
{
}


Switches_Pad_Widgets::~Switches_Pad_Widgets ( )
{
	clear_widgets();
}


void
Switches_Pad_Widgets::clear_widgets ( )
{
	if ( _label != 0 ) {
		delete _label;
		_label = 0;
	}
	if ( _input != 0 ) {
		delete _input;
		_input = 0;
	}
}


void
Switches_Pad_Widgets::set_input_wdg (
	QWidget * wdg_n )
{
	if ( _input != 0 ) {
		delete _input;
	}
	_input = wdg_n;
}


void
Switches_Pad_Widgets::set_label_wdg (
	QLabel * wdg_n )
{
	if ( _label != 0 ) {
		delete _label;
	}
	_label = wdg_n;
}


} // End of namespace

