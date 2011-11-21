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

#ifndef __INC_tray_mixer_setup_hpp__
#define __INC_tray_mixer_setup_hpp__

#include <dpe/image_allocator.hpp>


/// @brief Tray_Mixer_View_Setup
///
class Tray_Mixer_View_Setup
{
	// Public methods
	public:

	Tray_Mixer_View_Setup ( );


	// Public attributes
	public:

	/// @brief Balloon lifetime in ms
	unsigned int balloon_lifetime;
	/// @brief Mouse wheel degrees for slider widgets
	unsigned int wheel_degrees;
	/// @brief Whether to show the balloon
	bool show_balloon;

	::dpe::Image_Allocator * image_alloc;
};


#endif
