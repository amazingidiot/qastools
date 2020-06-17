/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_hctl_editor_int_hpp__
#define __INC_mixer_hctl_editor_int_hpp__

#include "mwdg/mixer_hctl_editor.hpp"
#include "wdg/scroll_area_horizontal.hpp"
#include "wdg/sliders_pad.hpp"
#include <QAction>
#include <QList>
#include <QMenu>
#include <QPointer>
#include <QScopedPointer>

namespace MWdg
{

// Forward declaration
class Mixer_HCTL_Int_Proxy_Slider;
class Mixer_HCTL_Int_Proxies_Group;
class Mixer_HCTL_Slider_Status_Widget;

/// @brief Mixer_HCTL_Edit_Int
///
class Mixer_HCTL_Edit_Int : public Mixer_HCTL_Editor
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_HCTL_Edit_Int ( ::MWdg::Mixer_HCTL_Editor_Data * data_n,
                        QWidget * parent_n = 0 );

  ~Mixer_HCTL_Edit_Int ();

  void
  clear ();

  void
  rebuild ();

  void
  set_inputs_setup ( const ::MWdg::Inputs_Setup * setup_n );

  void
  update_proxies_values ();

  QString
  integer_string ( long value_n ) const;

  QString
  dB_string ( double value_n ) const;

  bool
  event ( QEvent * event_n );

  bool
  eventFilter ( QObject * watched_n, QEvent * event_n );

  // Protected slots
  protected slots:

  void
  update_focus_proxies ();

  void
  context_menu_cleanup_behind ();

  void
  action_toggle_joined ();

  void
  action_level_volumes ();

  void
  footer_label_selected ( unsigned int group_idx_n, unsigned int column_idx_n );

  // Private methods
  private:
  void
  setup_single ();

  void
  setup_multi ();

  void
  setup_widgets ();

  QLayout *
  create_range_label ();

  ::Wdg::Pad_Proxies_Group *
  create_proxies_group ( ::QSnd::Mixer_HCTL_Elem * elem_n,
                         bool multi_n = false );

  // Context menu

  bool
  context_menu_start ( const QPoint & pos_n );

  /// @return The number of visible actions
  unsigned int
  context_menu_update ();

  // Private attributes
  private:
  QList<::MWdg::Mixer_HCTL_Int_Proxy_Slider * > _proxies_slider;
  QList<::Wdg::Pad_Proxies_Group * > _proxies_groups;

  QScopedPointer<::Wdg::Sliders_Pad > _sliders_pad;
  QScopedPointer<::Wdg::Scroll_Area_Horizontal > _scroll_area;

  // Slider status widget
  QPointer<::MWdg::Mixer_HCTL_Slider_Status_Widget > _status_wdg;
  unsigned int _status_group_idx;
  unsigned int _status_column_idx;

  // Context menu
  QPointer<::MWdg::Mixer_HCTL_Int_Proxies_Group > _focus_proxies_group;
  unsigned int _focus_proxy_column;

  QPointer<::MWdg::Mixer_HCTL_Int_Proxies_Group > _act_proxies_group;
  unsigned int _act_proxy_column;

  QMenu _cmenu;
  QAction _act_toggle_joined;
  QAction _act_level_channels;

  QString _range_mask;
  QString _range_ttip;
  QString _str_int_range;
  QString _str_dB_range;
};

} // namespace MWdg

#endif
