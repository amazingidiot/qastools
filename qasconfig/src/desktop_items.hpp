/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_desktop_items_hpp__
#define __INC_desktop_items_hpp__

#include "main_window.hpp"
#include <QScopedPointer>

/// @brief Manages all items that appear on the desktop
///
class Desktop_Items
{
  // Public methods
  public:
  Desktop_Items ();

  ~Desktop_Items ();

  /// @brief Reads options from storage and command line
  ///
  /// @return A negative value on an error
  int
  init_settings ( int argc, char * argv[] );

  /// @brief Start the mixer window and/or tray icon
  ///
  void
  start ( bool restore_session_n = false );

  /// @brief Command line option parser
  ///
  /// @return A negative value on an error
  int
  parse_cmd_options ( int argc, char * argv[] );

  // Private attributes
  private:
  QScopedPointer< Main_Window > _mwin;
};

#endif
