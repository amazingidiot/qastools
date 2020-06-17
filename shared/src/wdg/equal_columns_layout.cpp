/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "equal_columns_layout.hpp"
#include "equal_columns_layout_group.hpp"
#include "layout_weights.hpp"
#include <QFontMetrics>
#include <iostream>

namespace Wdg
{

///
/// @brief Equal_Columns_Layout_Index
///
class Equal_Columns_Layout_Index
{
  // Public methods
  public:
  Equal_Columns_Layout_Index ();

  // Attributes
  QLayoutItem * item;
  Equal_Columns_Layout_Group * group;
  Equal_Columns_Layout_Column * column;
  Equal_Columns_Layout_Row * row;
};

Equal_Columns_Layout_Index::Equal_Columns_Layout_Index ()
: item ( 0 )
, group ( 0 )
, column ( 0 )
, row ( 0 )
{
}

//
// Equal_Columns_Layout
//
Equal_Columns_Layout::Equal_Columns_Layout ( QWidget * parent_n )
: QLayout ( parent_n )
, _smallest_group_dist ( 0 )
, _smallest_column_dist ( 0 )
, _spacing_vertical ( 7 )
, _cache_dirty ( true )
{
  _col_type_min_widths[ 0 ] = 3; // Horizontal spacing minor
  _col_type_min_widths[ 1 ] = 6; // Horizontal spacing major
  _col_type_min_widths[ 2 ] = 0; // Content column minor
  _col_type_min_widths[ 3 ] = 0; // Content column major

  _col_type_weights[ 0 ] = 4; // Horizontal spacing minor
  _col_type_weights[ 1 ] = 9; // Horizontal spacing major
  _col_type_weights[ 2 ] =
      _col_type_weights[ 1 ] * 5 / 3; // Content column minor
  _col_type_weights[ 3 ] =
      _col_type_weights[ 1 ] * 8 / 3; // Content column major

  update_cache ();
}

Equal_Columns_Layout::~Equal_Columns_Layout ()
{
  if ( _groups.size () > 0 ) {
    for ( int ii = 0; ii < _groups.size (); ++ii ) {
      delete _groups[ ii ];
    }
    _groups.clear ();
  }

  if ( _indices.size () > 0 ) {
    for ( int ii = 0; ii < _indices.size (); ++ii ) {
      if ( _indices[ ii ]->item != 0 ) {
        delete _indices[ ii ]->item;
      }
      delete _indices[ ii ];
    }
    _indices.clear ();
  }
}

//
// QLayout methods
//

int
Equal_Columns_Layout::add_group_item ( QLayoutItem * item_n,
                                       unsigned int group_idx_n,
                                       unsigned int column_idx_n,
                                       unsigned int row_idx_n )
{
  bool res ( -1 );

  if ( item_n == 0 ) {
    return res;
  }

  // Create new groups on demand
  while ( (int)group_idx_n >= _groups.size () ) {
    unsigned int idx ( _groups.size () );
    _groups.append ( new ::Wdg::Equal_Columns_Layout_Group ( idx ) );
  }

  Equal_Columns_Layout_Group * grp ( _groups[ group_idx_n ] );

  // Create a new columns on demand
  while ( column_idx_n >= grp->num_columns () ) {
    unsigned int idx ( grp->num_columns () );
    grp->append_column ( new ::Wdg::Equal_Columns_Layout_Column ( idx ) );
  }

  ::Wdg::Equal_Columns_Layout_Column * col ( grp->column ( column_idx_n ) );

  // Create a new rows on demand
  while ( row_idx_n >= col->num_rows () ) {
    unsigned int idx ( col->num_rows () );
    col->append_row ( new ::Wdg::Equal_Columns_Layout_Row ( idx ) );
  }

  ::Wdg::Equal_Columns_Layout_Row * row ( col->row ( row_idx_n ) );

  // Add item
  if ( row->item () == 0 ) {
    row->set_item ( item_n );

    ::Wdg::Equal_Columns_Layout_Index * lindex (
        new Equal_Columns_Layout_Index );
    lindex->item = item_n;
    lindex->group = grp;
    lindex->column = col;
    lindex->row = row;

    _indices.append ( lindex );

    invalidate ();
    res = 0;

  } else {
    //::std::cout << "Shouldn't happen " << row->item() << "\n";
  }

  return res;
}

int
Equal_Columns_Layout::add_group_widget ( QWidget * wdg_n,
                                         unsigned int group_idx,
                                         unsigned int column_idx,
                                         unsigned int row_idx_n )
{
  int res ( -1 );
  if ( wdg_n != 0 ) {
    addChildWidget ( wdg_n );
    QWidgetItem * item ( new QWidgetItem ( wdg_n ) );
    res = add_group_item ( item, group_idx, column_idx, row_idx_n );
    if ( res != 0 ) {
      delete item;
    }
  }
  return res;
}

void
Equal_Columns_Layout::addItem ( QLayoutItem * item_n )
{
  if ( item_n != 0 ) {
    add_group_item ( item_n, _groups.size (), 0, 0 );
  }
}

QLayoutItem *
Equal_Columns_Layout::itemAt ( int idx_n ) const
{
  QLayoutItem * res ( 0 );
  if ( ( idx_n >= 0 ) && ( idx_n < _indices.size () ) ) {
    res = _indices[ idx_n ]->item;
  }
  return res;
}

QLayoutItem *
Equal_Columns_Layout::takeAt ( int idx_n )
{
  QLayoutItem * res ( 0 );

  //::std::cout << "Equal_Columns_Layout::takeAt " << idx_n << "\n";

  if ( ( idx_n >= 0 ) && ( idx_n < _indices.size () ) ) {

    Equal_Columns_Layout_Index * lindex ( _indices[ idx_n ] );
    _indices.removeAt ( idx_n );

    if ( lindex->row != 0 ) {
      lindex->row->set_item ( 0 );
    }
    if ( lindex->column != 0 ) {
      lindex->column->remove_empty_rows_at_back ();
    }
    if ( lindex->group != 0 ) {
      lindex->group->remove_empty_columns_at_back ();
    }
    remove_empty_groups_at_back ();

    res = lindex->item;
    delete lindex;

    _groups_active.clear ();

    invalidate ();
  }

  return res;
}

int
Equal_Columns_Layout::count () const
{
  return _indices.size ();
}

void
Equal_Columns_Layout::remove_empty_groups_at_back ()
{
  unsigned int idx ( _groups.size () );
  while ( idx > 0 ) {
    --idx;
    Equal_Columns_Layout_Group * grp ( _groups[ idx ] );
    bool remove ( true );
    if ( grp != 0 ) {
      if ( grp->num_columns () > 0 ) {
        remove = false;
      }
    }
    if ( remove ) {
      if ( grp != 0 ) {
        delete grp;
      }
      _groups.removeLast ();
    } else {
      break;
    }
  }
}

QSize
Equal_Columns_Layout::minimumSize () const
{
  update_cache_const ();

  QSize res ( 0, 0 );
  int & ww ( res.rwidth () );
  int & hh ( res.rheight () );

  { // Margins
    const QMargins mgs ( contentsMargins () );
    ww += mgs.left () + mgs.right ();
    hh += mgs.top () + mgs.bottom ();
  }

  // Width
  for ( unsigned int ii = 0; ii < 4; ++ii ) {
    ww += _col_type_count[ ii ] * _col_type_min_widths[ ii ];
  }

  // Height
  for ( int ii = 0; ii < _row_min_heights.size (); ++ii ) {
    hh += _row_min_heights[ ii ];
  }
  if ( _rows_active > 0 ) {
    hh += ( _rows_active - 1 ) * _spacing_vertical;
  }

  //::std::cout << "Equal_Columns_Layout::minimumSize " << res.width() << ":" <<
  //res.height() << "\n";

  return res;
}

QSize
Equal_Columns_Layout::sizeHint () const
{
  return minimumSize ();
}

void
Equal_Columns_Layout::invalidate ()
{
  //::std::cout << "Equal_Columns_Layout::invalidate" << "\n";

  _cache_dirty = true;
  QLayout::invalidate ();
}

unsigned int
Equal_Columns_Layout::all_rows_height () const
{
  unsigned int res ( 0 );
  for ( unsigned int ii = 0; ii < num_rows (); ++ii ) {
    res += row_height ( ii );
  }
  if ( _rows_active > 0 ) {
    res += ( _rows_active - 1 ) * spacing_vertical ();
  }
  return res;
}

void
Equal_Columns_Layout::set_row_height ( unsigned int row_idx_n,
                                       unsigned int height_n )
{
  _row_heights[ row_idx_n ] = height_n;
}

void
Equal_Columns_Layout::update_cache_const () const
{
  const_cast< Equal_Columns_Layout * > ( this )->update_cache ();
}

unsigned int
Equal_Columns_Layout::minor_spacing_max_width (
    unsigned int major_sp_width_n ) const
{
  unsigned int res;
  unsigned int min_width ( major_sp_width_n * _col_type_weights[ 0 ] );
  res = min_width / _col_type_weights[ 1 ];
  if ( ( min_width % _col_type_weights[ 1 ] ) > 0 ) {
    ++res;
  }
  return res;
}

void
Equal_Columns_Layout::update_cache ()
{
  if ( !_cache_dirty ) {
    return;
  }
  _cache_dirty = false;

  //::std::cout << "Equal_Columns_Layout::update_cache" << "\n";

  // Reset buffers
  _groups_active.clear ();
  for ( int ii = 0; ii < 4; ++ii ) {
    _col_type_count[ ii ] = 0;
  }
  _col_type_min_widths[ 2 ] = 0;
  _col_type_min_widths[ 3 ] = 0;
  _row_min_heights.clear ();
  _row_heights.clear ();
  _row_stretch.clear ();
  _rows_active = 0;

  for ( int gii = 0; gii < _groups.size (); ++gii ) {
    Equal_Columns_Layout_Group * grp ( _groups[ gii ] );
    if ( grp == 0 ) {
      continue;
    }

    grp->row_stats ().clear ();

    if ( grp->num_columns () == 0 ) {
      continue;
    }

    unsigned int active_cols ( 0 );
    unsigned int cmin_width ( 0 );

    for ( unsigned int cii = 0; cii < grp->num_columns (); ++cii ) {
      Equal_Columns_Layout_Column * col ( grp->column ( cii ) );
      if ( col == 0 ) {
        continue;
      }
      if ( col->num_rows () == 0 ) {
        continue;
      }

      // Adjust row heights buffer on demand
      while ( _row_min_heights.size () < (int)col->num_rows () ) {
        _row_min_heights.append ( 0 );
        _row_heights.append ( 0 );
        _row_stretch.append ( 0 );
      }

      // Adjust row statistics buffer size
      while ( grp->row_stats ().size () < (int)col->num_rows () ) {
        grp->row_stats ().append ( 0 );
      }

      unsigned int active_rows ( 0 );

      for ( unsigned int rii = 0; rii < col->num_rows (); ++rii ) {
        const Equal_Columns_Layout_Row * row ( col->row ( rii ) );
        if ( row == 0 ) {
          continue;
        }
        if ( row->item () == 0 ) {
          continue;
        }
        if ( row->item ()->isEmpty () ) {
          continue;
        }

        grp->row_stats ()[ rii ] += 1;
        ++active_rows;

        {
          Qt::Orientations ori ( row->item ()->expandingDirections () );
          if ( ( ori & Qt::Vertical ) != 0 ) {
            _row_stretch[ rii ] = 1;
          }
        }

        const QSize msize ( row->item ()->minimumSize () );
        if ( msize.height () > (int)_row_min_heights[ rii ] ) {
          _row_min_heights[ rii ] = msize.height ();
        }
        if ( msize.width () > (int)cmin_width ) {
          cmin_width = msize.width ();
        }
      }

      col->set_active_rows ( active_rows );
      if ( col->active_rows () > 0 ) {
        ++active_cols;
      }
    }

    grp->set_active_columns ( active_cols );
    if ( active_cols > 0 ) {
      _groups_active.append ( grp );

      _col_type_count[ 1 ] += 1; // Spacing major
      if ( active_cols == 1 ) {
        // Single major content column
        _col_type_count[ 3 ] += 1;
        if ( cmin_width > _col_type_min_widths[ 3 ] ) {
          _col_type_min_widths[ 3 ] = cmin_width;
        }
      } else {
        // More than one minor content columns
        _col_type_count[ 0 ] += ( active_cols - 1 );
        _col_type_count[ 2 ] += active_cols;
        if ( cmin_width > _col_type_min_widths[ 2 ] ) {
          _col_type_min_widths[ 2 ] = cmin_width;
        }
      }
    }
  }

  // Remove on major spacing
  if ( _col_type_count[ 1 ] > 0 ) {
    --_col_type_count[ 1 ];
  }

  for ( int ii = 0; ii < _row_min_heights.size (); ++ii ) {
    if ( _row_min_heights[ ii ] > 0 ) {
      ++_rows_active;
    }
  }
}

void
Equal_Columns_Layout::calc_columns_sizes ( unsigned int area_width_n,
                                           unsigned int area_height_n )
{
  calc_row_heights ( area_height_n );
  calc_column_widths ( area_width_n );
}

void
Equal_Columns_Layout::calc_row_heights ( unsigned int height_n )
{
  //::std::cout << "Equal_Columns_Layout::calc_row_heights " << height_n <<
  //"\n";

  unsigned int avail_space ( height_n );
  if ( _rows_active > 0 ) {
    avail_space -= ( _rows_active - 1 ) * _spacing_vertical;
  }

  unsigned int expanding_rows ( 0 );
  for ( int ii = 0; ii < _row_stretch.size (); ++ii ) {
    if ( _row_stretch[ ii ] == 0 ) {
      _row_heights[ ii ] = _row_min_heights[ ii ];
      if ( avail_space >= _row_min_heights[ ii ] ) {
        avail_space -= _row_min_heights[ ii ];
      } else {
        avail_space = 0;
      }
    } else {
      ++expanding_rows;
    }
  }

  // Share available space between expanding rows equally
  if ( ( expanding_rows > 0 ) && ( avail_space > 0 ) ) {
    unsigned int def_height ( avail_space / expanding_rows );
    unsigned int height_mod ( avail_space % expanding_rows );
    unsigned int exp_row ( 0 );
    for ( int ii = 0; ii < _row_stretch.size (); ++ii ) {
      if ( _row_stretch[ ii ] != 0 ) {
        unsigned int rheight ( def_height );
        if ( exp_row < height_mod ) {
          ++rheight;
        }
        _row_heights[ ii ] = rheight;
        ++exp_row;
      }
    }
  }

  // Calculate maximum column width
  {
    unsigned int max_width ( 0 );
    for ( int ii = 0; ii < _row_heights.size (); ++ii ) {
      if ( _row_heights[ ii ] > max_width ) {
        max_width = _row_heights[ ii ];
      }
    }
    max_width = ( max_width / 5 );
    {
      unsigned int max_width_font ( 0 );
      {
        QFont fnt;
        QFontMetrics fmet ( fnt );
        max_width_font = fmet.height () * 3;
      }
      if ( max_width > max_width_font ) {
        max_width = max_width_font;
      }
    }
    for ( int ii = 0; ii < 4; ++ii ) {
      _col_max_widths[ ii ] = max_width;
    }
    _col_max_widths[ 0 ] = minor_spacing_max_width ( max_width );
    // Crop to limits
    for ( int ii = 0; ii < 4; ++ii ) {
      if ( _col_type_count[ ii ] > 0 ) {
        if ( _col_max_widths[ ii ] < _col_type_min_widths[ ii ] ) {
          _col_max_widths[ ii ] = _col_type_min_widths[ ii ];
        }
      } else {
        _col_max_widths[ ii ] = 0;
      }
    }
  }
}

void
Equal_Columns_Layout::calc_column_widths ( unsigned int width_n )
{
  //::std::cout << "Equal_Columns_Layout::calc_column_widths " << width_n <<
  //"\n";

  _smallest_group_dist = 0;
  _smallest_column_dist = 0;

  if ( num_active_groups () == 0 ) {
    return;
  }
  if ( ( _col_type_count[ 2 ] == 0 ) && ( _col_type_count[ 3 ] == 0 ) ) {
    return;
  }

  const unsigned long avail_space ( width_n );
  for ( unsigned int ii = 0; ii < 4; ++ii ) {
    _col_widths[ ii ] = 0;
  }

  // Calculate a first estimate for the column widths
  {
    Layout_Weights weights_all;
    for ( unsigned int ii = 0; ii < 4; ++ii ) {
      weights_all[ ii ] = _col_type_weights[ ii ] * _col_type_count[ ii ];
    }

    const unsigned long wsum ( weights_all.sum () );

    // Calculate first estimate for the column widths
    for ( unsigned int ii = 0; ii < 4; ++ii ) {
      if ( _col_type_count[ ii ] > 0 ) {
        _col_widths[ ii ] = ( weights_all[ ii ] * avail_space ) /
                            ( wsum * _col_type_count[ ii ] );
        _col_widths[ ii ] =
            qMax ( _col_widths[ ii ], _col_type_min_widths[ ii ] );
      }
    }
  }

  // Crop estimates to allowed range
  for ( unsigned int ii = 0; ii < 4; ++ii ) {
    if ( _col_type_count[ ii ] > 0 ) {
      if ( _col_widths[ ii ] < _col_type_min_widths[ ii ] ) {
        _col_widths[ ii ] = _col_type_min_widths[ ii ];
      }
      if ( _col_widths[ ii ] > _col_max_widths[ ii ] ) {
        _col_widths[ ii ] = _col_max_widths[ ii ];
      }
    }
  }

  // Calculate used space
  unsigned int used_space ( 0 );
  for ( unsigned int ii = 0; ii < 4; ++ii ) {
    used_space += _col_widths[ ii ] * _col_type_count[ ii ];
  }

  // Take from all if too much space has been taken
  for ( unsigned int ii = 0; ii < 100; ++ii ) {
    if ( used_space <= avail_space ) {
      break;
    }

    // The estimate is too big
    bool changed ( false );
    for ( unsigned int tii = 0; tii < 4; ++tii ) {
      if ( ( _col_widths[ tii ] > _col_type_min_widths[ tii ] ) &&
           ( used_space >= _col_type_count[ tii ] ) ) {
        _col_widths[ tii ] -= 1;
        used_space -= _col_type_count[ tii ];
        changed = true;
      }
    }
    if ( !changed ) {
      break;
    }
  }

  // If now not all space is used after shrinking try to expand
  if ( used_space < avail_space ) {

    unsigned int space_delta ( avail_space - used_space );
    unsigned int min_sp_max ( minor_spacing_max_width ( _col_widths[ 1 ] ) );

    // Normalize single weights for comparison
    Layout_Weights col_weights_norm ( _col_type_weights );
    for ( int ii = 0; ii < 4; ++ii ) {
      if ( _col_type_count[ ii ] == 0 ) {
        col_weights_norm[ ii ] = 0;
      }
    }
    col_weights_norm.normalize ();

    const unsigned int invalid_type ( ~0 );
    while ( space_delta > 0 ) {

      // Find maximum delta
      unsigned int increment_type ( invalid_type );

      unsigned int num_less;
      unsigned int weight_delta_order[ 4 ];
      {
        Layout_Weights col_widths_norm ( _col_widths );
        col_widths_norm.normalize ();
        num_less = calc_weights_delta_order (
            weight_delta_order, col_weights_norm, col_widths_norm );
      }

      for ( unsigned int ii = 0; ii < num_less; ++ii ) {
        unsigned int idx ( weight_delta_order[ ii ] );
        if ( ( _col_type_count[ idx ] > 0 ) &&
             ( _col_type_count[ idx ] <= space_delta ) &&
             ( _col_widths[ idx ] < _col_max_widths[ idx ] ) ) {
          increment_type = idx;
          break;
        }
      }

      // Add to spacing as a fallback
      if ( increment_type == invalid_type ) {
        for ( unsigned int ii = 0; ii < 4; ++ii ) {
          unsigned int idx ( weight_delta_order[ ii ] );
          if ( idx < 2 ) {
            if ( ( _col_type_count[ idx ] > 0 ) &&
                 ( _col_type_count[ idx ] <= space_delta ) &&
                 ( _col_widths[ idx ] < _col_max_widths[ idx ] ) ) {
              if ( ( idx == 1 ) || ( _col_widths[ 0 ] < min_sp_max ) ) {
                increment_type = idx;
                break;
              }
            }
          }
        }
      }

      if ( increment_type != invalid_type ) {
        _col_widths[ increment_type ] += 1;
        used_space += _col_type_count[ increment_type ];
        space_delta = avail_space - used_space;
        if ( increment_type == 1 ) { // Major spacing grew
          min_sp_max = minor_spacing_max_width ( _col_widths[ 1 ] );
        }
      } else {
        break;
      }
    }
  }

  //
  // Set position and with for every active group / column
  //

  if ( _groups_active.size () == 0 ) {
    return;
  }

  // Write columns positioning data
  unsigned int spare_space ( avail_space - used_space );
  unsigned int smallest_group_width ( width_n );
  bool extra_space_major ( ( spare_space > 0 ) &&
                           ( _col_type_count[ 1 ] > 0 ) &&
                           ( _col_widths[ 1 ] < _col_max_widths[ 1 ] ) );
  bool extra_space_minor ( ( spare_space > 0 ) &&
                           ( _col_type_count[ 0 ] > 0 ) &&
                           ( _col_type_count[ 1 ] == 0 ) &&
                           ( _col_widths[ 0 ] < _col_max_widths[ 0 ] ) );

  // extra_space_start indicates the group or column index from which on
  // a single pixel horizontal space can be added to distribute the spare_space
  unsigned int extra_space_start ( ~0 );
  if ( extra_space_major ) {
    extra_space_start = _groups_active.size () - 1;
  } else if ( extra_space_minor ) {
    if ( _groups_active[ 0 ]->num_columns () > 0 ) {
      extra_space_start = _groups_active[ 0 ]->num_columns () - 1;
    }
  }
  if ( spare_space > 0 ) {
    if ( extra_space_start > spare_space ) {
      extra_space_start -= spare_space;
    } else {
      extra_space_start = 0;
    }
  }

  unsigned int pos ( 0 );
  unsigned int num_act_grps ( _groups_active.size () );
  for ( unsigned int gii = 0; gii < num_act_grps; ++gii ) {
    Equal_Columns_Layout_Group * grp ( _groups_active[ gii ] );

    unsigned long col_width_cur;
    if ( grp->active_columns () == 1 ) {
      col_width_cur = _col_widths[ 3 ]; // Major slider
    } else {
      col_width_cur = _col_widths[ 2 ]; // Minor slider
    }

    unsigned int group_pos ( pos );

    {
      const unsigned int num_cols ( grp->num_columns () );
      unsigned int act_col_idx ( 0 );
      for ( unsigned int cii = 0; cii < num_cols; ++cii ) {
        Equal_Columns_Layout_Column * col ( grp->column ( cii ) );
        if ( col->active_rows () > 0 ) {
          if ( act_col_idx > 0 ) {
            pos += _col_widths[ 0 ]; // Spacing minor
            if ( extra_space_minor && ( act_col_idx > extra_space_start ) ) {
              ++pos;
            }
          }
          col->set_column_pos ( pos );
          col->set_column_width ( col_width_cur );

          pos += col_width_cur;
          ++act_col_idx;
        }
      }
    }

    unsigned int group_width ( pos - group_pos );

    grp->set_group_pos ( group_pos );
    grp->set_group_width ( group_width );

    pos += _col_widths[ 1 ]; // Spacing major
    if ( extra_space_major && ( gii >= extra_space_start ) ) {
      ++pos;
    }

    if ( ( group_width > 0 ) && ( group_width < smallest_group_width ) ) {
      smallest_group_width = group_width;
    }
  }

  for ( unsigned int ii = 0; ii < 4; ++ii ) {
    _col_widths[ ii ] = _col_widths[ ii ];
  }

  _smallest_group_dist = smallest_group_width + _col_widths[ 1 ];
  if ( _col_type_count[ 0 ] > 0 ) {
    _smallest_column_dist = _col_widths[ 0 ] + _col_widths[ 2 ];
  } else {
    _smallest_column_dist = _smallest_group_dist;
  }
}

void
Equal_Columns_Layout::setGeometry ( const QRect & rect_n )
{
  QLayout::setGeometry ( rect_n );
  if ( rect_n.isValid () ) {
    QRect crect ( rect_n );
    {
      const QMargins & mgs ( contentsMargins () );
      crect.adjust ( mgs.left (), mgs.top (), -mgs.right (), -mgs.bottom () );
    }
    if ( crect.isValid () ) {
      update_cache ();
      if ( _rows_active > 0 ) {
        calc_columns_sizes ( crect.width (), crect.height () );
        set_geometries ( crect );
      }
    }
  }
}

void
Equal_Columns_Layout::set_geometries ( const QRect & crect_n )
{
  // Set widgets geometries

  int y_pos ( crect_n.top () );
  unsigned int num_rows ( _row_heights.size () );
  for ( unsigned int rii = 0; rii < num_rows; ++rii ) {
    if ( _row_heights[ rii ] == 0 ) {
      continue;
    }

    unsigned int items_set ( 0 );

    QRect row_rect (
        crect_n.left (), y_pos, crect_n.width (), _row_heights[ rii ] );

    unsigned int num_act_grps ( _groups_active.size () );
    for ( unsigned int gii = 0; gii < num_act_grps; ++gii ) {
      const Equal_Columns_Layout_Group * grp ( _groups_active[ gii ] );
      if ( grp == 0 ) {
        continue;
      }

      // Place items normally
      const unsigned int num_cols ( grp->num_columns () );
      for ( unsigned int cii = 0; cii < num_cols; ++cii ) {
        const Equal_Columns_Layout_Column * col ( grp->column ( cii ) );
        if ( col == 0 ) {
          continue;
        }
        if ( col->num_rows () <= rii ) {
          continue;
        }

        const Equal_Columns_Layout_Row * row ( col->row ( rii ) );
        if ( row == 0 ) {
          continue;
        }
        if ( row->item () == 0 ) {
          continue;
        }
        if ( row->item ()->isEmpty () ) {
          continue;
        }

        set_geometry_row ( row_rect, grp, col, row );
        ++items_set;
      }
    }

    if ( items_set > 0 ) {
      y_pos += row_rect.height ();
      y_pos += _spacing_vertical;
    }
  }
}

void
Equal_Columns_Layout::set_geometry_row (
    const QRect & row_rect_n,
    const Equal_Columns_Layout_Group * grp_n,
    const Equal_Columns_Layout_Column * col_n,
    const Equal_Columns_Layout_Row * row_n )
{
  QRect re_wdg ( row_rect_n.left () + col_n->column_pos (),
                 row_rect_n.top (),
                 col_n->column_width (),
                 row_rect_n.height () );

  // Count type numbers

  unsigned int num_sliders ( 0 );
  unsigned int num_switches ( 0 );
  if ( grp_n->num_rows () > 0 ) {
    num_sliders = grp_n->row_stat ( 0 );
    if ( grp_n->num_rows () > 1 ) {
      num_switches = grp_n->row_stat ( 1 );
    }
  }

  bool center_widget ( false );
  if ( row_n->row_index () == 0 ) {
    // Center slider on demand
    if ( ( num_sliders == 1 ) && ( num_switches > 1 ) ) {
      center_widget = true;
    }
  } else if ( row_n->row_index () == 1 ) {
    // Center switch on demand
    if ( ( num_sliders > 1 ) && ( num_switches == 1 ) ) {
      center_widget = true;
    }
  }

  if ( center_widget ) {
    // Center widget horizontally in group
    int xx ( grp_n->group_pos () );
    xx += ( grp_n->group_width () - re_wdg.width () ) / 2;
    re_wdg.moveLeft ( xx );
  }

  //::std::cout << "re_wdg " << re_wdg.left() << " " << re_wdg.top() << " " <<
  //re_wdg.width() << " " << re_wdg.height() << "\n";

  row_n->item ()->setGeometry ( re_wdg );
}

} // namespace Wdg
