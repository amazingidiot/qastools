/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "sliders_pad_layout.hpp"
#include "equal_columns_layout.hpp"
#include "equal_columns_layout_group.hpp"
#include "sliders_pad_header.hpp"

#define _USE_MATH_DEFINES
#include <algorithm>
#include <cmath>
#include <iostream>

namespace Wdg
{

Sliders_Pad_Layout::Sliders_Pad_Layout ( Sliders_Pad_Data * sp_data_n,
                                         QWidget * parent_n )
: QLayout ( parent_n )
, _num_items ( 0 )
, _header_data ( 0 )
, _footer_data ( 0 )
, _sp_data ( sp_data_n )
{
  _lay_eqc.reset ( new ::Wdg::Equal_Columns_Layout );
  _lay_eqc->setContentsMargins ( 0, 0, 0, 0 );
  _items[ 0 ] = _lay_eqc.data ();
  ++_num_items;
}

Sliders_Pad_Layout::~Sliders_Pad_Layout () {}

// QLayout methods

void
Sliders_Pad_Layout::set_header_item ( QLayoutItem * item_n )
{
  if ( _header_item != 0 ) {
    QScopedPointer< QLayoutItem > item ( _header_item.data () );
    removeItem ( item.data () );
    _header_data = 0;
  }
  if ( item_n != 0 ) {
    _header_item.reset ( item_n );
    _items[ _num_items ] = item_n;
    ++_num_items;
    invalidate ();
  }
}

void
Sliders_Pad_Layout::set_header_widget ( QWidget * wdg_n )
{
  if ( wdg_n != 0 ) {
    ::Wdg::Sliders_Pad_Header * header (
        dynamic_cast<::Wdg::Sliders_Pad_Header * > ( wdg_n ) );
    if ( header != 0 ) {
      set_header_item ( new QWidgetItem ( wdg_n ) );
      _header_data = &header->hd_data ();
    }
  }
}

void
Sliders_Pad_Layout::set_footer_item ( QLayoutItem * item_n )
{
  if ( _footer_item != 0 ) {
    QScopedPointer< QLayoutItem > item ( _footer_item.data () );
    removeItem ( item.data () );
    _footer_data = 0;
  }
  if ( item_n != 0 ) {
    _footer_item.reset ( item_n );
    _items[ _num_items ] = item_n;
    ++_num_items;
    invalidate ();
  }
}

void
Sliders_Pad_Layout::set_footer_widget ( QWidget * wdg_n )
{
  if ( wdg_n != 0 ) {
    ::Wdg::Sliders_Pad_Header * header (
        dynamic_cast<::Wdg::Sliders_Pad_Header * > ( wdg_n ) );
    if ( header != 0 ) {
      set_footer_item ( new QWidgetItem ( wdg_n ) );
      _footer_data = &header->hd_data ();
    }
  }
}

int
Sliders_Pad_Layout::add_group_widget ( QWidget * wdg_n,
                                       unsigned int group_idx,
                                       unsigned int column_idx,
                                       unsigned int row_idx_n )
{
  return _lay_eqc->add_group_widget ( wdg_n, group_idx, column_idx, row_idx_n );
}

void
Sliders_Pad_Layout::addItem ( QLayoutItem * item_n )
{
  _lay_eqc->addItem ( item_n );
}

QLayoutItem *
Sliders_Pad_Layout::itemAt ( int idx_n ) const
{
  QLayoutItem * res ( 0 );
  if ( ( idx_n >= 0 ) && ( idx_n < (int)_num_items ) ) {
    res = _items[ idx_n ];
  }
  return res;
}

QLayoutItem *
Sliders_Pad_Layout::takeAt ( int idx_n )
{
  QLayoutItem * res ( 0 );
  if ( ( idx_n >= 0 ) && ( idx_n < (int)_num_items ) ) {
    res = _items[ idx_n ];
    --_num_items;

    const unsigned int num ( _num_items - idx_n );
    for ( unsigned int ii = 0; ii < num; ++ii ) {
      const unsigned int idx ( idx_n + ii );
      _items[ idx ] = _items[ idx + 1 ];
    }

    if ( res == _header_item.data () ) {
      _header_item.take ();
    } else if ( res == _lay_eqc.data () ) {
      _lay_eqc.take ();
    } else if ( res == _footer_item.data () ) {
      _footer_item.take ();
    }
    invalidate ();
  }
  return res;
}

int
Sliders_Pad_Layout::count () const
{
  return _num_items;
}

QSize
Sliders_Pad_Layout::minimumSize () const
{
  QSize res ( _lay_eqc->minimumSize () );
  if ( _header_item != 0 ) {
    const QSize hms ( _header_item->minimumSize () );
    res.rwidth () = qMax ( hms.width (), res.width () );
    res.rheight () += hms.height ();
  }
  if ( _footer_item != 0 ) {
    const QSize fms ( _footer_item->minimumSize () );
    res.rwidth () = qMax ( fms.width (), res.width () );
    res.rheight () += fms.height ();
  }
  if ( extra_sub_slider_spacing () ) {
    // Sub slider area when no buttons are present
    res.rheight () += _lay_eqc->spacing_vertical ();
  }

  {
    const QMargins & mgs ( contentsMargins () );
    res.rwidth () += mgs.left () + mgs.right ();
    res.rheight () += mgs.top () + mgs.bottom ();
  }

  //::std::cout << "Sliders_Pad_Layout::minimumSize " << res.width() << ":" <<
  //res.height() << "\n";
  return res;
}

QSize
Sliders_Pad_Layout::sizeHint () const
{
  QSize res ( minimumSize () );
  int & ww ( res.rwidth () );
  int & hh ( res.rheight () );

  int mg_hor ( 0 );
  int mg_vert ( 0 );
  {
    const QMargins mgs ( contentsMargins () );
    mg_hor = mgs.left () + mgs.right ();
    mg_vert = mgs.top () + mgs.bottom ();
  }

  ww -= mg_hor;
  hh -= mg_vert;

  ww *= 3;
  hh *= 3;
  ww /= 2;
  hh /= 2;

  ww += mg_hor;
  hh += mg_vert;

  return res;
}

bool
Sliders_Pad_Layout::extra_sub_slider_spacing () const
{
  return ( _lay_eqc->num_rows () == 1 );
}

unsigned int
Sliders_Pad_Layout::header_height_hint (
    const QLayoutItem * item_n, const Sliders_Pad_Header_Data * hdata_n ) const
{
  unsigned int res ( 0 );

  if ( ( item_n == 0 ) || ( hdata_n == 0 ) ) {
    return res;
  }

  if ( !item_n->isEmpty () ) {
    unsigned int fnt_height ( 16 );
    if ( hdata_n->widget != 0 ) {
      fnt_height = hdata_n->widget->fontMetrics ().height ();
    }
    unsigned int max_label_len ( 0 );
    {
      unsigned int num_lbl ( hdata_n->labels.size () );
      if ( num_lbl > 0 ) {
        for ( unsigned int ii = 0; ii < num_lbl; ++ii ) {
          unsigned int len ( hdata_n->labels[ ii ].label_length_max );
          if ( max_label_len < len ) {
            max_label_len = len;
          }
        }
      } else {
        max_label_len = hdata_n->max_str_length_px;
      }
    }

    max_label_len += hdata_n->pad_left;
    max_label_len += hdata_n->pad_right;

    const double angle_sin ( ::std::abs ( hdata_n->angle_sin ) );
    const double angle_cos ( ::std::abs ( hdata_n->angle_cos ) );

    double h_hint ( hdata_n->spacing_vertical );
    h_hint += max_label_len * angle_sin;
    h_hint += fnt_height * angle_cos;
    res = ::std::ceil ( h_hint );
  }

  return res;
}

void
Sliders_Pad_Layout::calc_column_widths_sync ( unsigned int width_n )
{
  _lay_eqc->calc_column_widths ( width_n );

  // Synchronize sliders pad data
  const unsigned int num_grps ( _lay_eqc->num_active_groups () );
  if ( num_grps == _sp_data->groups.size () ) {
    for ( unsigned int gii = 0; gii < num_grps; ++gii ) {
      const Equal_Columns_Layout_Group * eq_grp ( _lay_eqc->group ( gii ) );
      Sliders_Pad_Data_Group * sp_grp ( _sp_data->groups[ gii ] );
      sp_grp->group_pos = eq_grp->group_pos ();
      sp_grp->group_width = eq_grp->group_width ();

      const unsigned int num_cols ( eq_grp->num_columns () );
      if ( num_cols == sp_grp->columns.size () ) {
        for ( unsigned int cii = 0; cii < num_cols; ++cii ) {
          const Equal_Columns_Layout_Column * eq_col ( eq_grp->column ( cii ) );
          Sliders_Pad_Data_Column * sp_col ( sp_grp->columns[ cii ] );
          sp_col->col_pos = eq_col->column_pos ();
          sp_col->col_width = eq_col->column_width ();
        }
      }
    }
  }
}

void
Sliders_Pad_Layout::calc_label_angle ( Sliders_Pad_Header_Data * hdata_n,
                                       unsigned int lbl_hor_dist_n,
                                       bool min_angle_n )
{
  if ( hdata_n == 0 ) {
    return;
  }

  unsigned int fnt_height ( 16 );
  if ( hdata_n->widget != 0 ) {
    fnt_height = hdata_n->widget->fontMetrics ().height ();
  }

  double angle_sin;
  const unsigned int lbl_v_dist ( fnt_height + hdata_n->spacing_inter );
  const unsigned int col_h_dist ( qMax ( (unsigned int)1, lbl_hor_dist_n ) );

  //::std::cout << "lbl_v_dist " << lbl_v_dist << "\n";
  //::std::cout << "col_h_dist " << col_h_dist << "\n";

  if ( col_h_dist >= lbl_v_dist ) {
    angle_sin = double ( lbl_v_dist ) / double ( col_h_dist );
    if ( min_angle_n && ( angle_sin > 0.7 ) ) {
      angle_sin = 1.0;
    }
  } else {
    angle_sin = 1.0;
  }
  if ( hdata_n->upside_down ) {
    angle_sin = -angle_sin;
  }
  angle_sin = ::std::asin ( angle_sin );
  hdata_n->angle = angle_sin;
  hdata_n->angle_sin = ::std::sin ( angle_sin );
  hdata_n->angle_cos = ::std::cos ( angle_sin );

  const double fh2 ( fnt_height / 2.0 );
  hdata_n->center_x = hdata_n->angle_sin * fh2;
  hdata_n->center_y = hdata_n->angle_cos * fh2;

  const double limit_min ( 0.001 );
  if ( ::std::abs ( hdata_n->center_x ) < limit_min ) {
    hdata_n->center_x = 0.0;
  }
  if ( ::std::abs ( hdata_n->center_y ) < limit_min ) {
    hdata_n->center_y = 0.0;
  }

  hdata_n->update_elided_texts = true;
}

double
Sliders_Pad_Layout::calc_label_x_center (
    const Sliders_Pad_Header_Data * hdata_n,
    const Sliders_Pad_Data_Group * sp_grp_n,
    const Sliders_Pad_Data_Column * sp_col_n )
{
  double x_center;
  if ( hdata_n->column_labels ) {
    x_center = sp_col_n->col_pos;
    x_center += sp_col_n->col_width / 2.0;
  } else {
    x_center = sp_grp_n->group_pos;
    x_center += sp_grp_n->group_width / 2.0;
  }
  return x_center;
}

unsigned int
Sliders_Pad_Layout::calc_labels_max_x (
    const Sliders_Pad_Header_Data * hdata_n )
{
  unsigned int res ( 0 );

  if ( hdata_n != 0 ) {
    const double fnt_height ( hdata_n->widget->fontMetrics ().height () );
    const double angle_sin_abs ( ::std::abs ( hdata_n->angle_sin ) );
    const double angle_cos_abs ( ::std::abs ( hdata_n->angle_cos ) );
    const double height_off ( fnt_height * angle_sin_abs / 2.0 );
    const unsigned int x_padding ( hdata_n->pad_left + hdata_n->pad_right );

    unsigned int lbl_idx ( hdata_n->labels.size () );
    unsigned int lim_num ( 4 ); // Check these number of labels
    if ( lbl_idx < lim_num ) {
      lim_num = lbl_idx;
    }
    for ( unsigned int ii = 0; ii < lim_num; ++ii ) {
      --lbl_idx;
      const Sliders_Pad_Header_Label & lbl ( hdata_n->labels[ lbl_idx ] );
      const Sliders_Pad_Data_Group * sp_grp (
          _sp_data->groups[ lbl.group_idx ] );
      const Sliders_Pad_Data_Column * sp_col (
          sp_grp->columns[ lbl.column_idx ] );

      unsigned int x_max;
      {
        double x_center ( calc_label_x_center ( hdata_n, sp_grp, sp_col ) );
        double label_len ( lbl.label_length_max + x_padding );
        double x_max_dbl ( x_center + height_off + label_len * angle_cos_abs );
        x_max = ::std::ceil ( x_max_dbl );
      }
      if ( x_max > res ) {
        res = x_max;
      }
    }
  }

  return res;
}

void
Sliders_Pad_Layout::calc_columns_sizes ( unsigned int area_width_n,
                                         unsigned int area_height_n )
{
  //::std::cout << "Sliders_Pad_Layout::calc_columns_sizes " << area_width_n <<
  //":" << area_height_n << "\n";

  if ( _header_data == 0 ) {
    return;
  }

  // Require synchronized label storage sizes
  if ( _header_data->labels.size () != _lay_eqc->num_groups () ) {
    return;
  }

  // Minimum size of the sliders block
  const QSize inputs_min_size ( _lay_eqc->minimumSize () );

  bool header_vis ( false );
  bool footer_vis ( false );

  unsigned int h_header = 0;
  unsigned int h_inputs = 0;
  unsigned int h_footer = 0;
  unsigned int h_sub_slider = 0;
  if ( extra_sub_slider_spacing () ) {
    h_sub_slider = _lay_eqc->spacing_vertical ();
  }

  if ( _header_item != 0 ) {
    if ( !_header_item->isEmpty () ) {
      header_vis = true;
      h_header = _header_item->minimumSize ().height ();
    }
  }
  if ( _footer_item != 0 ) {
    if ( !_footer_item->isEmpty () ) {
      footer_vis = true;
      h_footer = _footer_item->minimumSize ().height ();
    }
  }
  h_inputs = area_height_n - h_header - h_footer - h_sub_slider;

  // Calculate a first estimate for the row heights
  _lay_eqc->calc_row_heights ( h_inputs );

  // Shrink columns widths so that the last labels fit into the given frame
  // The column widths are fixed after this step
  if ( header_vis || footer_vis ) {
    const unsigned int min_width ( qMax ( 0, inputs_min_size.width () ) );
    const unsigned int max_width ( area_width_n );
    unsigned int w_min ( ( area_width_n * 4 ) / 5 );
    if ( w_min < min_width ) {
      w_min = inputs_min_size.width ();
    }

    unsigned int w_test ( max_width );
    while ( ( w_test >= w_min ) && ( w_test > 0 ) ) {
      calc_column_widths_sync ( w_test );
      const bool min_angle ( w_test <= min_width );
      calc_label_angle (
          _header_data, _lay_eqc->smallest_group_dist (), min_angle );
      calc_label_angle (
          _footer_data, _lay_eqc->smallest_column_dist (), min_angle );
      unsigned int max_x_header ( calc_labels_max_x ( _header_data ) );
      unsigned int max_x_footer ( calc_labels_max_x ( _footer_data ) );
      if ( ( max_x_header < max_width ) && ( max_x_footer < max_width ) ) {
        break;
      }
      --w_test;
    }
    //::std::cout << "Iterations min_width " << min_width  << "\n";
    //::std::cout << "Iterations max_width " << max_width  << "\n";
    //::std::cout << "Iterations w_test " << w_test  << "\n";
    //::std::cout << "Iterations " << max_width - w_test << "\n";
  }

  //
  // Adjust header and sliders block heights
  //

  const unsigned int h_header_hint (
      header_height_hint ( _header_item.data (), _header_data ) );
  const unsigned int h_footer_hint (
      header_height_hint ( _footer_item.data (), _footer_data ) );
  h_header = h_header_hint;
  h_inputs = _lay_eqc->all_rows_height ();
  h_footer = h_footer_hint;

  // Header and footer should always be smaller than the inputs block
  if ( ( h_header * 3 ) > area_height_n ) {
    h_header = area_height_n / 3;
    if ( h_header > h_header_hint ) {
      h_header = h_header_hint;
    }
  }
  if ( ( h_footer * 4 ) > area_height_n ) {
    h_footer = area_height_n / 4;
    if ( h_footer > h_footer_hint ) {
      h_footer = h_footer_hint;
    }
  }

  h_inputs = area_height_n - h_header - h_footer - h_sub_slider;

  // Check inputs area minimum size and shrink the header and footer on demand
  if ( (int)h_inputs < inputs_min_size.height () ) {
    h_inputs = inputs_min_size.height ();
    if ( h_inputs < area_height_n ) {
      unsigned int delta ( area_height_n - h_inputs - h_sub_slider );
      h_footer = delta / 2;
      h_header = delta - h_footer;
    } else {
      h_header = 0;
      h_footer = 0;
    }
  }

  _lay_eqc->calc_row_heights ( h_inputs );
  post_adjust_row_heights ();

  // Store calculated area sizes
  _sp_data->header_area.setHeight ( h_header );
  _sp_data->inputs_area.setHeight ( h_inputs );
  _sp_data->footer_area.setHeight ( h_footer );
  {
    unsigned int aheight ( 0 );
    if ( _lay_eqc->num_rows () > 0 ) {
      aheight = _lay_eqc->row_height ( 0 );
    }
    _sp_data->sliders_area_height = aheight;
  }
  {
    unsigned int aheight ( 0 );
    if ( _lay_eqc->num_rows () > 1 ) {
      aheight = _lay_eqc->row_height ( 1 );
    }
    _sp_data->switches_area_height = aheight;
  }
}

void
Sliders_Pad_Layout::post_adjust_row_heights ()
{
  if ( _lay_eqc->num_rows () < 2 ) {
    return;
  }
  if ( _lay_eqc->row_height ( 0 ) <= _lay_eqc->row_min_height ( 0 ) ) {
    return;
  }

  unsigned int sw_height ( _lay_eqc->row_height ( 0 ) / 16 );
  // Switch row should not be higher than broad
  for ( unsigned int ii = 2; ii <= 3; ++ii ) {
    const unsigned int twidth ( _lay_eqc->col_type_width ( ii ) );
    if ( ( twidth > 0 ) && ( sw_height > twidth ) ) {
      sw_height = twidth;
    }
  }

  if ( sw_height <= _lay_eqc->row_height ( 1 ) ) {
    return;
  }
  unsigned int delta ( sw_height - _lay_eqc->row_height ( 1 ) );
  const unsigned int max_delta ( _lay_eqc->row_height ( 0 ) -
                                 _lay_eqc->row_min_height ( 0 ) );
  if ( delta > max_delta ) {
    delta = max_delta;
  }

  _lay_eqc->set_row_height ( 0, _lay_eqc->row_height ( 0 ) - delta );
  _lay_eqc->set_row_height ( 1, _lay_eqc->row_height ( 1 ) + delta );
}

void
Sliders_Pad_Layout::setGeometry ( const QRect & rect_n )
{
  QRect crect ( rect_n );
  {
    const QMargins & mgs ( contentsMargins () );
    crect.adjust ( mgs.left (), mgs.top (), -mgs.right (), -mgs.bottom () );
  }

  if ( crect.isValid () ) {
    set_geometries ( crect );
  }
}

void
Sliders_Pad_Layout::set_geometries ( const QRect & crect_n )
{
  _lay_eqc->update_cache ();
  calc_columns_sizes ( crect_n.width (), crect_n.height () );

  // Header area
  _sp_data->header_area.moveLeft ( crect_n.left () );
  _sp_data->header_area.moveTop ( crect_n.top () );
  _sp_data->header_area.setWidth ( crect_n.width () );

  // Inputs area
  _sp_data->inputs_area.moveLeft ( crect_n.left () );
  _sp_data->inputs_area.moveTop ( _sp_data->header_area.top () +
                                  _sp_data->header_area.height () );
  _sp_data->inputs_area.setWidth ( crect_n.width () );

  // Sliders area
  _sp_data->sliders_area_y = _sp_data->inputs_area.top ();

  // Sub sliders area
  _sp_data->sub_slider_area_y = _sp_data->sliders_area_y;
  _sp_data->sub_slider_area_y += _sp_data->sliders_area_height;
  _sp_data->sub_slider_area_height = _lay_eqc->spacing_vertical ();

  // Switches area
  _sp_data->switches_area_y = _sp_data->sub_slider_area_y;
  _sp_data->switches_area_y += _sp_data->sub_slider_area_height;

  // Footer area
  _sp_data->footer_area.moveLeft ( crect_n.left () );
  _sp_data->footer_area.moveTop ( _sp_data->switches_area_y +
                                  _sp_data->switches_area_height );
  _sp_data->footer_area.setWidth ( crect_n.width () );

  // Apply geometries to widgets
  if ( _header_item != 0 ) {
    if ( !_header_item->isEmpty () ) {
      _header_item->setGeometry ( _sp_data->header_area );
      update_labels_transforms ( _header_data, _sp_data->header_area );
    }
  }
  _lay_eqc->set_geometries ( _sp_data->inputs_area );
  if ( _footer_item != 0 ) {
    if ( !_footer_item->isEmpty () ) {
      _footer_item->setGeometry ( _sp_data->footer_area );
      update_labels_transforms ( _footer_data, _sp_data->footer_area );
    }
  }
}

void
Sliders_Pad_Layout::update_labels_transforms (
    Sliders_Pad_Header_Data * hdata_n, const QRect & hrect_n )
{
  //::std::cout << "Sliders_Pad_Layout::update_labels_transforms\n";

  if ( !hrect_n.isValid () || ( hdata_n == 0 ) ) {
    return;
  }
  if ( hdata_n->widget == 0 ) {
    return;
  }

  const double fnt_height ( hdata_n->widget->fontMetrics ().height () );
  double lbl_area_height ( hrect_n.height () - hdata_n->spacing_vertical );
  double x_right ( hrect_n.width () );
  double y_base;
  if ( !hdata_n->upside_down ) {
    y_base = lbl_area_height;
  } else {
    y_base = 0.0;
    y_base += hdata_n->spacing_vertical;
    y_base += double ( fnt_height ) * hdata_n->angle_cos;
  }

  const unsigned int num_lbl ( hdata_n->labels.size () );
  for ( unsigned int ii = 0; ii < num_lbl; ++ii ) {
    Sliders_Pad_Header_Label & lbl ( hdata_n->labels[ ii ] );
    if ( lbl.group_idx >= _sp_data->groups.size () ) {
      continue;
    }
    const Sliders_Pad_Data_Group * sp_grp ( _sp_data->groups[ lbl.group_idx ] );
    if ( lbl.column_idx >= sp_grp->columns.size () ) {
      continue;
    }
    const Sliders_Pad_Data_Column * sp_col (
        sp_grp->columns[ lbl.column_idx ] );

    // Find horizontal center
    double x_center ( calc_label_x_center ( hdata_n, sp_grp, sp_col ) );
    double x_base ( x_center + hdata_n->center_x );

    // Label transformation and inverse
    {
      const QTransform trans ( hdata_n->angle_cos,
                               -hdata_n->angle_sin,
                               0.0,
                               hdata_n->angle_sin,
                               hdata_n->angle_cos,
                               0.0,
                               x_base,
                               y_base,
                               1.0 );
      lbl.label_trans = trans;
      lbl.label_trans_inv = trans.inverted ();
    }

    // Label rectangle
    const double limit_min ( 0.002 );
    const double angle_sin_abs ( ::std::abs ( hdata_n->angle_sin ) );
    const double angle_cos_abs ( ::std::abs ( hdata_n->angle_cos ) );
    double lbl_width; // Label width including left and right padding
    if ( angle_sin_abs > limit_min ) {
      double h_avail ( lbl_area_height );
      h_avail -= double ( fnt_height ) * angle_cos_abs;
      lbl_width = h_avail / angle_sin_abs;
    } else {
      lbl_width = hdata_n->max_str_length_px * 2;
    }

    // Shorten the length if the label is too near to the right edge
    if ( angle_cos_abs > limit_min ) {
      double xdist ( x_right - x_center );
      double height_offset ( fnt_height * angle_sin_abs / 2.0 );
      double x_center_dist ( lbl_width * angle_cos_abs + height_offset );
      if ( xdist < x_center_dist ) {
        xdist -= height_offset;
        lbl_width = xdist / angle_cos_abs;
        if ( lbl_width < 0.0 ) {
          lbl_width = 0.0;
        }
      }
    }

    lbl.label_rect = QRectF ( 0, -fnt_height, lbl_width, fnt_height );

    lbl.text_area = lbl.label_rect;
    lbl.text_area.moveLeft ( lbl.text_area.left () + hdata_n->pad_left );
    lbl.text_area.setWidth ( lbl.text_area.width () - hdata_n->pad_left -
                             hdata_n->pad_right );

    {
      QRectF bbox ( lbl.label_rect );
      bbox = lbl.label_trans.mapRect ( bbox );
      lbl.label_txt_bbox.setRect ( ::std::floor ( bbox.left () ),
                                   ::std::floor ( bbox.top () ),
                                   ::std::ceil ( bbox.width () ),
                                   ::std::ceil ( bbox.height () ) );
      // Make a bit wider just to be sure
      lbl.label_txt_bbox.adjust ( -1, -1, 1, 1 );
    }
  }
}

} // namespace Wdg
