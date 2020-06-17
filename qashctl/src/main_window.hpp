/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_main_window_hpp__
#define __INC_main_window_hpp__

#include "dpe/image_allocator.hpp"
#include "main_window_setup.hpp"
#include "views/mixer_hctl.hpp"
#include "wdg/ds_widget_style_db.hpp"
#include <QDialog>
#include <QFileSystemWatcher>
#include <QMainWindow>
#include <QPointer>
#include <QScopedPointer>
#include <QSplitter>

// Forward declaration
namespace Views
{
class Mixer_HCTL;
}
namespace Views
{
class Device_Selection_View;
}

/// @brief Main_Window
///
class Main_Window : public QMainWindow
{
  Q_OBJECT

  // Public methods
  public:
  Main_Window ();

  QSize
  sizeHint () const;

  void
  restore_state ();

  void
  save_state ();

  // Public slots
  public slots:

  void
  select_snd_ctl ( const QString & ctl_n );

  void
  refresh ();

  void
  set_fullscreen ( bool flag_n );

  // Device selection

  void
  show_device_selection ( bool flag_n );

  void
  toggle_device_selection ();

  // Protected methods
  protected:
  void
  changeEvent ( QEvent * event_n );

  void
  closeEvent ( QCloseEvent * event_n );

  // Private slots
  private slots:

  void
  select_ctl_from_side_iface ();

  void
  show_info_dialog ();

  // Private methods
  private:
  void
  init_widgets ();

  void
  init_menu_bar ();

  void
  update_fullscreen_action ();

  // Private attributes
  private:
  // Shared storages and settings
  ::Wdg::DS_Widget_Style_Db _wdg_style_db;
  ::dpe::Image_Allocator _image_alloc;

  // Widgets settings
  Main_Window_Setup _setup;

  // Widgets
  QScopedPointer< QSplitter > _splitter;
  ::Views::Mixer_HCTL * _mixer_hctl;
  ::Views::Device_Selection_View * _dev_select;

  QPointer< QDialog > _info_dialog;

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
