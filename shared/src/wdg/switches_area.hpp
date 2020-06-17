/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_switches_scroll_area_hpp__
#define __INC_switches_scroll_area_hpp__

#include <QFrame>
#include <QScrollArea>

namespace Wdg
{

/// @brief Switches_Area
///
class Switches_Area : public QScrollArea
{
  // Public methods
  public:
  Switches_Area ( QWidget * parent_n = 0 );

  QSize
  minimumSizeHint () const;

  QSize
  sizeHint () const;

  void
  set_widget ( QWidget * wdg_n );

  QWidget *
  take_widget ();

  // Protected methods
  protected:
  void
  resizeEvent ( QResizeEvent * event_n );

  bool
  eventFilter ( QObject * watched_n, QEvent * event_n );
};

} // namespace Wdg

#endif
