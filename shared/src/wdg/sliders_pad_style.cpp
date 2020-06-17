/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "sliders_pad_style.hpp"
#include "sliders_pad_data.hpp"
#include "sliders_pad_header_data.hpp"
#include <QPainterPath>
#include <QPicture>

#define _USE_MATH_DEFINES
#include <climits>
#include <cmath>
#include <iostream>

namespace Wdg
{

Sliders_Pad_Style::Sliders_Pad_Style ( Sliders_Pad_Data * data_n )
: _sp_data ( data_n )
{
  stem_corner_indent = 1.0;
  stem_pen.setWidthF ( 1 );
  stem_pen.setCapStyle ( Qt::FlatCap );
  stem_pen.setJoinStyle ( Qt::BevelJoin );
}

Sliders_Pad_Style::~Sliders_Pad_Style () {}

double
Sliders_Pad_Style::calc_col_center ( unsigned int col_width_n,
                                     unsigned int col_idx_n,
                                     unsigned int num_cols_n ) const
{
  unsigned int div_val ( col_width_n );

  if ( col_width_n > 0 ) {
    if ( ( col_width_n % 2 ) == 0 ) {
      if ( col_idx_n >= ( num_cols_n / 2 ) ) {
        ++div_val;
      } else {
        --div_val;
      }
    }
  }

  return ( double ( div_val ) / 2.0 );
}

QPainterPath
Sliders_Pad_Style::bridge_path ( Sliders_Pad_Data_Group * sp_grp_n,
                                 double y_top_n ) const
{
  QPainterPath ppath;
  if ( sp_grp_n->columns.size () < 2 ) {
    return ppath;
  }

  const unsigned int num_cols ( sp_grp_n->columns.size () );
  const unsigned int col_idx_max ( num_cols - 1 );
  const double y_bottom ( 0.0 );

  // Draw bridge line from the leftmost column rightmost
  {
    const Sliders_Pad_Data_Column * sp_col_b ( sp_grp_n->columns[ 0 ] );
    const Sliders_Pad_Data_Column * sp_col_e (
        sp_grp_n->columns[ col_idx_max ] );

    double cx_mid_min ( sp_col_b->col_pos );
    double cx_mid_max ( sp_col_e->col_pos );

    cx_mid_min += calc_col_center ( sp_col_b->col_width, 0, num_cols );

    cx_mid_max +=
        calc_col_center ( sp_col_e->col_width, col_idx_max, num_cols );

    const double c_off ( stem_corner_indent ); // Corner offset
    ppath.moveTo ( cx_mid_min, y_bottom );
    ppath.lineTo ( cx_mid_min, y_top_n - c_off );
    ppath.lineTo ( cx_mid_min + c_off, y_top_n );
    ppath.lineTo ( cx_mid_max - c_off, y_top_n );
    ppath.lineTo ( cx_mid_max, y_top_n - c_off );
    ppath.lineTo ( cx_mid_max, y_bottom );
  }

  // Draw vertical lines from the bridge line down to the bottom
  for ( unsigned int cii = 1; cii < col_idx_max; ++cii ) {
    const Sliders_Pad_Data_Column * sp_col ( sp_grp_n->columns[ cii ] );
    double cx_mid ( sp_col->col_pos );
    cx_mid += calc_col_center ( sp_col->col_width, cii, num_cols );

    ppath.moveTo ( cx_mid, y_top_n );
    ppath.lineTo ( cx_mid, y_bottom );
  }

  return ppath;
}

QPainterPath
Sliders_Pad_Style::inclined_bridge_path ( Sliders_Pad_Data_Group * sp_grp_n,
                                          double y_top_n ) const
{
  QPainterPath ppath;
  if ( sp_grp_n->columns.size () < 2 ) {
    return ppath;
  }

  const unsigned int num_cols ( sp_grp_n->columns.size () );
  const unsigned int col_idx_max ( num_cols - 1 );
  const double y_bottom ( 0.0 );

  // Draw bridge line from the leftmost column rightmost
  {
    const Sliders_Pad_Data_Column * sp_col_b ( sp_grp_n->columns[ 0 ] );
    const Sliders_Pad_Data_Column * sp_col_e (
        sp_grp_n->columns[ col_idx_max ] );

    double cx_mid_min ( sp_col_b->col_pos );
    double cx_mid_max ( sp_col_e->col_pos );

    cx_mid_min += calc_col_center ( sp_col_b->col_width, 0, num_cols );

    cx_mid_max +=
        calc_col_center ( sp_col_e->col_width, col_idx_max, num_cols );

    const double x_off ( y_top_n - 0.5 ); // Corner offset
    ppath.moveTo ( cx_mid_min, y_bottom );
    ppath.lineTo ( cx_mid_min, y_bottom + 0.5 );
    ppath.lineTo ( cx_mid_min + x_off, y_top_n );

    ppath.lineTo ( cx_mid_max - x_off, y_top_n );
    ppath.lineTo ( cx_mid_max, y_bottom + 0.5 );
    ppath.lineTo ( cx_mid_max, y_bottom );
  }

  // Draw vertical lines from the bridge line down to the bottom
  for ( unsigned int cii = 1; cii < col_idx_max; ++cii ) {
    const Sliders_Pad_Data_Column * sp_col ( sp_grp_n->columns[ cii ] );
    double cx_mid ( sp_col->col_pos );
    cx_mid += calc_col_center ( sp_col->col_width, cii, num_cols );

    ppath.moveTo ( cx_mid, y_top_n );
    ppath.lineTo ( cx_mid, y_bottom );
  }

  return ppath;
}

QPainterPath
Sliders_Pad_Style::inclined_multi_bridge_path (
    Sliders_Pad_Data_Group * sp_grp_n, double y_top_n ) const
{
  QPainterPath ppath;
  if ( sp_grp_n->columns.size () < 2 ) {
    return ppath;
  }

  const unsigned int num_cols ( sp_grp_n->columns.size () );
  const unsigned int col_idx_max ( num_cols - 1 );
  const double y_bottom ( 0.0 );

  // Draw vertical lines from the bridge line down to the bottom
  for ( unsigned int cii = 0; cii < col_idx_max; ++cii ) {
    const Sliders_Pad_Data_Column * sp_col_b ( sp_grp_n->columns[ cii ] );
    const Sliders_Pad_Data_Column * sp_col_e ( sp_grp_n->columns[ cii + 1 ] );

    double cx_mid_1 ( sp_col_b->col_pos );
    double cx_mid_2 ( sp_col_e->col_pos );

    cx_mid_1 += calc_col_center ( sp_col_b->col_width, 0, num_cols );

    cx_mid_2 += calc_col_center ( sp_col_e->col_width, col_idx_max, num_cols );

    const double x_off ( y_top_n - 0.5 ); // Corner offset
    ppath.moveTo ( cx_mid_1, y_bottom );
    ppath.lineTo ( cx_mid_1, y_bottom + 0.5 );
    ppath.lineTo ( cx_mid_1 + x_off, y_top_n );

    ppath.lineTo ( cx_mid_2 - x_off, y_top_n );
    ppath.lineTo ( cx_mid_2, y_bottom + 0.5 );
    ppath.lineTo ( cx_mid_2, y_bottom );
  }

  return ppath;
}

void
Sliders_Pad_Style::paint_base_decoration ()
{
  if ( sp_data () != 0 ) {
    this->draw_base ();
  }
}

void
Sliders_Pad_Style::paint_header_decoration ()
{
  if ( sp_data () != 0 ) {
    if ( ( sp_data ()->header != 0 ) && ( sp_data ()->header_data != 0 ) ) {
      this->draw_header ();
    }
  }
}

void
Sliders_Pad_Style::paint_footer_decoration ()
{
  if ( sp_data () != 0 ) {
    if ( ( sp_data ()->footer != 0 ) && ( sp_data ()->footer_data != 0 ) ) {
      this->draw_footer ();
    }
  }
}

// Base painting

void
Sliders_Pad_Style::draw_base ()
{
  _area_left = qMax ( 0, sp_data ()->inputs_area.left () );
  _area_height = sp_data ()->sub_slider_area_height;
  _y_top = sp_data ()->sub_slider_area_y;
  _y_bottom = sp_data ()->switches_area_y;

  _y_mid = _area_height;
  if ( ( _area_height % 2 ) == 0 ) {
    _y_mid -= 1;
  }
  _y_mid = _y_mid / 2.0;

  const unsigned int num_grps ( sp_data ()->groups.size () );
  for ( unsigned int ii = 0; ii < num_grps; ++ii ) {
    Sliders_Pad_Data_Group * sp_grp ( sp_data ()->groups[ ii ] );
    sp_grp->center_pic = QPicture ();

    const unsigned int num_sliders ( sp_grp->num_sliders );
    const unsigned int num_switches ( sp_grp->num_switches );

    _x_mid = sp_grp->group_width;
    if ( ( sp_grp->group_width % 2 ) == 0 ) {
      _x_mid -= 1.0;
    }
    _x_mid = _x_mid / 2.0;
    _x_mid += double ( _area_left ) + sp_grp->group_pos;

    QPainter pnt ( &sp_grp->center_pic );

    if ( num_sliders > 1 ) {
      dbase_sliders_bridge ( pnt, sp_grp );
    }

    if ( num_switches > 0 ) {
      if ( num_switches == 1 ) {
        dbase_single_switch_stem ( pnt, sp_grp );
      } else {
        if ( num_sliders == num_switches ) {
          dbase_multi_switch_stems ( pnt, sp_grp );
        } else {
          dbase_switches_bridge ( pnt, sp_grp );
          if ( num_sliders == 1 ) {
            dbase_single_slider_stem ( pnt, sp_grp );
          }
        }
      }
    }

    if ( ( num_sliders > 0 ) && ( num_switches < num_sliders ) ) {
      dbase_labels_connectors ( pnt, sp_grp );
    }
  }
}

void
Sliders_Pad_Style::dbase_sliders_bridge ( QPainter & pnt_n,
                                          Sliders_Pad_Data_Group * sp_grp_n )
{
  QPainterPath ppath;
  ppath = inclined_multi_bridge_path ( sp_grp_n, _y_mid );
  ppath.translate ( _area_left, _y_top );

  pnt_n.setBrush ( Qt::NoBrush );
  pnt_n.setPen ( stem_pen );
  pnt_n.drawPath ( ppath );
}

void
Sliders_Pad_Style::dbase_switches_bridge ( QPainter & pnt_n,
                                           Sliders_Pad_Data_Group * sp_grp_n )
{
  QPainterPath ppath;
  ppath = bridge_path ( sp_grp_n, double ( _area_height ) - _y_mid );
  { // Invert and translate
    const QTransform trans (
        1.0, 0.0, 0.0, 0.0, -1.0, 0.0, _area_left, _y_bottom, 1.0 );
    ppath = trans.map ( ppath );
  }

  pnt_n.setBrush ( Qt::NoBrush );
  pnt_n.setPen ( stem_pen );
  pnt_n.drawPath ( ppath );
}

void
Sliders_Pad_Style::dbase_single_switch_stem (
    QPainter & pnt_n, Sliders_Pad_Data_Group * sp_grp_n )
{
  double y_top ( _y_top );
  if ( sp_grp_n->num_sliders > 1 ) {
    y_top += _y_mid;
  }

  QPainterPath ppath;
  ppath.moveTo ( _x_mid, y_top );
  ppath.lineTo ( _x_mid, _y_bottom );

  pnt_n.setBrush ( Qt::NoBrush );
  pnt_n.setPen ( stem_pen );
  pnt_n.drawPath ( ppath );
}

void
Sliders_Pad_Style::dbase_single_slider_stem ( QPainter & pnt_n,
                                              Sliders_Pad_Data_Group * )
{
  QPainterPath ppath;
  ppath.moveTo ( _x_mid, _y_top );
  ppath.lineTo ( _x_mid, _y_mid );

  pnt_n.setBrush ( Qt::NoBrush );
  pnt_n.setPen ( stem_pen );
  pnt_n.drawPath ( ppath );
}

void
Sliders_Pad_Style::dbase_multi_switch_stems (
    QPainter & pnt_n, Sliders_Pad_Data_Group * sp_grp_n )
{
  QPainterPath ppath;

  const unsigned int num_cols ( sp_grp_n->num_switches );
  for ( unsigned int cii = 0; cii < num_cols; ++cii ) {
    const Sliders_Pad_Data_Column * sp_col ( sp_grp_n->columns[ cii ] );
    double cx_mid ( _area_left );
    cx_mid += sp_col->col_pos;
    cx_mid += calc_col_center ( sp_col->col_width, cii, num_cols );

    ppath.moveTo ( cx_mid, _y_top );
    ppath.lineTo ( cx_mid, _y_bottom );
  }

  pnt_n.setBrush ( Qt::NoBrush );
  pnt_n.setPen ( stem_pen );
  pnt_n.drawPath ( ppath );
}

void
Sliders_Pad_Style::dbase_labels_connectors ( QPainter & pnt_n,
                                             Sliders_Pad_Data_Group * sp_grp_n )
{
  if ( sp_data ()->show_value_labels ) {
    QPainterPath ppath;

    const unsigned int num_cols ( sp_grp_n->num_sliders );
    for ( unsigned int cii = 0; cii < num_cols; ++cii ) {
      const Sliders_Pad_Data_Column * sp_col ( sp_grp_n->columns[ cii ] );
      if ( sp_col->show_value_label ) {
        double cx_mid ( _area_left );
        cx_mid += sp_col->col_pos;
        cx_mid += calc_col_center ( sp_col->col_width, cii, num_cols );

        double y_bottom ( _y_bottom );
        y_bottom += sp_data ()->switches_area_height;
        ppath.moveTo ( cx_mid, _y_top );
        ppath.lineTo ( cx_mid, y_bottom );
      }
    }

    pnt_n.setBrush ( Qt::NoBrush );
    pnt_n.setPen ( stem_pen );
    pnt_n.drawPath ( ppath );
  }
}

// Header painting

void
Sliders_Pad_Style::draw_header ()
{
  const unsigned int pic_idx ( 0 );
  for ( unsigned int ii = 0; ii < sp_data ()->groups.size (); ++ii ) {
    Sliders_Pad_Data_Group * sp_grp ( sp_data ()->groups[ ii ] );
    if ( sp_grp->num_sliders > 0 ) {
      sp_grp->hd_pics[ pic_idx ] = QPicture ();

      QPainter pnt ( &sp_grp->hd_pics[ pic_idx ] );
      dheader_stem ( pnt, sp_grp );
      if ( sp_grp->num_sliders > 1 ) {
        dheader_bridge ( pnt, sp_grp );
      }
    }
  }
}

void
Sliders_Pad_Style::dheader_stem ( QPainter & pnt_n,
                                  Sliders_Pad_Data_Group * sp_grp_n )
{
  QPainterPath ppath;
  const Sliders_Pad_Header_Data & hdata ( *sp_data ()->header_data );
  const QFontMetrics & fmet ( sp_data ()->header->fontMetrics () );
  {
    const unsigned int hheight ( sp_data ()->header_area.height () );
    const double fnt_hhalf ( fmet.height () / 2.0 );
    const double x_left ( 0.0 );
    const double y_bottom ( hheight );
    const double y_mid ( ( hheight - hdata.spacing_inter ) + 0.5 );

    double stem_height ( hdata.angle_cos * fnt_hhalf );
    double stem_bottom;
    if ( sp_grp_n->num_sliders == 1 ) {
      stem_height += hdata.spacing_vertical;
      stem_bottom = y_bottom;
    } else {
      stem_height += 0.5;
      stem_bottom = y_mid;
    }

    if ( stem_height > 0.5 ) {

      double stem_l1 ( stem_height / ( 1.0 + hdata.angle_sin ) );
      double stem_dx ( stem_l1 * hdata.angle_cos );
      stem_dx = std::floor ( stem_dx );
      if ( ( sp_grp_n->group_width % 2 ) == 0 ) {
        stem_dx += 0.5;
      }

      double stem_h2 ( 0.0 );
      if ( hdata.angle_cos > 0.002 ) {
        stem_h2 = stem_dx * hdata.angle_sin / hdata.angle_cos;
      }

      double grp_x_mid ( x_left + sp_grp_n->group_pos );
      grp_x_mid += double ( sp_grp_n->group_width ) / double ( 2.0 );
      double stem_x1 = grp_x_mid - stem_dx;
      double stem_h1 ( stem_height - stem_h2 );

      QPointF pts[ 3 ] = {QPointF ( grp_x_mid, stem_bottom - stem_height ),
                          QPointF ( stem_x1, stem_bottom - stem_h1 ),
                          QPointF ( stem_x1, stem_bottom )};

      QPointF pmid[ 2 ];
      pmid[ 0 ] = ( pts[ 0 ] * 1.0 + pts[ 1 ] * 2.0 ) / 3.0;
      pmid[ 1 ] = ( pts[ 1 ] * 2.0 + pts[ 2 ] * 1.0 ) / 3.0;

      ppath.moveTo ( pts[ 0 ] );
      ppath.lineTo ( pmid[ 0 ] );
      // ppath.lineTo ( pts[1] );
      ppath.lineTo ( pmid[ 1 ] );
      ppath.lineTo ( pts[ 2 ] );
    }
  }

  pnt_n.setBrush ( Qt::NoBrush );
  pnt_n.setPen ( stem_pen );
  pnt_n.drawPath ( ppath );
}

void
Sliders_Pad_Style::dheader_bridge ( QPainter & pnt_n,
                                    Sliders_Pad_Data_Group * sp_grp_n )
{
  if ( sp_grp_n->columns.size () < 2 ) {
    return;
  }

  QPainterPath ppath;
  { // Acquire bridge path
    double bheight ( sp_data ()->header_data->spacing_inter );
    bheight -= 0.5;
    ppath = inclined_bridge_path ( sp_grp_n, bheight );
  }
  { // Invert and translate
    const double yoff ( sp_data ()->header_area.height () );
    const QTransform trans ( 1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, yoff, 1.0 );
    ppath = trans.map ( ppath );
  }

  pnt_n.setBrush ( Qt::NoBrush );
  pnt_n.setPen ( stem_pen );
  pnt_n.drawPath ( ppath );
}

// Footer painting

void
Sliders_Pad_Style::draw_footer ()
{
  if ( sp_data ()->show_value_labels ) {
    const unsigned int pic_idx ( 1 );
    Sliders_Pad_Header_Data & hdata ( *sp_data ()->footer_data );
    for ( unsigned int ii = 0; ii < hdata.labels.size (); ++ii ) {
      const Sliders_Pad_Header_Label & lbl ( hdata.labels[ ii ] );
      Sliders_Pad_Data_Group * sp_grp ( sp_data ()->groups[ lbl.group_idx ] );
      Sliders_Pad_Data_Column * sp_col ( sp_grp->columns[ lbl.column_idx ] );

      sp_col->hd_pics[ pic_idx ] = QPicture ();

      if ( sp_col->show_value_label ) {
        QPainter pnt ( &sp_col->hd_pics[ pic_idx ] );
        dfooter_stem ( pnt, sp_grp, sp_col );
      }
    }
  }
}

void
Sliders_Pad_Style::dfooter_stem ( QPainter & pnt_n,
                                  Sliders_Pad_Data_Group * sp_grp_n,
                                  Sliders_Pad_Data_Column * sp_col_n )
{
  const Sliders_Pad_Header_Data & hdata ( *sp_data ()->footer_data );

  QPainterPath ppath_line;
  QPainterPath ppath_solid;

  const double fnt_height ( sp_data ()->footer->fontMetrics ().height () );
  const double fheight_yoff ( ::std::abs ( hdata.angle_cos ) * fnt_height /
                              2.0 );
  const double lbl_y_center ( fheight_yoff + hdata.spacing_vertical );

  double x_center;
  if ( hdata.column_labels ) {
    x_center = sp_col_n->col_pos;
    x_center += calc_col_center (
        sp_col_n->col_width, sp_col_n->col_idx, sp_grp_n->columns.size () );
  } else {
    x_center = sp_grp_n->group_pos;
    x_center += calc_col_center ( sp_grp_n->group_width, 0, 0 );
  }

  const double rad ( qMin ( fnt_height / 6.0, lbl_y_center ) );
  const QTransform rot_trans ( hdata.angle_cos,
                               -hdata.angle_sin,
                               0.0,
                               hdata.angle_sin,
                               hdata.angle_cos,
                               0.0,
                               0.0,
                               0.0,
                               1.0 );

  {
    ppath_line.moveTo ( x_center, 0.0 );
    ppath_line.lineTo ( x_center, 0.5 );
    QPointF pnt ( rot_trans.map ( QPointF ( -rad / 2.0, 0.0 ) ) );
    pnt += QPointF ( x_center, lbl_y_center );
    ppath_line.lineTo ( pnt );
  }

  ppath_solid.addEllipse ( QPointF ( 0, 0 ), rad, rad );
  {
    QPainterPath ppd;
    ppd.addRect ( -fnt_height, -fnt_height, fnt_height, 2 * fnt_height );
    ppath_solid = ppath_solid.intersected ( ppd );
  }
  ppath_solid = rot_trans.map ( ppath_solid );
  ppath_solid.translate ( x_center, lbl_y_center );

  // Paint
  pnt_n.setBrush ( Qt::NoBrush );
  pnt_n.setPen ( stem_pen );
  pnt_n.drawPath ( ppath_line );

  pnt_n.setBrush ( stem_pen.color () );
  pnt_n.setPen ( Qt::NoPen );
  pnt_n.drawPath ( ppath_solid );
}

} // namespace Wdg
