/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_desktop_items_hpp__
#define __INC_desktop_items_hpp__

#include "cmd_options.hpp"
#include "desktop_items_setup.hpp"
#include "dpe/image_allocator.hpp"
#include "mwdg/mixer_style.hpp"
#include <QDialog>
#include <QObject>
#include <QPointer>

// Forward declaration
class Tray_Mixer;
class Main_Window;
class Dialog_Settings;

/// @brief Manages all items that appear on the desktop
///
/// The managed window items are
///  - Main_Mixer
///  - Tray_Mixer ( with tray icon )
///
/// All settings are always present in the setup tree.
/// The setup tree gets initialized in the constructor
/// and on calling init_settings().
/// On destruction it gets written to the storage (disk).
class Desktop_Items : public QObject
{
  Q_OBJECT

  // Public methods
  public:
  Desktop_Items ( QObject * parent_n = 0 );

  ~Desktop_Items ();

  /// @brief Options that came from the command line or a second instance
  ///
  const CMD_Options &
  cmd_opts () const;

  /// @brief Reads options from storage and command line
  ///
  /// @return A negative value on an error
  int
  init_settings ( int argc, char * argv[] );

  /// @brief Message to be sent to an other instance
  ///
  /// @return The message
  QString
  message_to_other_instance () const;

  /// @brief Start the mixer window and/or tray icon
  ///
  void
  start ( bool restore_session_n = false );

  // Event handling

  bool
  event ( QEvent * event_n );

  bool
  eventFilter ( QObject * obj_n, QEvent * event_n );

  // Signals
  signals:

  void
  sig_quit ();

  // Public slots
  public slots:

  /// @brief Reads messages from other instances
  ///
  void
  parse_message ( QString msg_n );

  // Main mixer minimize / raise

  void
  main_mixer_raise ();

  void
  main_mixer_close ();

  void
  main_mixer_toggle_by_tray ();

  // Dialogs

  void
  show_dialog_info ();

  void
  show_dialog_settings ();

  /// @brief Closes and destroy all widgets
  void
  shutdown ();

  /// @brief Shuts down and emits sig_quit()
  void
  quit ();

  // Protected slots
  protected slots:

  void
  main_mixer_reload_view ();

  /// @brief Lets widgets reload the inputs setup from the setup tree
  ///
  void
  reload_inputs_setup ();

  /// @return true if the tray icon is visible
  ///
  void
  tray_mixer_update_visibility ();

  void
  tray_mixer_reload_mdev ();

  void
  tray_mixer_reload_current_mdev ();

  void
  tray_mixer_update_balloon_setup ();

  // Private methods
  private:
  /// @brief Creates a new main mixer window
  ///
  void
  main_mixer_create ();

  /// @brief Destroys the main mixer window
  ///
  void
  main_mixer_destroy ();

  /// @brief True if the main mixer exists and is visible
  ///
  bool
  main_mixer_visible ();

  /// @brief Will be called shortly after the mixer window was closed
  ///
  void
  main_mixer_closed ();

  /// @brief Creates a tray mixer instance
  ///
  void
  tray_mixer_create ();

  void
  tray_mixer_destroy ();

  bool
  tray_mixer_visible ();

  /// @brief Command line option parser
  ///
  /// @return A negative value on an error
  int
  parse_cmd_options ( int argc, char * argv[] );

  // Private attributes
  private:
  Desktop_Items_Setup _dsetup;

  QPointer< Tray_Mixer > _tray_mixer;
  QPointer< Main_Window > _main_mixer;

  // Shared storages and settings
  ::MWdg::Mixer_Style _mixer_style;
  ::Wdg::DS_Widget_Style_Db _wdg_style_db;
  ::dpe::Image_Allocator _image_alloc;

  // Dialogs
  QPointer< QDialog > _dialog_settings;
  QPointer< QDialog > _dialog_info;

  CMD_Options _cmd_opts;
  int _evt_mixer_closed;
  bool _started;
  bool _shutdown;
};

inline const CMD_Options &
Desktop_Items::cmd_opts () const
{
  return _cmd_opts;
}

#endif
