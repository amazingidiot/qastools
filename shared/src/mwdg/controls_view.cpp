/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "controls_view.hpp"
#include "mwdg/controls_delegate.hpp"
#include "mwdg/event_types.hpp"
#include <QCoreApplication>
#include <QScrollBar>
#include <iostream>

namespace MWdg
{

Controls_View::Controls_View ( QWidget * parent_n )
: QListView ( parent_n )
, _show_rows_min ( 3 )
, _show_rows_avrg ( 10 )
, _min_chars_vis ( 12 )
, _maximum_update_requested ( false )
{
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
  {
    QSizePolicy policy ( sizePolicy () );
    policy.setVerticalPolicy ( QSizePolicy::Expanding );
    setSizePolicy ( policy );
  }

  setSelectionMode ( QAbstractItemView::SingleSelection );
  setItemDelegate ( new ::MWdg::Controls_Delegate ( this ) );
}

Controls_View::~Controls_View () {}

QSize
Controls_View::minimumSizeHint () const
{
  ensurePolished ();

  QSize res;
  int & ww ( res.rwidth () );
  int & hh ( res.rheight () );

  QSize vsb_msh ( verticalScrollBar ()->minimumSizeHint () );
  QSize vsb_sh ( verticalScrollBar ()->sizeHint () );

  // Width
  {
    ww = fontMetrics ().averageCharWidth () * _min_chars_vis;
    ww = qMax ( 16, ww );
    if ( spacing () > 0 ) {
      ww += spacing () * 2;
    }
    if ( vsb_msh.width () > 0 ) {
      ww += vsb_msh.width ();
    } else if ( vsb_sh.width () > 0 ) {
      ww += vsb_sh.width ();
    }
  }

  // Height
  {
    int rh0 ( sizeHintForRow ( 0 ) );
    rh0 = qMax ( rh0,
                 itemDelegate ()
                     ->sizeHint ( viewOptions (), QModelIndex () )
                     .height () );
    if ( spacing () > 0 ) {
      rh0 += spacing () * 2;
    }
    hh = rh0 * _show_rows_min;
    hh = qMax ( vsb_msh.height (), hh );
  }

  QMargins mg ( contentsMargins () );
  ww += mg.left () + mg.right ();
  hh += mg.top () + mg.bottom ();

  return res;
}

QSize
Controls_View::sizeHint () const
{
  ensurePolished ();

  QSize res;
  int & ww ( res.rwidth () );
  int & hh ( res.rheight () );

  QSize vsb_sh ( verticalScrollBar ()->sizeHint () );

  // Width
  {
    ww = fontMetrics ().averageCharWidth () * ( _min_chars_vis * 3 / 2 );
    if ( spacing () > 0 ) {
      ww += spacing () * 2;
    }
    if ( vsb_sh.width () > 0 ) {
      ww += vsb_sh.width ();
    }
  }

  // Height
  {
    int rh0 ( sizeHintForRow ( 0 ) );
    rh0 = qMax ( rh0,
                 itemDelegate ()
                     ->sizeHint ( viewOptions (), QModelIndex () )
                     .height () );
    if ( spacing () > 0 ) {
      rh0 += spacing () * 2;
    }
    hh = rh0 * _show_rows_avrg;
    hh = qMax ( vsb_sh.height (), hh );
  }

  QMargins mg ( contentsMargins () );
  ww += mg.left () + mg.right ();
  hh += mg.top () + mg.bottom ();

  return res;
}

void
Controls_View::setModel ( QAbstractItemModel * model_n )
{
  if ( model () != 0 ) {
    disconnect ( model (), 0, this, 0 );
  }

  QListView::setModel ( model_n );

  if ( model () != 0 ) {
    maximum_height_update_request ();

    connect ( model (),
              SIGNAL ( modelReset () ),
              this,
              SLOT ( maximum_height_update_request () ) );

    connect ( model (),
              SIGNAL ( layoutChanged () ),
              this,
              SLOT ( maximum_height_update_request () ) );

    connect ( model (),
              SIGNAL ( rowsInserted ( const QModelIndex &, int, int ) ),
              this,
              SLOT ( maximum_height_update_request () ) );

    connect ( model (),
              SIGNAL ( rowsRemoved ( const QModelIndex &, int, int ) ),
              this,
              SLOT ( maximum_height_update_request () ) );
  }
}

void
Controls_View::maximum_height_update_request ()
{
  if ( !_maximum_update_requested ) {
    _maximum_update_requested = true;

    QCoreApplication::postEvent ( this,
                                  new QEvent ( ::MWdg::evt_refresh_data ) );
  }
}

bool
Controls_View::event ( QEvent * event_n )
{
  bool res ( false );
  if ( event_n->type () == ::MWdg::evt_refresh_data ) {
    _maximum_update_requested = false;
    maximum_height_update ();
    res = true;
  } else {
    res = QListView::event ( event_n );
  }
  return res;
}

void
Controls_View::changeEvent ( QEvent * event_n )
{
  maximum_height_update_request ();
  QListView::changeEvent ( event_n );
}

void
Controls_View::maximum_height_update ()
{
  unsigned int hsum ( 16000 );
  unsigned int vspace ( 0 );
  if ( spacing () > 0 ) {
    vspace = spacing () * 2;
  }
  if ( model () != 0 ) {
    hsum = 0;
    hsum += frameWidth () * 2;

    for ( int ii = 0; ii < model ()->rowCount (); ++ii ) {
      int hrow ( sizeHintForRow ( ii ) );
      if ( hrow <= 0 ) {
        hrow = sizeHintForRow ( 0 );
      }
      hrow += vspace;
      if ( hrow > 0 ) {
        hsum += hrow;
      }
    }
  }
  //::std::cout << "Controls_View::maximum_height_update " << hsum << "\n";

  if ( maximumHeight () != (int)hsum ) {
    setMaximumHeight ( hsum );
  }
}

} // namespace MWdg
