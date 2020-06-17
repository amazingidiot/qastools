/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_views_settings_dialog_hpp__
#define __INC_views_settings_dialog_hpp__

#include "views/multi_page_dialog.hpp"
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QModelIndex>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>
#include <QStackedLayout>
#include <QStandardItem>

// Forward declaration
class Desktop_Items_Setup;

namespace Views
{

/// @brief Settings_Dialog
///
class Settings_Dialog : public ::Views::Multi_Page_Dialog
{
  Q_OBJECT

  // Public methods
  public:
  Settings_Dialog ( QWidget * parent_n = 0, Qt::WindowFlags flags_n = 0 );

  ~Settings_Dialog ();

  void
  set_setup ( ::Desktop_Items_Setup * setup_n );

  // Signals
  signals:

  void
  sig_change_mixer_view ();

  void
  sig_change_input ();

  void
  sig_change_tray_view ();

  void
  sig_change_tray_mdev ();

  void
  sig_change_tray_balloon ();

  void
  sig_close ();

  // Protected slots
  protected slots:

  void
  change_startup ();

  void
  change_appearance ();

  void
  change_input ();

  void
  change_tray_view ();

  void
  change_tray_mdev ();

  void
  change_tray_balloon ();

  void
  wheel_turns_changed ( int value_n );

  void
  wheel_turns_changed ( double value_n );

  // Protected methods
  protected:
  void
  init_page_startup ();

  void
  init_page_appearance ();

  void
  init_page_input ();

  void
  init_page_sys_tray ();

  void
  update_inputs_values ();

  void
  update_inputs_vis_state ();

  // Private attributes
  private:
  ::Desktop_Items_Setup * _dsetup;

  // Pages
  QWidget * _page_startup;
  QWidget * _page_appearance;
  QWidget * _page_input;
  QWidget * _page_sys_tray;

  // Page: Startup
  QButtonGroup * _start_btn_grp_device;
  QRadioButton * _start_btn_device[ 3 ];
  QWidget * _start_dev_user;
  QLineEdit * _start_dev_user_edit;

  // Page: Appearance
  QCheckBox * _btn_show_sliders_labels;

  // Page: Input
  QString _mwheel_degrees_mask;
  QDoubleSpinBox * _mwheel_turns_input;
  QSlider * _mwheel_turns_slider;

  // Page: System tray usage
  QWidget * _tray_minimize;
  QCheckBox * _tray_btn_on_close;
  QCheckBox * _tray_btn_show_icon;

  QWidget * _tray_wdg_grp_ballon;
  QCheckBox * _tray_btn_balloon_show;
  QWidget * _tray_balloon_time;
  QSpinBox * _tray_balloon_time_input;

  QWidget * _tray_wdg_grp_device;
  QButtonGroup * _tray_btn_grp_device;
  QRadioButton * _tray_btn_device[ 3 ];

  QWidget * _tray_dev_user;
  QLineEdit * _tray_dev_user_edit;

  // Layout & Styling
  QLocale loc;
  bool _updating_values;
  unsigned int _vspace;
};

} // namespace Views

#endif
