/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_tray_mixer_icon_hpp__
#define __INC_tray_mixer_icon_hpp__

#include <QMenu>
#include <QObject>
#include <QSystemTrayIcon>

/// @brief Tray_Mixer_Icon
///
class Tray_Mixer_Icon : public QSystemTrayIcon
{
  Q_OBJECT

  // Public methods
  public:
  Tray_Mixer_Icon ( QObject * parent_n = 0 );

  bool
  event ( QEvent * event_n );

  // Public signals
  signals:

  void
  sig_hover ();

  void
  sig_activated ();

  void
  sig_middle_click ();

  void
  sig_wheel_delta ( int delta_n );

  void
  sig_quit ();

  // Protected slots
  protected slots:

  void
  activation ( QSystemTrayIcon::ActivationReason reason_n );

  // Protected methods
  protected:
  // Private attributes
  private:
  QMenu _cmenu;
};

#endif
