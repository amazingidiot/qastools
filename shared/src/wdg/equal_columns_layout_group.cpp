/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "equal_columns_layout_group.hpp"

namespace Wdg
{

//
// Equal_Columns_Layout_Row
//
Equal_Columns_Layout_Row::Equal_Columns_Layout_Row ( unsigned int row_idx_n )
: _row_index ( row_idx_n )
, _item ( 0 )
{
}

//
// Equal_Columns_Layout_Column
//
Equal_Columns_Layout_Column::Equal_Columns_Layout_Column (
    unsigned int column_index_n )
: _column_index ( column_index_n )
, _active_rows ( 0 )
, _column_pos ( 0 )
, _column_width ( 0 )
{
}

Equal_Columns_Layout_Column::~Equal_Columns_Layout_Column ()
{
  clear_rows ();
}

void
Equal_Columns_Layout_Column::set_column_pos ( unsigned int pos_n )
{
  _column_pos = pos_n;
}

void
Equal_Columns_Layout_Column::set_column_width ( unsigned int width_n )
{
  _column_width = width_n;
}

void
Equal_Columns_Layout_Column::clear_rows ()
{
  unsigned int rows ( num_rows () );
  if ( rows > 0 ) {
    for ( unsigned int ii = 0; ii < rows; ++ii ) {
      Equal_Columns_Layout_Row * row ( _rows[ ii ] );
      if ( row != 0 ) {
        delete row;
      }
    }
    _rows.clear ();
  }
}

void
Equal_Columns_Layout_Column::append_row ( Equal_Columns_Layout_Row * item_n )
{
  _rows.append ( item_n );
}

void
Equal_Columns_Layout_Column::remove_empty_rows_at_back ()
{
  unsigned int idx ( num_rows () );
  while ( idx > 0 ) {
    --idx;
    Equal_Columns_Layout_Row * row ( _rows[ idx ] );
    bool remove ( true );
    if ( row != 0 ) {
      if ( row->item () != 0 ) {
        remove = false;
      }
    }
    if ( remove ) {
      if ( row != 0 ) {
        delete row;
      }
      _rows.removeLast ();
    } else {
      break;
    }
  }
}

//
// Equal_Columns_Layout_Column
//
Equal_Columns_Layout_Group::Equal_Columns_Layout_Group (
    unsigned int group_idx_n )
: _group_index ( group_idx_n )
, _active_columns ( 0 )
, _group_pos ( 0 )
, _group_width ( 0 )
{
}

Equal_Columns_Layout_Group::~Equal_Columns_Layout_Group ()
{
  clear_columns ();
}

void
Equal_Columns_Layout_Group::set_group_pos ( unsigned int pos_n )
{
  _group_pos = pos_n;
}

void
Equal_Columns_Layout_Group::set_group_width ( unsigned int width_n )
{
  _group_width = width_n;
}

void
Equal_Columns_Layout_Group::clear_columns ()
{
  if ( num_columns () > 0 ) {
    for ( int ii = 0; ii < _columns.size (); ++ii ) {
      delete _columns[ ii ];
    }
    _columns.clear ();
  }
}

void
Equal_Columns_Layout_Group::append_column (
    Equal_Columns_Layout_Column * column_n )
{
  if ( column_n != 0 ) {
    _columns.append ( column_n );
  }
}

void
Equal_Columns_Layout_Group::remove_empty_columns_at_back ()
{
  unsigned int idx ( num_columns () );
  while ( idx > 0 ) {
    --idx;
    Equal_Columns_Layout_Column * col ( _columns[ idx ] );
    bool remove ( true );
    if ( col != 0 ) {
      if ( col->num_rows () != 0 ) {
        remove = false;
      }
    }
    if ( remove ) {
      if ( col != 0 ) {
        delete col;
      }
      _columns.removeLast ();
    } else {
      break;
    }
  }
}

} // namespace Wdg
