/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_cmd_options_hpp__
#define __INC_cmd_options_hpp__

#include <QString>

/// @brief Command (line) options
///
/// These can come from the command line or from a second instance
class CMD_Options
{

  // Public methods
  public:
  CMD_Options ();

  // Public attributes
  public:
  bool start_tray_minimized;
  bool start_single_instance;
  QString start_ctl_address;
};

#endif
