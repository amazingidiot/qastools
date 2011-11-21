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

#ifndef __INC_desktop_items_setup_hpp__
#define __INC_desktop_items_setup_hpp__

#include <views/dev_select_view.hpp>
#include <views/settings_view_setup.hpp>
#include "tray_mixer_mdev_setup.hpp"
#include "tray_mixer_view_setup.hpp"
#include "main_window_setup.hpp"
#include <QString>


/// @brief Desktop_Items_Setup
///
class Desktop_Items_Setup
{
	// Public typedefs
	public:

	enum Mixer_Device {
		MIXER_DEV_DEFAULT  = 0,
		MIXER_DEV_PREVIOUS = 1,
		MIXER_DEV_USER     = 2,
		MIXER_DEV_LAST     = MIXER_DEV_USER
	};


	// Public methods
	public:

	Desktop_Items_Setup ( );


	void
	read_from_storage ( );

	void
	write_to_storage ( );


	// Public attributes
	public:

	// Device to load on startup
	unsigned int start_device_mode;
	QString start_user_device;

	::Tray_Mixer_MDev_Setup tray_mdev;
	::Tray_Mixer_View_Setup tray_view;
	::Main_Window_Setup main_window;
	::Views::Dev_Select_View_Setup dev_select_view;
	::Views::Settings_View_Setup settings_view;

	bool tray_on_close;
	bool tray_show_icon;
	bool tray_is_minimized;
};


#endif
