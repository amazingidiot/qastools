/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "scroll_area_vertical.hpp"
#include <QCoreApplication>
#include <QEvent>
#include <QScrollBar>
#include <iostream>

namespace Wdg
{

Scroll_Area_Vertical::Scroll_Area_Vertical ( QWidget * parent_n )
: QScrollArea ( parent_n )
{
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
  setWidgetResizable ( true );
}

QSize
Scroll_Area_Vertical::minimumSizeHint () const
{
  QSize res = ( QScrollArea::minimumSizeHint () );

  if ( widget () != 0 ) {
    res.setHeight ( 0 );
    {
      const QSize wmsh ( widget ()->minimumSizeHint () );
      if ( wmsh.width () > 0 ) {
        res.rwidth () += wmsh.width ();
      }
    }
    if ( verticalScrollBar () != 0 ) {
      QSize sb_msh ( verticalScrollBar ()->minimumSizeHint () );
      if ( sb_msh.width () <= 0 ) {
        sb_msh = verticalScrollBar ()->sizeHint ();
      }
      if ( sb_msh.width () > 0 ) {
        res.rwidth () += sb_msh.width ();
      }
    }
    {
      const QMargins mgs ( contentsMargins () );
      res.rwidth () += ( mgs.left () + mgs.right () );
    }
  }

  return res;
}

void
Scroll_Area_Vertical::set_widget ( QWidget * wdg_n )
{
  if ( widget () != 0 ) {
    widget ()->removeEventFilter ( this );
  }

  setWidget ( wdg_n );

  if ( widget () != 0 ) {
    widget ()->installEventFilter ( this );
  }

  updateGeometry ();
}

QWidget *
Scroll_Area_Vertical::take_widget ()
{
  if ( widget () != 0 ) {
    widget ()->removeEventFilter ( this );
  }
  return takeWidget ();
}

bool
Scroll_Area_Vertical::eventFilter ( QObject * watched_n, QEvent * event_n )
{
  if ( watched_n == widget () ) {
    //::std::cout << "Event type: " << event_n->type() << "\n";
    if ( event_n->type () == QEvent::LayoutRequest ) {
      updateGeometry ();
    }
  }

  return false;
}

} // namespace Wdg
