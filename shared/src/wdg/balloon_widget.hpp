/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_balloon_widget_hpp__
#define __INC_balloon_widget_hpp__

#include <QFrame>
#include <QTimer>

namespace Wdg
{

/// @brief Balloon_Widget
///
class Balloon_Widget : public QWidget
{
  Q_OBJECT

  // Public methods
  public:
  Balloon_Widget ( QWidget * parent_n = 0 );

  // Duration

  unsigned int
  duration_ms () const;

  void
  set_duration_ms ( unsigned int ms_n );

  /// @brief Stay visible if widget is under mouse
  ///
  /// @return The flag - true by default
  bool
  stay_on_hover () const;

  void
  set_stay_on_hover ( bool flag_n );

  // Tray icon geometry

  const QRect &
  tray_icon_geometry () const;

  void
  set_tray_icon_geometry ( const QRect & geom_n );

  // Widgets

  void
  add_widget ( QWidget * wdg_n );

  // Public signals
  signals:

  void
  sig_close ();

  // Public slots
  public slots:

  void
  start_show ();

  // Protected methods
  protected:
  void
  leaveEvent ( QEvent * event_n );

  void
  resizeEvent ( QResizeEvent * event_n );

  void
  paintEvent ( QPaintEvent * event_n );

  // Private slots
  private slots:

  void
  close_timeout ();

  // Private methods
  private:
  void
  update_mask ();

  void
  update_pixmap ();

  void
  update_geometry ();

  // Private attributes
  private:
  QPixmap _pxmap;
  QRect _tray_icon_geom;
  QTimer _close_timer;

  static const unsigned int _border_width = 1;
  static const unsigned int _corner_rad = 6;

  bool _update_pxmap;
  bool _stay_on_hover;
  bool _remains_on_hover;
};

inline bool
Balloon_Widget::stay_on_hover () const
{
  return _stay_on_hover;
}

inline const QRect &
Balloon_Widget::tray_icon_geometry () const
{
  return _tray_icon_geom;
}

} // namespace Wdg

#endif
