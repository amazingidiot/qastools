/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "switches_area.hpp"
#include "wdg/fill_columns_layout.hpp"
#include "wdg/switches_pad.hpp"
#include <QEvent>
#include <QScrollBar>
#include <iostream>

namespace Wdg
{

Switches_Area::Switches_Area ( QWidget * parent_n )
: QScrollArea ( parent_n )
{
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
  setWidgetResizable ( true );
}

QSize
Switches_Area::minimumSizeHint () const
{
  ensurePolished ();

  if ( widget () == 0 ) {
    return QScrollArea::minimumSizeHint ();
  }

  QSize res ( 0, 0 );

  // Height
  {
    ::Wdg::Switches_Pad * sw_pad (
        dynamic_cast<::Wdg::Switches_Pad * > ( widget () ) );

    const ::Wdg::Fill_Columns_Layout * lay_cols (
        dynamic_cast< const ::Wdg::Fill_Columns_Layout * > (
            widget ()->layout () ) );

    {
      // Number of visible rows
      unsigned int num_vis ( 4 );
      if ( sw_pad != 0 ) {
        num_vis = sw_pad->num_proxies_groups ();
      }

      // Number of minimum visible rows
      unsigned int min_rows ( 4 );
      if ( num_vis < 6 ) {
        min_rows = 3;
      }
      if ( num_vis < 2 ) {
        min_rows = 2;
      }

      res.rheight () += widget ()->fontMetrics ().height () * min_rows;
      if ( lay_cols != 0 ) {
        res.rheight () +=
            lay_cols->vertical_spacing_default () * ( min_rows - 1 );
      }
    }
    if ( lay_cols != 0 ) {
      QMargins mgs ( lay_cols->contentsMargins () );
      res.rheight () += mgs.top () + mgs.bottom ();
    }
  }

  // Widget width
  {
    QSize wdg_msh ( widget ()->minimumSizeHint () );
    if ( wdg_msh.width () > 0 ) {
      res.rwidth () += wdg_msh.width ();
    }
  }

  // Scrollbar
  if ( verticalScrollBar () != 0 ) {
    QSize sb_msh ( verticalScrollBar ()->minimumSizeHint () );
    if ( sb_msh.width () <= 0 ) {
      sb_msh = verticalScrollBar ()->sizeHint ();
    }
    if ( sb_msh.width () > 0 ) {
      res.rwidth () += sb_msh.width ();
    }
  }

  // Contents margins
  {
    QMargins mgs ( contentsMargins () );
    res.rwidth () += mgs.left () + mgs.right ();
    res.rheight () += mgs.top () + mgs.bottom ();
  }

  return res;
}

QSize
Switches_Area::sizeHint () const
{
  return minimumSizeHint ();
}

void
Switches_Area::set_widget ( QWidget * wdg_n )
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
Switches_Area::take_widget ()
{
  if ( widget () != 0 ) {
    widget ()->removeEventFilter ( this );
  }
  return takeWidget ();
}

void
Switches_Area::resizeEvent ( QResizeEvent * event_n )
{
  if ( widget () != 0 ) {
    ::Wdg::Switches_Pad * spad (
        dynamic_cast<::Wdg::Switches_Pad * > ( widget () ) );
    if ( spad != 0 ) {
      spad->set_viewport_geometry ( viewport ()->contentsRect () );
    }
  }
  QScrollArea::resizeEvent ( event_n );
}

bool
Switches_Area::eventFilter ( QObject * watched_n, QEvent * event_n )
{
  if ( watched_n == widget () ) {
    if ( event_n->type () == QEvent::LayoutRequest ) {
      //::std::cout << "Switches_Area::eventFilter: " << event_n->type() <<
      //"\n";
      updateGeometry ();
    }
  }

  return false;
}

} // namespace Wdg
