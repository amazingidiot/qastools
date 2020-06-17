/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_tray_mixer_hpp__
#define __INC_tray_mixer_hpp__

#include <QIcon>
#include <QObject>
#include <QScopedPointer>

// Forward declaration
class Tray_Mixer_MDev_Setup;
class Tray_Mixer_View_Setup;
class Tray_Mixer_Balloon;
class Tray_Mixer_Icon;
namespace QSnd
{
class Mixer_Simple;
class Mixer_Simple_Elem;
} // namespace QSnd

/// @brief Tray_Mixer
///
class Tray_Mixer : public QObject
{
  Q_OBJECT

  // Public methods
  public:
  Tray_Mixer ( QObject * parent_n = 0 );

  ~Tray_Mixer ();

  void
  set_mdev_setup ( Tray_Mixer_MDev_Setup * setup_n );

  void
  set_view_setup ( Tray_Mixer_View_Setup * setup_n );

  void
  update_balloon_setup ();

  bool
  is_visible () const;

  bool
  event ( QEvent * event_n );

  // Public signals
  signals:

  void
  sig_toggle_mixer ();

  void
  sig_quit ();

  // Protected slots
  protected slots:

  void
  mixer_values_changed ();

  void
  mixer_toggle_switch ();

  void
  mouse_wheel_delta ( int wheel_delta_n );

  void
  raise_balloon ();

  void
  close_balloon ();

  // Private methods
  private:
  void
  init_icons ();

  void
  update_tray_icon ();

  bool
  update_volume_permille ();

  void
  update_volume_widgets ();

  void
  load_mixer ();

  void
  close_mixer ();

  // Private attributes
  private:
  Tray_Mixer_MDev_Setup * _mdev_setup;
  Tray_Mixer_View_Setup * _view_setup;

  // Mixer
  ::QSnd::Mixer_Simple * _snd_mixer;
  ::QSnd::Mixer_Simple_Elem * _mx_elem;
  unsigned int _volume_permille;

  // Icons and pixmaps
  /// 0: default
  /// 1: muted
  /// 2...: volumes
  unsigned int _icon_idx;
  QList< QIcon > _icons_volume;

  // State flags
  bool _volume_muted;
  bool _updating_scheduled;

  Tray_Mixer_Icon * _tray_icon;
  QScopedPointer< Tray_Mixer_Balloon > _balloon;
};

#endif
