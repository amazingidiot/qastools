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

#ifndef __INC_mixer_window_setup_hpp__
#define __INC_mixer_window_setup_hpp__

#include "mwdg/mixer_device_setup.hpp"
#include "mwdg/inputs_setup.hpp"
#include "views/mixer_simple_setup.hpp"
#include "views/dev_select_view_setup.hpp"
#include <QByteArray>
#include <QKeySequence>


/// @brief Mixer_Window_Setup
///
class Mixer_Window_Setup
{
	// Public methods
	public:

	Mixer_Window_Setup ( );


	// Public attributes
	public:

	bool show_dev_select;
	QByteArray window_state;
	QByteArray window_geometry;

	::MWdg::Mixer_Device_Setup mixer_dev;
	::MWdg::Inputs_Setup inputs;

	::Views::Mixer_Simple_Setup mixer_simple;
	::Views::Dev_Select_View_Setup dev_select;
};


#endif
