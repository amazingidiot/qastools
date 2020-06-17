/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "scroll_area_horizontal.hpp"
#include <QCoreApplication>
#include <QEvent>
#include <QScrollBar>
#include <iostream>

namespace Wdg
{

Scroll_Area_Horizontal::Scroll_Area_Horizontal ( QWidget * parent_n )
: QScrollArea ( parent_n )
{
  setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
  setWidgetResizable ( true );
}

QSize
Scroll_Area_Horizontal::minimumSizeHint () const
{
  QSize res = ( QScrollArea::minimumSizeHint () );

  if ( widget () != 0 ) {
    res.setHeight ( 0 );
    {
      const QSize wmsh ( widget ()->minimumSizeHint () );
      if ( wmsh.height () > 0 ) {
        res.rheight () += wmsh.height ();
      }
    }
    if ( horizontalScrollBar () != 0 ) {
      QSize sb_msh ( horizontalScrollBar ()->minimumSizeHint () );
      if ( sb_msh.height () <= 0 ) {
        sb_msh = horizontalScrollBar ()->sizeHint ();
      }
      if ( sb_msh.height () > 0 ) {
        res.rheight () += sb_msh.height ();
      }
    }
    {
      const QMargins mgs ( contentsMargins () );
      res.rheight () += ( mgs.top () + mgs.bottom () );
    }
  }

  return res;
}

void
Scroll_Area_Horizontal::set_widget ( QWidget * wdg_n )
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
Scroll_Area_Horizontal::take_widget ()
{
  if ( widget () != 0 ) {
    widget ()->removeEventFilter ( this );
  }
  return takeWidget ();
}

bool
Scroll_Area_Horizontal::eventFilter ( QObject * watched_n, QEvent * event_n )
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
