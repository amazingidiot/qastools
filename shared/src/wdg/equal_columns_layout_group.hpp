/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_equal_columns_layout_group_hpp__
#define __INC_equal_columns_layout_group_hpp__

#include <QLayoutItem>

namespace Wdg
{

///
/// @brief Equal_Columns_Layout_Column
///
class Equal_Columns_Layout_Row
{
  // Public methods
  public:
  Equal_Columns_Layout_Row ( unsigned int row_idx_n );

  // Row index

  unsigned int
  row_index () const;

  void
  set_row_index ( unsigned int row_idx_n );

  // Layout item

  QLayoutItem *
  item () const;

  void
  set_item ( QLayoutItem * item_n );

  // Private attributes
  private:
  unsigned int _row_index;

  QLayoutItem * _item;
};

inline unsigned int
Equal_Columns_Layout_Row::row_index () const
{
  return _row_index;
}

inline void
Equal_Columns_Layout_Row::set_row_index ( unsigned int row_idx_n )
{
  _row_index = row_idx_n;
}

inline QLayoutItem *
Equal_Columns_Layout_Row::item () const
{
  return _item;
}

inline void
Equal_Columns_Layout_Row::set_item ( QLayoutItem * item_n )
{
  _item = item_n;
}

///
/// @brief Equal_Columns_Layout_Column
///
class Equal_Columns_Layout_Column
{
  // Public methods
  public:
  Equal_Columns_Layout_Column ( unsigned int column_idx_n );

  virtual ~Equal_Columns_Layout_Column ();

  // Column index

  unsigned int
  column_index () const;

  void
  set_column_index ( unsigned int idx_n );

  // Active rows

  unsigned int
  active_rows () const;

  void
  set_active_rows ( unsigned int num_n );

  // Column position

  unsigned int
  column_pos () const;

  void
  set_column_pos ( unsigned int pos_n );

  // Column width

  unsigned int
  column_width () const;

  void
  set_column_width ( unsigned int pos_n );

  // Rows

  void
  clear_rows ();

  void
  append_row ( Equal_Columns_Layout_Row * item_n );

  void
  remove_empty_rows_at_back ();

  unsigned int
  num_rows () const;

  const Equal_Columns_Layout_Row *
  row ( unsigned int idx_n ) const;

  Equal_Columns_Layout_Row *
  row ( unsigned int idx_n );

  // Private attributes
  private:
  unsigned int _column_index;

  unsigned int _active_rows;

  unsigned int _column_pos;

  unsigned int _column_width;

  QList< Equal_Columns_Layout_Row * > _rows;
};

inline unsigned int
Equal_Columns_Layout_Column::column_index () const
{
  return _column_index;
}

inline void
Equal_Columns_Layout_Column::set_column_index ( unsigned int idx_n )
{
  _column_index = idx_n;
}

inline unsigned int
Equal_Columns_Layout_Column::active_rows () const
{
  return _active_rows;
}

inline void
Equal_Columns_Layout_Column::set_active_rows ( unsigned int num_n )
{
  _active_rows = num_n;
}

inline unsigned int
Equal_Columns_Layout_Column::column_pos () const
{
  return _column_pos;
}

inline unsigned int
Equal_Columns_Layout_Column::column_width () const
{
  return _column_width;
}

inline unsigned int
Equal_Columns_Layout_Column::num_rows () const
{
  return _rows.size ();
}

inline const Equal_Columns_Layout_Row *
Equal_Columns_Layout_Column::row ( unsigned int idx_n ) const
{
  return _rows[ idx_n ];
}

inline Equal_Columns_Layout_Row *
Equal_Columns_Layout_Column::row ( unsigned int idx_n )
{
  return _rows[ idx_n ];
}

///
/// @brief Equal_Columns_Layout_Group
///
class Equal_Columns_Layout_Group
{
  // Public methods
  public:
  Equal_Columns_Layout_Group ( unsigned int group_idx_n );

  virtual ~Equal_Columns_Layout_Group ();

  // Group index

  unsigned int
  group_index () const;

  void
  set_group_index ( unsigned int idx_n );

  // Active columns

  unsigned int
  active_columns () const;

  void
  set_active_columns ( unsigned int num_n );

  // Group pos

  unsigned int
  group_pos () const;

  void
  set_group_pos ( unsigned int pos_n );

  // Group width

  unsigned int
  group_width () const;

  void
  set_group_width ( unsigned int pos_n );

  // Columns

  void
  clear_columns ();

  void
  append_column ( Equal_Columns_Layout_Column * column_n );

  void
  remove_empty_columns_at_back ();

  unsigned int
  num_columns () const;

  const Equal_Columns_Layout_Column *
  column ( unsigned int idx_n ) const;

  Equal_Columns_Layout_Column *
  column ( unsigned int idx_n );

  // Row statistics

  QList< unsigned int > &
  row_stats ();

  unsigned int
  num_rows () const;

  unsigned int
  row_stat ( unsigned int idx_n ) const;

  // Private attributes
  private:
  unsigned int _group_index;

  unsigned int _active_columns;

  unsigned int _group_pos;

  unsigned int _group_width;

  QList< Equal_Columns_Layout_Column * > _columns;

  QList< unsigned int > _row_stats;
};

inline unsigned int
Equal_Columns_Layout_Group::group_index () const
{
  return _group_index;
}

inline void
Equal_Columns_Layout_Group::set_group_index ( unsigned int idx_n )
{
  _group_index = idx_n;
}

inline unsigned int
Equal_Columns_Layout_Group::active_columns () const
{
  return _active_columns;
}

inline void
Equal_Columns_Layout_Group::set_active_columns ( unsigned int num_n )
{
  _active_columns = num_n;
}

inline unsigned int
Equal_Columns_Layout_Group::group_pos () const
{
  return _group_pos;
}

inline unsigned int
Equal_Columns_Layout_Group::group_width () const
{
  return _group_width;
}

inline unsigned int
Equal_Columns_Layout_Group::num_columns () const
{
  return _columns.size ();
}

inline const Equal_Columns_Layout_Column *
Equal_Columns_Layout_Group::column ( unsigned int idx_n ) const
{
  return _columns[ idx_n ];
}

inline Equal_Columns_Layout_Column *
Equal_Columns_Layout_Group::column ( unsigned int idx_n )
{
  return _columns[ idx_n ];
}

inline QList< unsigned int > &
Equal_Columns_Layout_Group::row_stats ()
{
  return _row_stats;
}

inline unsigned int
Equal_Columns_Layout_Group::num_rows () const
{
  return _row_stats.size ();
}

inline unsigned int
Equal_Columns_Layout_Group::row_stat ( unsigned int idx_n ) const
{
  return _row_stats[ idx_n ];
}

} // namespace Wdg

#endif
