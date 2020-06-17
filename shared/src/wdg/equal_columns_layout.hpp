/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_equal_columns_layout_hpp__
#define __INC_equal_columns_layout_hpp__

#include <QLayout>
#include <QList>
#include <QObject>
#include <QWidget>
#include <iostream>
#include <vector>

namespace Wdg
{

// Forward declaration
class Equal_Columns_Layout_Group;
class Equal_Columns_Layout_Column;
class Equal_Columns_Layout_Row;
class Equal_Columns_Layout_Index;

///
/// @brief Equal_Columns_Layout
///
class Equal_Columns_Layout : public QLayout
{
  // Public methods
  public:
  Equal_Columns_Layout ( QWidget * parent_n = 0 );

  ~Equal_Columns_Layout ();

  //
  // Size hints
  //

  QSize
  minimumSize () const;

  QSize
  sizeHint () const;

  //
  // Variable reader
  //

  unsigned int
  num_groups () const;

  const Equal_Columns_Layout_Group *
  group ( unsigned int idx_n ) const;

  unsigned int
  num_active_groups () const;

  const Equal_Columns_Layout_Group *
  active_group ( unsigned int idx_n ) const;

  int
  spacing_vertical () const;

  unsigned int
  smallest_group_dist () const;

  unsigned int
  smallest_column_dist () const;

  unsigned int
  num_rows () const;

  unsigned int
  row_min_height ( unsigned int idx_n ) const;

  unsigned int
  row_height ( unsigned int idx_n ) const;

  unsigned int
  all_rows_height () const;

  unsigned int
  row_pos ( unsigned int idx_n ) const;

  unsigned int
  col_type_width ( unsigned int type_n );

  //
  // QLayout methods
  //

  int
  add_group_item ( QLayoutItem * item_n,
                   unsigned int group_idx_n,
                   unsigned int column_idx_n,
                   unsigned int row_idx_n );

  int
  add_group_widget ( QWidget * wdg_n,
                     unsigned int group_idx,
                     unsigned int column_idx,
                     unsigned int row_idx_n );

  void
  addItem ( QLayoutItem * item_n );

  QLayoutItem *
  itemAt ( int index_n ) const;

  QLayoutItem *
  takeAt ( int index_n );

  int
  count () const;

  void
  invalidate ();

  void
  setGeometry ( const QRect & rect_n );

  // Implementation methods
  // Direct use is not recommended

  void
  update_cache_const () const;

  void
  update_cache ();

  /// update_cache should be called before this once
  void
  calc_columns_sizes ( unsigned int area_width_n, unsigned int area_height_n );

  void
  calc_row_heights ( unsigned int height_n );

  void
  calc_column_widths ( unsigned int width_n );

  void
  set_row_height ( unsigned int row_idx_n, unsigned int height_n );

  void
  set_geometries ( const QRect & crect_n );

  void
  set_geometry_row ( const QRect & row_rect_n,
                     const Equal_Columns_Layout_Group * grp_n,
                     const Equal_Columns_Layout_Column * col_n,
                     const Equal_Columns_Layout_Row * row_n );

  // Protected methods
  protected:
  // Utility

  void
  remove_empty_groups_at_back ();

  unsigned int
  minor_spacing_max_width ( unsigned int major_sp_width_n ) const;

  // Private attributes;
  private:
  QList< Equal_Columns_Layout_Index * > _indices;

  QList< Equal_Columns_Layout_Group * > _groups;
  QList< Equal_Columns_Layout_Group * > _groups_active;

  unsigned int _smallest_group_dist;
  unsigned int _smallest_column_dist;

  unsigned int _col_type_count[ 4 ];
  unsigned int _col_type_weights[ 4 ];
  unsigned int _col_type_min_widths[ 4 ];
  unsigned int _col_max_widths[ 4 ];
  unsigned int _col_widths[ 4 ];

  QList< unsigned int > _row_min_heights;
  QList< unsigned int > _row_heights;
  QList< unsigned int > _row_stretch;
  unsigned int _rows_active;

  int _spacing_vertical;

  bool _cache_dirty;
};

inline unsigned int
Equal_Columns_Layout::num_groups () const
{
  return _groups.size ();
}

inline const Equal_Columns_Layout_Group *
Equal_Columns_Layout::group ( unsigned int idx_n ) const
{
  return _groups[ idx_n ];
}

inline unsigned int
Equal_Columns_Layout::num_active_groups () const
{
  return _groups_active.size ();
}

inline const Equal_Columns_Layout_Group *
Equal_Columns_Layout::active_group ( unsigned int idx_n ) const
{
  return _groups_active[ idx_n ];
}

inline unsigned int
Equal_Columns_Layout::num_rows () const
{
  return _row_heights.size ();
}

inline unsigned int
Equal_Columns_Layout::row_height ( unsigned int idx_n ) const
{
  return _row_heights[ idx_n ];
}

inline unsigned int
Equal_Columns_Layout::row_min_height ( unsigned int idx_n ) const
{
  return _row_min_heights[ idx_n ];
}

inline unsigned int
Equal_Columns_Layout::row_pos ( unsigned int idx_n ) const
{
  unsigned int res ( 0 );
  if ( idx_n > 0 ) {
    for ( unsigned int ii = 0; ii <= idx_n; ++ii ) {
      res += _row_heights[ ii ];
      res += spacing_vertical ();
    }
  }
  return res;
}

inline unsigned int
Equal_Columns_Layout::col_type_width ( unsigned int type_n )
{
  return _col_widths[ type_n ];
}

inline int
Equal_Columns_Layout::spacing_vertical () const
{
  return _spacing_vertical;
}

inline unsigned int
Equal_Columns_Layout::smallest_group_dist () const
{
  return _smallest_group_dist;
}

inline unsigned int
Equal_Columns_Layout::smallest_column_dist () const
{
  return _smallest_column_dist;
}

} // namespace Wdg

#endif
