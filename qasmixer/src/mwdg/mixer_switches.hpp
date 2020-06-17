/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mwdg_mixer_switches_hpp__
#define __INC_mwdg_mixer_switches_hpp__

#include <QMenu>
#include <QPointer>
#include <QTimer>
#include <QWidget>

// Forward declaration
namespace MWdg
{
class Inputs_Setup;
class Mixer_Simple_Setup;
class Mixer_Switches_Proxies_Group;
class Mixer_GUI_State_Proxy;
} // namespace MWdg
namespace Wdg
{
class Switches_Pad;
class Pad_Proxy;
class Pad_Proxies_Column;
class Pad_Proxies_Group;
class Switches_Area;
} // namespace Wdg

namespace MWdg
{

/// @brief Mixer_Switches
///
class Mixer_Switches : public QWidget
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_Switches ( QWidget * parent_n = 0 );

  ~Mixer_Switches ();

  // Mixer setup

  const Mixer_Simple_Setup *
  mixer_setup () const;

  void
  set_mixer_setup ( const Mixer_Simple_Setup * setup_n );

  // Inputs setup

  const ::MWdg::Inputs_Setup *
  inputs_setup () const;

  void
  set_inputs_setup ( const ::MWdg::Inputs_Setup * setup_n );

  unsigned int
  num_visible () const;

  // Event handling

  bool
  event ( QEvent * event_n );

  // Protected slots
  protected slots:

  void
  action_toggle_joined ();

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
  setup_proxies_group_joined ( ::MWdg::Mixer_Switches_Proxies_Group * mspg_n );

  void
  setup_proxies_group_separate (
      ::MWdg::Mixer_Switches_Proxies_Group * mspg_n );

  void
  create_proxy ( ::MWdg::Mixer_Switches_Proxies_Group * mspg_n,
                 int channel_idx_n );

  // Proxy group manipulation

  bool
  should_be_visible (
      const ::MWdg::Mixer_Switches_Proxies_Group * mspg_n ) const;

  void
  toggle_joined_separated ( ::MWdg::Mixer_Switches_Proxies_Group * mspg_n );

  void
  join_proxies_group ( ::MWdg::Mixer_Switches_Proxies_Group * mspg_n );

  void
  separate_proxies_group ( ::MWdg::Mixer_Switches_Proxies_Group * mspg_n );

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

  ::MWdg::Mixer_Switches_Proxies_Group *
  find_visible_proxy ( const ::MWdg::Mixer_GUI_State_Proxy & prox_id_n );

  // Context menu

  bool
  context_menu_start ( const QPoint & pos_n );

  unsigned int
  context_menu_update ();

  // Event callbacks

  bool
  eventFilter ( QObject * watched, QEvent * event );

  /// @brief Switches pad getter
  ///
  const ::Wdg::Switches_Pad *
  switches_pad () const;

  /// @brief Switches pad getter
  ///
  ::Wdg::Switches_Pad *
  switches_pad ();

  // Private attributes
  private:
  const ::MWdg::Mixer_Simple_Setup * _mixer_setup;
  const ::MWdg::Inputs_Setup * _inputs_setup;

  QList<::MWdg::Mixer_Switches_Proxies_Group * > _proxies_groups;
  QList<::MWdg::Mixer_Switches_Proxies_Group * > _proxies_groups_vis;
  QList<::Wdg::Pad_Proxies_Group * > _proxies_groups_pass;

  ::Wdg::Switches_Area * _switches_area;
  ::Wdg::Switches_Pad * _switches_pad;

  // Flags
  bool _separation_requested;

  // Action focus proxy
  QPointer<::MWdg::Mixer_Switches_Proxies_Group > _act_proxies_group;
  unsigned int _act_proxies_column;

  // Context menu
  QMenu _cmenu;
  QAction _act_toggle_joined;
};

inline const Mixer_Simple_Setup *
Mixer_Switches::mixer_setup () const
{
  return _mixer_setup;
}

inline const ::MWdg::Inputs_Setup *
Mixer_Switches::inputs_setup () const
{
  return _inputs_setup;
}

inline unsigned int
Mixer_Switches::num_visible () const
{
  return _proxies_groups_vis.size ();
}

inline const ::Wdg::Switches_Pad *
Mixer_Switches::switches_pad () const
{
  return _switches_pad;
}

inline Wdg::Switches_Pad *
Mixer_Switches::switches_pad ()
{
  return _switches_pad;
}

} // namespace MWdg

#endif
