//
// C++ Interface:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INC_main_window_setup_hpp__
#define __INC_main_window_setup_hpp__

#include "mwdg/mixer_device_setup.hpp"
#include "mwdg/inputs_setup.hpp"
#include "views/mixer_hctl_setup.hpp"
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

	::MWdg::Mixer_Device_Setup mixer_dev;
	::MWdg::Inputs_Setup inputs;

	::Views::Mixer_HCTL_Setup hctl;
	::Views::Dev_Select_View_Setup dev_select;
};


#endif
