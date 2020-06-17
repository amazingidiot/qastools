/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mwdg_mixer_sliders_hpp__
#define __INC_mwdg_mixer_sliders_hpp__

#include <QAction>
#include <QMenu>
#include <QPointer>
#include <QString>

// Forward declaration
namespace MWdg
{
class Inputs_Setup;
class Mixer_Simple_Setup;
class Mixer_Sliders_Proxies_Group;
class Mixer_GUI_State;
class Mixer_GUI_State_Proxy;
} // namespace MWdg
namespace Wdg
{
class Sliders_Pad;
class Pad_Proxies_Group;
class Scroll_Area_Horizontal;
} // namespace Wdg

namespace MWdg
{

/// @brief Mixer_Sliders
///
class Mixer_Sliders : public QWidget
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_Sliders ( QWidget * parent_n = 0 );

  ~Mixer_Sliders ();

  // Mixer setup

  const ::MWdg::Mixer_Simple_Setup *
  mixer_setup () const;

  void
  set_mixer_setup ( const ::MWdg::Mixer_Simple_Setup * setup_n );

  // Inputs setup

  const ::MWdg::Inputs_Setup *
  inputs_setup () const;

  void
  set_inputs_setup ( const ::MWdg::Inputs_Setup * setup_n );

  /// @brief Number of visualized proxies groups
  ///
  unsigned int
  num_visible () const;

  ::Wdg::Sliders_Pad *
  sliders_pad ();

  // Event handling

  bool
  event ( QEvent * event_n );

  bool
  eventFilter ( QObject * watched, QEvent * event );

  // Signals
  signals:

  void
  sig_footer_label_selected ( unsigned int group_idx_n,
                              unsigned int column_idx_n );

  // Protected slots
  protected slots:

  void
  action_toggle_joined ();

  void
  action_level_volumes ();

  void
  action_toggle_mute ();

  void
  update_focus_proxies ();

  void
  context_menu_cleanup_behind ();

  // Protected methods
  protected:
  // Proxy group creation / deletion

  void
  clear_proxies_groups ();

  void
  create_proxies_groups ();

  void
  setup_proxies_group_joined ( ::MWdg::Mixer_Sliders_Proxies_Group * mspg_n );

  void
  setup_proxies_group_separate ( ::MWdg::Mixer_Sliders_Proxies_Group * mspg_n );

  bool
  create_proxies_group ( ::MWdg::Mixer_Sliders_Proxies_Group * mspg_n,
                         unsigned int channel_idx_n );

  // Proxy group manipulation

  bool
  should_be_visible (
      const ::MWdg::Mixer_Sliders_Proxies_Group * mspg_n ) const;

  void
  toggle_joined_separated ( ::MWdg::Mixer_Sliders_Proxies_Group * mspg_n );

  void
  join_proxies_group ( ::MWdg::Mixer_Sliders_Proxies_Group * mspg_n );

  void
  separate_proxies_group ( ::MWdg::Mixer_Sliders_Proxies_Group * mspg_n );

  // Proxy group showing hiding

  void
  show_visible_proxies_sets ( bool flag_n );

  // Proxy group updating

  void
  separate_where_requested ();

  void
  rebuild_visible_proxies_list ();

  // Focus proxy

  void
  acquire_gui_state ( ::MWdg::Mixer_GUI_State_Proxy & state_n );

  void
  restore_gui_state ( const ::MWdg::Mixer_GUI_State_Proxy & state_n );

  Mixer_Sliders_Proxies_Group *
  find_visible_proxy ( const ::MWdg::Mixer_GUI_State_Proxy & prox_id_n );

  // Context menu

  bool
  context_menu_start ( const QPoint & pos_n );

  /// @return The number of visible actions
  unsigned int
  context_menu_update ();

  // Private attributes
  private:
  const ::MWdg::Mixer_Simple_Setup * _mixer_setup;
  const ::MWdg::Inputs_Setup * _inputs_setup;

  QList<::MWdg::Mixer_Sliders_Proxies_Group * > _proxies_groups;
  QList<::MWdg::Mixer_Sliders_Proxies_Group * > _proxies_groups_vis;
  QList<::Wdg::Pad_Proxies_Group * > _proxies_groups_pass;

  ::Wdg::Scroll_Area_Horizontal * _sliders_area;
  ::Wdg::Sliders_Pad * _sliders_pad;

  // Flags
  bool _separation_requested;

  // Action focus proxy
  QPointer< Mixer_Sliders_Proxies_Group > _act_proxies_group;
  unsigned int _act_proxies_column;

  // Context menu
  QMenu _cmenu;
  QAction _act_toggle_joined;
  QAction _act_level_channels;
  QAction _act_separator_channels;
  QAction _act_toggle_mute;

  // Strings and Icons
  QString _act_str_mute[ 2 ];
  QString _act_str_unmute[ 2 ];
  QString _act_str_toggle_mute;
  QString _ttip_slider[ 2 ];
  QString _ttip_switch[ 2 ];

  QIcon _icon_vol_high;
  QIcon _icon_vol_med;
  QIcon _icon_muted;
};

inline const Mixer_Simple_Setup *
Mixer_Sliders::mixer_setup () const
{
  return _mixer_setup;
}

inline const ::MWdg::Inputs_Setup *
Mixer_Sliders::inputs_setup () const
{
  return _inputs_setup;
}

inline unsigned int
Mixer_Sliders::num_visible () const
{
  return _proxies_groups_vis.size ();
}

inline ::Wdg::Sliders_Pad *
Mixer_Sliders::sliders_pad ()
{
  return _sliders_pad;
}

} // namespace MWdg

#endif
