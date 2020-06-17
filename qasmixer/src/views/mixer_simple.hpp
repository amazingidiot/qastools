/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mv_mixer_simple_hpp__
#define __INC_mv_mixer_simple_hpp__

#include "mwdg/mixer_simple_setup.hpp"
#include "views/view_base.hpp"
#include <QPointer>
#include <QPushButton>
#include <QSplitter>

// Forward declaration
namespace QSnd
{
class Mixer_Simple;
}
namespace MWdg
{
class Mixer_Sliders;
class Mixer_Sliders_Status_Widget;
class Mixer_Switches;
} // namespace MWdg
namespace Views
{
class Mixer_Simple_Setup;
}

namespace Views
{

class Mixer_Simple : public ::Views::View_Base
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_Simple ( QWidget * parent_n = 0 );

  ~Mixer_Simple ();

  // Mixer device and view setup

  void
  set_mdev_setup ( const ::MWdg::Mixer_Device_Setup * setup_n );

  void
  set_inputs_setup ( const ::MWdg::Inputs_Setup * setup_n );

  void
  set_view_setup ( ::Views::View_Base_Setup * setup_n );

  // Public slots
  public slots:

  void
  show_playback ( bool flag_n );

  void
  show_capture ( bool flag_n );

  void
  toggle_show_playback ();

  void
  toggle_show_capture ();

  void
  show_slider_value_widget ();

  // Protected slots
  protected slots:

  void
  footer_label_selected ( unsigned int group_idx_n, unsigned int column_idx_n );

  // Protected methods
  protected:
  void
  keyPressEvent ( QKeyEvent * event_n );

  void
  showEvent ( QShowEvent * event_n );

  void
  hideEvent ( QHideEvent * event_n );

  // Private methods
  private:
  void
  clear_view ();

  void
  setup_view ();

  // Private attributes
  private:
  ::Views::Mixer_Simple_Setup * _view_setup;
  ::MWdg::Mixer_Simple_Setup _smixer_setup;

  // widgets
  QWidget _wdg_areas;
  QSplitter _mixer_split;
  ::MWdg::Mixer_Sliders * _mixer_sliders;

  QWidget * _switches_area;
  QPushButton * _btn_stream[ 2 ];
  ::MWdg::Mixer_Switches * _mixer_switches;

  QPointer<::MWdg::Mixer_Sliders_Status_Widget > _status_wdg;
  unsigned int _status_group_idx;
  unsigned int _status_column_idx;

  /// @brief Mixer object
  ::QSnd::Mixer_Simple * _qsnd_mixer;

  // Strings and Icons
  QString _act_stream_text[ 2 ];
  QString _act_stream_ttip[ 2 ];
  QIcon _act_stream_icon[ 2 ];
};

} // namespace Views

#endif
