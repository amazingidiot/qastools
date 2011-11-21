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

#include "mview/switcher_setup.hpp"
#include "mview/dev_select_view_setup.hpp"
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
	QKeySequence kseq_dev_select;
	QKeySequence kseq_vtype_select;

	::MView::Switcher_Setup switcher;
	::MView::Dev_Select_View_Setup dev_select;
};


#endif
