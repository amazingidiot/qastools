//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_main_window_setup_hpp__
#define __INC_main_window_setup_hpp__

#include "mwdg/mixer_device_setup.hpp"
#include "mwdg/inputs_setup.hpp"
#include "views/dev_select_view_setup.hpp"
#include <QByteArray>
#include <QKeySequence>


/// @brief Main_Window_Setup
///
class Main_Window_Setup
{
	// Public methods
	public:

	Main_Window_Setup ( );


	// Public attributes
	public:

	bool show_dev_select;
	QByteArray window_state;
	QByteArray window_geometry;
	QByteArray splitter_state;

	::MWdg::Mixer_Device_Setup mixer_dev;
	::MWdg::Inputs_Setup inputs;

	::Views::Dev_Select_View_Setup dev_select;
};


#endif
