/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_desktop_items_setup_hpp__
#define __INC_desktop_items_setup_hpp__

#include "main_window_setup.hpp"
#include "tray_mixer_mdev_setup.hpp"
#include "tray_mixer_view_setup.hpp"
#include "views/device_selection_view.hpp"
#include "views/settings_dialog_setup.hpp"
#include <QString>

/// @brief Desktop_Items_Setup
///
class Desktop_Items_Setup
{
  // Public typedefs
  public:
  enum Mixer_Device
  {
    MIXER_DEV_DEFAULT = 0,
    MIXER_DEV_PREVIOUS = 1,
    MIXER_DEV_USER = 2,
    MIXER_DEV_LAST = MIXER_DEV_USER
  };

  // Public methods
  public:
  Desktop_Items_Setup ();

  void
  read_from_storage ();

  void
  write_to_storage ();

  // Public attributes
  public:
  // Device to load on startup
  unsigned int start_device_mode;
  QString start_user_device;

  ::Tray_Mixer_MDev_Setup tray_mdev;
  ::Tray_Mixer_View_Setup tray_view;
  ::Main_Window_Setup main_window;
  ::Views::Device_Selection_View_Setup device_selection_view;
  ::Views::Settings_Dialog_Setup settings_dialog;

  bool tray_on_close;
  bool tray_show_icon;
  bool tray_is_minimized;
};

#endif
