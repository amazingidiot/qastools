/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_main_window_hpp__
#define __INC_main_window_hpp__

#include "main_window_setup.hpp"
#include "qastools_config.hpp"
#include <QAction>
#include <QActionGroup>
#include <QMainWindow>
#include <QScopedPointer>
#include <QSplitter>

// Forward declaration
namespace Views
{
class Mixer_Simple;
class Device_Selection_View;
} // namespace Views

class Main_Window : public QMainWindow
{
  Q_OBJECT

  // Public methods
  public:
  Main_Window ( QWidget * parent_n = 0, Qt::WindowFlags flags_n = 0 );

  ~Main_Window ();

  QSize
  sizeHint () const;

  void
  set_window_setup ( Main_Window_Setup * setup_n );

  void
  select_ctl ( const QString & ctl_n );

  // Signals
  signals:

  void
  sig_show_settings ();

  void
  sig_show_info ();

  void
  sig_control_changed ();

  void
  sig_quit ();

  // Public slots
  public slots:

  // Snd control selection

  void
  select_ctl_from_side_iface ();

  void
  reload_mixer_device ();

  void
  reload_mixer_inputs ();

  void
  reload_mixer_view ();

  void
  refresh_views ();

  /// @brief Sets/unsets fullscreen mode
  ///
  void
  set_fullscreen ( bool flag_n );

  // Device selection

  void
  show_device_selection ( bool flag_n );

  void
  toggle_device_selection ();

  /// @brief Save state to the setup tree
  ///
  void
  save_state ();

  // Protected methods
  protected:
  void
  update_fullscreen_action ();

  // Event handlers

  void
  changeEvent ( QEvent * event_n );

  void
  keyPressEvent ( QKeyEvent * event_n );

  void
  init_menus ();

  void
  init_widgets ();

  // Private attributes
  private:
  Main_Window_Setup * _win_setup;

  // Base widgets
  QScopedPointer< QSplitter > _splitter;
  ::Views::Mixer_Simple * _mixer_simple;
  ::Views::Device_Selection_View * _dev_select;

  // Menubar
  QMenu * _menu_mixer;
  QAction * _act_show_dev_select;
  QAction * _act_fullscreen;

  // Strings and icons
  QString _str_fscreen_enable;
  QString _str_fscreen_disable;
  QIcon _icon_fscreen_enable;
  QIcon _icon_fscreen_disable;
};

#endif
