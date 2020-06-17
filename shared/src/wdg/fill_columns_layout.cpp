/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "fill_columns_layout.hpp"
#include <QApplication>
#include <QWidget>
#include <iostream>

namespace Wdg
{

Fill_Columns_Layout::Fill_Columns_Layout ( QWidget * parent_n )
: QLayout ( parent_n )
, _hspace ( -1 )
, _vspace ( -1 )
, _cache_dirty ( true )
{
}

Fill_Columns_Layout::~Fill_Columns_Layout ()
{
  for ( int ii = 0; ii < _items.size (); ++ii ) {
    delete _items[ ii ].item;
  }
}

QSize
Fill_Columns_Layout::minimumSize () const
{
  update_cache_const ();

  QSize res ( 0, 0 );

  const unsigned int num_items ( _items.size () );
  for ( unsigned int ii = 0; ii < num_items; ++ii ) {
    const QSize & ish ( _items[ ii ].min_size );
    res.rwidth () = qMax ( ish.width (), res.width () );
    res.rheight () = qMax ( ish.height (), res.height () );
  }

  {
    const QMargins mgs ( contentsMargins () );
    res.rwidth () += mgs.left () + mgs.right ();
    res.rheight () += mgs.top () + mgs.bottom ();
  }

  return res;
}

QSize
Fill_Columns_Layout::sizeHint () const
{
  return minimumSize ();
}

bool
Fill_Columns_Layout::hasHeightForWidth () const
{
  return true;
}

int
Fill_Columns_Layout::heightForWidth ( int width ) const
{
  QSize nsz ( 0, 0 );
  if ( width > 0 ) {
    unsigned int aheight ( 65536 );
    if ( _viewport.height () > 0 ) {
      aheight = _viewport.height ();
    }

    nsz = const_cast< Fill_Columns_Layout * > ( this )->calc_columns (
        width, aheight );
  }

  return ( nsz.height () );
}

int
Fill_Columns_Layout::horizontal_spacing () const
{
  int res ( -1 );
  if ( _hspace >= 0 ) {
    res = _hspace;
  } else {
    QObject * pobj ( parent () );
    if ( pobj != 0 ) {
      QStyle * style ( 0 );
      QWidget * pwdg ( dynamic_cast< QWidget * > ( pobj ) );
      if ( pwdg != 0 ) {
        style = pwdg->style ();
      }
      if ( style != 0 ) {
        res = pwdg->style ()->pixelMetric (
            QStyle::PM_LayoutHorizontalSpacing, 0, pwdg );
      } else {
        res = static_cast< QLayout * > ( pobj )->spacing ();
      }
    }
  }
  return res;
}

int
Fill_Columns_Layout::vertical_spacing () const
{
  int res ( -1 );
  if ( _vspace >= 0 ) {
    res = _vspace;
  } else {
    QObject * pobj ( parent () );
    if ( pobj != 0 ) {
      QStyle * style ( 0 );
      QWidget * pwdg ( dynamic_cast< QWidget * > ( pobj ) );
      if ( pwdg != 0 ) {
        style = pwdg->style ();
      }
      if ( style != 0 ) {
        res = style->pixelMetric ( QStyle::PM_LayoutVerticalSpacing, 0, pwdg );
      } else {
        res = static_cast< QLayout * > ( pobj )->spacing ();
      }
    }
  }
  return res;
}

unsigned int
Fill_Columns_Layout::horizontal_spacing_default () const
{
  int hspace ( horizontal_spacing () );
  if ( hspace < 0 ) {
    hspace = spacing_default ( Qt::Horizontal );
  }
  return hspace;
}

unsigned int
Fill_Columns_Layout::vertical_spacing_default () const
{
  int vspace ( vertical_spacing () );
  if ( vspace < 0 ) {
    vspace = spacing_default ( Qt::Vertical );
  }
  return vspace;
}

unsigned int
Fill_Columns_Layout::spacing_default ( Qt::Orientation orient_n ) const
{
  int res ( 0 );

  QWidget * pwdg ( parentWidget () );
  if ( pwdg != 0 ) {
    QStyle * style ( pwdg->style () );
    if ( style != 0 ) {
      res = style->combinedLayoutSpacing ( QSizePolicy::DefaultType,
                                           QSizePolicy::DefaultType,
                                           orient_n,
                                           0,
                                           pwdg );
      if ( res < 0 ) {
        res = 0;
      }
    }
  }

  return res;
}

QLayoutItem *
Fill_Columns_Layout::itemAt ( int index ) const
{
  QLayoutItem * res ( 0 );
  if ( ( index >= 0 ) && ( index < _items.size () ) ) {
    res = _items[ index ].item;
  }
  return res;
}

int
Fill_Columns_Layout::count () const
{
  return _items.size ();
}

void
Fill_Columns_Layout::addItem ( QLayoutItem * item_n )
{
  if ( item_n != 0 ) {
    Fill_Columns_Layout_Item fitem;
    fitem.item = item_n;
    _items.push_back ( fitem );
  }
}

QLayoutItem *
Fill_Columns_Layout::takeAt ( int index_n )
{
  QLayoutItem * res ( 0 );
  if ( ( index_n >= 0 ) && ( index_n < _items.size () ) ) {
    res = _items[ index_n ].item;
    _items.removeAt ( index_n );
  }
  return res;
}

void
Fill_Columns_Layout::invalidate ()
{
  _cache_dirty = true;
  QLayout::invalidate ();
}

void
Fill_Columns_Layout::set_viewport_geometry ( const QRect & rect_n )
{
  if ( _viewport != rect_n ) {
    _viewport = rect_n;
    invalidate ();
  }
}

void
Fill_Columns_Layout::setGeometry ( const QRect & rect_n )
{
  QLayout::setGeometry ( rect_n );

  if ( !rect_n.isValid () ) {
    return;
  }

  unsigned int aheight ( 65536 );
  if ( _viewport.isValid () ) {
    int hh ( _viewport.top () + _viewport.height () - rect_n.top () );
    if ( hh > 0 ) {
      aheight = hh;
    }
  }
  calc_columns ( rect_n.width (), aheight );

  const unsigned int num_items ( _items.size () );
  for ( unsigned int ii = 0; ii < num_items; ++ii ) {
    Fill_Columns_Layout_Item & fitem ( _items[ ii ] );
    if ( !fitem.item->isEmpty () ) {
      QRect irect ( fitem.rect );
      irect.moveTop ( irect.top () + rect_n.top () );
      irect.moveLeft ( irect.left () + rect_n.left () );
      fitem.item->setGeometry ( irect );
    }
  }
}

QSize
Fill_Columns_Layout::calc_columns ( unsigned int width_n,
                                    unsigned int height_n )
{
  update_cache ();

  //::std::cout << "Fill_Columns_Layout::calc_columns " << width_n << "\n";

  QSize res;

  unsigned int x_left ( 0 );
  unsigned int x_limit ( width_n );
  unsigned int y_top ( 0 );
  unsigned int y_limit ( height_n );

  // Adjust start points and limits
  {
    QMargins mgs ( contentsMargins () );
    if ( mgs.left () > 0 ) {
      x_left = mgs.left ();
    }
    if ( mgs.right () > 0 ) {
      if ( (int)x_limit > mgs.right () ) {
        x_limit -= mgs.right ();
      } else {
        x_limit = 0;
      }
    }
    if ( mgs.top () > 0 ) {
      y_top += mgs.top ();
    }
    if ( mgs.bottom () > 0 ) {
      if ( (int)y_limit > mgs.bottom () ) {
        y_limit -= mgs.bottom ();
      } else {
        y_limit = 0;
      }
    }
    res.setWidth ( mgs.right () );
    res.setHeight ( mgs.bottom () );
  }

  //::std::cout << "Viewport " << _viewport.width() << ":" << _viewport.height()
  //<< "\n";
  //::std::cout << "y_top " << y_top << "\n";
  //::std::cout << "y_max " << y_max << "\n";

  const unsigned int fixed_hspace ( horizontal_spacing () );
  const unsigned int fixed_vspace ( vertical_spacing () );

  QStyle * style ( 0 );
  QWidget * pwdg ( parentWidget () );
  if ( pwdg != 0 ) {
    style = pwdg->style ();
  }

  QSizePolicy::ControlTypes ctl_types1;
  QSizePolicy::ControlTypes ctl_types2;

  unsigned int x_pos ( x_left );
  unsigned int x_pos_max ( x_left );
  unsigned int y_pos ( y_top );
  unsigned int y_pos_max ( y_top );

  unsigned int row_idx ( 0 );

  const unsigned int num_items ( _items.size () );
  for ( unsigned int ii = 0; ii < num_items; ++ii ) {
    Fill_Columns_Layout_Item & fitem ( _items[ ii ] );
    if ( fitem.item->isEmpty () ) {
      continue;
    }

    ctl_types1 = ctl_types2;
    ctl_types2 = fitem.item->controlTypes ();

    // Add vertical spacing
    if ( row_idx > 0 ) {
      int vspace ( fixed_vspace );

      if ( ( vspace < 0 ) && ( style != 0 ) ) {
        vspace = style->combinedLayoutSpacing (
            ctl_types1, ctl_types2, Qt::Vertical, 0, pwdg );
      }
      if ( vspace < 0 ) {
        vspace = 0;
      }
      y_pos += vspace;
    }

    const unsigned int iwidth ( qMax ( 0, fitem.min_size.width () ) );
    const unsigned int iheight ( qMax ( 0, fitem.min_size.height () ) );
    unsigned int y_bot ( y_pos + iheight );

    // Start new column on demand
    if ( ( y_bot > y_limit ) && ( row_idx > 0 ) ) {

      // Find broadest following widget
      unsigned int wmax ( 0 );
      for ( unsigned int jj = ii; jj < num_items; ++jj ) {
        if ( !_items[ jj ].item->isEmpty () ) {
          int iww ( _items[ jj ].min_size.width () );
          if ( ( iww > 0 ) && ( iww > (int)wmax ) ) {
            wmax = iww;
          }
        }
      }

      int hspace ( fixed_hspace );
      if ( ( hspace < 0 ) && ( style != 0 ) ) {
        hspace = style->combinedLayoutSpacing (
            ctl_types1, ctl_types2, Qt::Horizontal, 0, pwdg );
      }
      if ( hspace < 0 ) {
        hspace = 0;
      }

      // Start new column if it fits into the give width
      if ( ( x_pos_max + hspace + wmax ) <= x_limit ) {
        // New column
        x_pos = x_pos_max + hspace;
        x_pos_max = x_pos;
        y_pos = y_top;
        row_idx = 0;
      }
    }

    fitem.rect.moveLeft ( x_pos );
    fitem.rect.moveTop ( y_pos );
    fitem.rect.setWidth ( iwidth );
    fitem.rect.setHeight ( iheight );

    y_pos += iheight;

    // Update maxima storages
    {
      unsigned int tmp ( x_pos + iwidth );
      if ( x_pos_max < tmp ) {
        x_pos_max = tmp;
      }
      if ( y_pos_max < y_pos ) {
        y_pos_max = y_pos;
      }
    }

    ++row_idx;
  }

  // Adjust vertical align
  if ( y_pos_max < y_limit ) {
    unsigned int delta ( ( y_limit - y_pos_max ) / 2 );
    y_pos_max += delta;
    for ( unsigned int ii = 0; ii < num_items; ++ii ) {
      Fill_Columns_Layout_Item & fitem ( _items[ ii ] );
      fitem.rect.moveTop ( fitem.rect.top () + delta );
    }
  }

  res.rwidth () += qMax ( 0, (int)x_pos_max - 1 );
  res.rheight () += qMax ( 0, (int)y_pos_max - 1 );

  return res;
}

void
Fill_Columns_Layout::update_cache_const () const
{
  const_cast< Fill_Columns_Layout * > ( this )->update_cache ();
}

void
Fill_Columns_Layout::update_cache ()
{
  if ( !_cache_dirty ) {
    return;
  }
  _cache_dirty = false;

  const unsigned int num_items ( _items.size () );
  for ( unsigned int ii = 0; ii < num_items; ++ii ) {
    _items[ ii ].min_size = _items[ ii ].item->minimumSize ();
  }
}

} // namespace Wdg
