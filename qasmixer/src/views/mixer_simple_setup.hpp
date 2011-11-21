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

#ifndef __INC_mv_mixer_simple_setup_hpp__
#define __INC_mv_mixer_simple_setup_hpp__

#include "mv_base_setup.hpp"


namespace MView
{


class MV_Mixer_Simple_Setup :
	public MV_Base_Setup
{
	// Public methods
	public:

	MV_Mixer_Simple_Setup ( );


	// Public attributes
	public:

	bool show_slider_value_labels;
	bool show_stream[2];
};


} // End of namespace

#endif
