/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_sliders_pad_layout_hpp__
#define __INC_sliders_pad_layout_hpp__

#include "sliders_pad_data.hpp"
#include "sliders_pad_header_data.hpp"
#include <QLayout>
#include <QList>
#include <QObject>
#include <QPen>
#include <QScopedPointer>
#include <QWidget>

namespace Wdg
{

// Forward declarations
class Equal_Columns_Layout;

///
/// @brief Sliders_Pad_Layout
///
class Sliders_Pad_Layout : public QLayout
{
  // Public methods
  public:
  Sliders_Pad_Layout ( Sliders_Pad_Data * sp_data_n, QWidget * parent_n = 0 );

  ~Sliders_Pad_Layout ();

  //
  // Size hints
  //

  QSize
  minimumSize () const;

  QSize
  sizeHint () const;

  // Header specific

  Sliders_Pad_Header_Data *
  header_data () const;

  Sliders_Pad_Header_Data *
  footer_data () const;

  // Header labels

  unsigned int
  num_header_labels () const;

  const Sliders_Pad_Header_Label &
  header_label ( unsigned int idx_n ) const;

  Sliders_Pad_Header_Label &
  header_label ( unsigned int idx_n );

  //
  // QLayout methods
  //

  void
  set_header_item ( QLayoutItem * item_n );

  void
  set_header_widget ( QWidget * wdg_n );

  void
  set_footer_item ( QLayoutItem * item_n );

  void
  set_footer_widget ( QWidget * wdg_n );

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
  setGeometry ( const QRect & rect_n );

  // Protected methods
  protected:
  bool
  extra_sub_slider_spacing () const;

  unsigned int
  header_height_hint ( const QLayoutItem * item_n,
                       const Sliders_Pad_Header_Data * hdata_n ) const;

  void
  calc_column_widths_sync ( unsigned int width_n );

  void
  calc_label_angle ( Sliders_Pad_Header_Data * hdata_n,
                     unsigned int lbl_hor_dist_n,
                     bool min_angle_n );

  double
  calc_label_x_center ( const Sliders_Pad_Header_Data * hdata_n,
                        const Sliders_Pad_Data_Group * sp_grp_n,
                        const Sliders_Pad_Data_Column * sp_col_n );

  unsigned int
  calc_labels_max_x ( const Sliders_Pad_Header_Data * hdata_n );

  void
  calc_columns_sizes ( unsigned int area_width_n, unsigned int area_height_n );

  void
  post_adjust_row_heights ();

  void
  set_geometries ( const QRect & crect_n );

  void
  update_labels_transforms ( Sliders_Pad_Header_Data * hdata_n,
                             const QRect & hrect_n );

  // Private attributes;
  private:
  QScopedPointer< QLayoutItem > _header_item;
  QScopedPointer< Equal_Columns_Layout > _lay_eqc;
  QScopedPointer< QLayoutItem > _footer_item;

  unsigned int _num_items;
  QLayoutItem * _items[ 3 ];

  Sliders_Pad_Header_Data * _header_data;
  Sliders_Pad_Header_Data * _footer_data;

  Sliders_Pad_Data * _sp_data;
};

inline Sliders_Pad_Header_Data *
Sliders_Pad_Layout::header_data () const
{
  return _header_data;
}

inline Sliders_Pad_Header_Data *
Sliders_Pad_Layout::footer_data () const
{
  return _footer_data;
}

} // namespace Wdg

#endif
