/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "ds_slider_painter_bevelled.hpp"
#include "dpe/image_set.hpp"
#include "dpe/image_set_meta.hpp"
#include "dpe/paint_job.hpp"
#include "wdg/color_methods.hpp"
#include "wdg/ds_slider_meta_bg.hpp"
#include "wdg/ds_widget_style_db.hpp"
#include "wdg/ds_widget_types.hpp"
#include "wdg/uint_mapper.hpp"
#include <QImage>
#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>
#include <QRadialGradient>
#include <QScopedPointer>
#include <cmath>
#include <iostream>

namespace Wdg
{
namespace Painter
{

struct DS_Slider_Painter_Bevelled::PData
{

  inline QSize &
  size ()
  {
    return meta->size;
  }

  inline int
  width ()
  {
    return meta->size.width ();
  }

  inline int
  height ()
  {
    return meta->size.height ();
  }

  ::dpe::Image_Set_Meta * meta;

  QPalette pal;
  QPainter qpnt;

  int ew; // Edge width
  int bw; // Border width x,y
  int bevel;
  int tick_width[ 3 ];
  int tick_min_dist;
  QRectF rectf;

  bool has_focus;
  bool has_weak_focus;
  bool mouse_over;
  bool is_down;

  ::dpe::Image * img;
  ::Wdg::DS_Slider_Meta_Bg * meta_bg;
};

DS_Slider_Painter_Bevelled::DS_Slider_Painter_Bevelled ()
: ::Wdg::Painter::DS_Widget_Painter ( ::Wdg::DS_SLIDER )
{
}

int
DS_Slider_Painter_Bevelled::paint_image ( ::dpe::Paint_Job * pjob_n )
{
  int res ( 0 );

  // Init paint data
  PData pd;

  pd.meta = pjob_n->meta;
  pd.img = &pjob_n->img_set->image ( pjob_n->img_idx );

  res = create_image_data ( pd.img, pd.meta );

  if ( res == 0 ) {

    // Init general painting setup
    if ( wdg_style_db () != 0 ) {
      pd.pal = wdg_style_db ()->palettes[ pd.meta->style_id ];
      pd.pal.setCurrentColorGroup (
          wdg_style_db ()->color_group ( pd.meta->style_sub_id ) );
    }
    pd.bw = pd.meta->size.width () / 24;
    if ( pd.bw < 2 ) {
      pd.bw = 2;
    }
    pd.ew = ( pd.bw / 4 );
    if ( pd.ew < 1 ) {
      pd.ew = 1;
    }
    pd.bevel = pd.bw;
    pd.rectf = QRectF ( 0.0, 0.0, pd.width (), pd.height () );

    // Init painter
    pd.qpnt.begin ( &pd.img->qimage () );
    pd.qpnt.setRenderHints ( QPainter::Antialiasing |
                             QPainter::SmoothPixmapTransform );

    // Paint type
    switch ( pd.meta->type_id ) {
    case 0:
      res = paint_bg ( pjob_n, pd );
      break;
    case 1:
      res = paint_marker ( pjob_n, pd );
      break;
    case 2:
      res = paint_frame ( pjob_n, pd );
      break;
    case 3:
      res = paint_handle ( pjob_n, pd );
      break;
    }
  }

  return res;
}

// Background painting

int
DS_Slider_Painter_Bevelled::paint_bg ( ::dpe::Paint_Job * pjob_n, PData & pd )
{
  //::std::cout << "DS_Slider_Painter_Bevelled::paint_bg " << "\n";

  pd.meta_bg = dynamic_cast<::Wdg::DS_Slider_Meta_Bg * > ( pjob_n->meta );
  if ( pd.meta_bg == 0 ) {
    return -1;
  }

  // Calculate tick sizes
  {
    int in_width ( pd.meta->size.width () - 2 * pd.bw );
    pd.tick_width[ 0 ] = in_width * 1 / 2;
    pd.tick_width[ 1 ] = in_width * 1 / 4;
    pd.tick_width[ 2 ] = in_width * 1 / 16;

    pd.tick_width[ 0 ] = ::std::max ( pd.tick_width[ 0 ], 6 );
    pd.tick_width[ 1 ] = ::std::max ( pd.tick_width[ 1 ], 4 );
    pd.tick_width[ 2 ] = ::std::max ( pd.tick_width[ 2 ], 2 );

    if ( ( pd.meta->size.width () % 2 ) == 0 ) {
      for ( int ii = 0; ii < 3; ++ii ) {
        if ( ( pd.tick_width[ ii ] % 2 ) != 0 ) {
          ++( pd.tick_width[ ii ] );
        }
      }
    } else {
      for ( int ii = 0; ii < 3; ++ii ) {
        if ( ( pd.tick_width[ ii ] % 2 ) == 0 ) {
          ++( pd.tick_width[ ii ] );
        }
      }
    }

    pd.tick_min_dist = 8;
  }

  // State flags
  {
    pd.is_down = ( pjob_n->img_idx == 1 );
  }

  // Painting
  {
    paint_bg_area ( pd );
    paint_bg_frame ( pd );
    paint_bg_area_deco ( pd );
    paint_bg_ticks ( pd );
  }

  return 0;
}

void
DS_Slider_Painter_Bevelled::paint_bg_area ( PData & pd )
{
  QColor col_li ( pd.pal.color ( QPalette::Button ) );
  QColor col_mid ( pd.pal.color ( QPalette::Button ) );
  QColor col_dk ( pd.pal.color ( QPalette::Mid ) );

  col_li.setAlpha ( 55 );
  col_dk.setAlpha ( 190 );

  const double inner_width ( pd.width () - 2 * pd.bw );
  const double x_start = pd.bw;
  const double x_end = pd.width () - pd.bw;
  const double x_mid = x_start + inner_width / 3.0;

  QPainterPath area_path;
  papp_bevel_area ( area_path, pd.rectf, pd.bevel, pd.bw / 2.0 );

  // Base color
  {
    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( col_mid );
  }
  pd.qpnt.drawPath ( area_path );

  // Fake 3D gradient
  {
    QLinearGradient lgrad ( QPointF ( x_start, 0.0 ), QPointF ( x_end, 0.0 ) );

    lgrad.setColorAt ( 0.0, col_li );
    lgrad.setColorAt ( x_mid / ( x_end - x_start ), col_mid );
    lgrad.setColorAt ( 1.0, col_dk );

    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( lgrad );
  }
  pd.qpnt.drawPath ( area_path );

  // Sound color overlay
  {
    QColor c0 ( pd.pal.color ( QPalette::Window ) );
    QColor c1 ( c0 );
    c0.setAlpha ( 30 );
    c1.setAlpha ( 0 );

    QLinearGradient lgrad ( QPointF ( x_start, 0.0 ),
                            QPointF ( x_start + inner_width / 7.0, 0.0 ) );

    lgrad.setColorAt ( 0.0, c0 );
    lgrad.setColorAt ( 1.0, c1 );

    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( lgrad );
  }
  pd.qpnt.drawPath ( area_path );
}

void
DS_Slider_Painter_Bevelled::paint_bg_frame ( PData & pd )
{
  QColor fcol;
  {
    QColor col_btn ( pd.pal.color ( QPalette::Button ) );
    QColor col_mid ( pd.pal.color ( QPalette::Mid ) );
    if ( col_mid == col_btn ) {
      col_mid = pd.pal.color ( QPalette::WindowText );
      fcol = col_mix ( col_btn, col_mid, 1, 1 );
    } else {
      fcol = col_mix ( col_btn, col_mid, 1, 2 );
    }
    // fcol = col_mid;
  }

  paint_bevel_raised_frame ( pd, pd.rectf, pd.bevel, pd.bw, pd.ew, fcol );
}

void
DS_Slider_Painter_Bevelled::paint_bg_area_deco ( PData & pd )
{
  QPainterPath ppath;

  const int range_max_idx ( pd.meta_bg->ticks_range_max_idx );
  int y_min;
  int y_bottom;
  int y_top;

  bool has_minimum ( pd.meta_bg->bg_show_image );
  bool has_zero_split ( false );

  y_bottom = pd.height () - 1;
  y_bottom -= pd.meta_bg->ticks_range_start;
  y_top = y_bottom - range_max_idx;
  y_min = y_bottom;

  if ( has_minimum ) {
    Wdg::UInt_Mapper_Auto mapper ( range_max_idx, pd.meta_bg->ticks_max_idx );
    y_min -= mapper.v2_to_v1 ( pd.meta_bg->bg_tick_min_idx );
  }
  has_zero_split = ( ( y_min > y_top ) && ( y_min < y_bottom ) );

  const double w_tick ( ( pd.tick_width[ 0 ] + pd.tick_width[ 1 ] ) / 2.0 );
  const double w_out ( w_tick * 2.0 / 3.0 );
  const double w_neck ( 1 );

  double x_out_l ( ( pd.width () - w_tick ) / 2.0 );
  double x_neck_l ( x_out_l );
  double x_out_r;
  double x_neck_r;

  x_neck_l = std::floor ( x_neck_l );
  x_neck_r = x_neck_l + w_neck;

  x_out_l = std::floor ( x_out_l );
  x_out_r = x_out_l + w_out;

  if ( has_minimum ) {
    if ( has_zero_split ) {
      double x_out_rt ( x_out_r );
      double x_out_rb ( x_out_r );

      const int dt ( y_min - y_top );
      const int db ( y_bottom - y_min );
      if ( dt > db ) {
        int ww = ( db * w_out ) / dt;
        x_out_rb = x_out_l + ww;
      } else {
        int ww = ( dt * w_out ) / db;
        x_out_rt = x_out_l + ww;
      }

      ppath.moveTo ( x_out_l, y_top + 0.5 );
      ppath.lineTo ( x_out_rt, y_top + 0.5 );
      ppath.lineTo ( x_neck_r, y_min + 0.5 );
      ppath.lineTo ( x_out_rb, y_bottom + 0.5 );
      ppath.lineTo ( x_out_l, y_bottom + 0.5 );
      ppath.lineTo ( x_neck_l, y_min + 0.5 );
      ppath.closeSubpath ();

    } else {
      double x_rt;
      double x_rb;
      if ( y_min < y_bottom ) {
        x_rt = x_neck_r;
        x_rb = x_out_r;
      } else {
        x_rt = x_out_r;
        x_rb = x_neck_r;
      }

      ppath.moveTo ( x_neck_l, y_top + 0.5 );
      ppath.lineTo ( x_rt, y_top + 0.5 );
      ppath.lineTo ( x_rb, y_bottom + 0.5 );
      ppath.lineTo ( x_out_l, y_bottom + 0.5 );
      ppath.closeSubpath ();
    }
  } else {
    x_out_r = x_out_l + w_out / 2.0;
    ppath.moveTo ( x_out_l, y_top + 0.5 );
    ppath.lineTo ( x_out_r, y_top + 0.5 );
    ppath.lineTo ( x_out_r, y_bottom + 0.5 );
    ppath.lineTo ( x_out_l, y_bottom + 0.5 );
    ppath.closeSubpath ();
  }

  {
    QColor col_fill ( pd.pal.color ( QPalette::Window ) );
    QColor col_pen ( pd.pal.color ( QPalette::WindowText ) );
    if ( pd.is_down ) {
      col_fill.setAlpha ( 40 );
      col_pen.setAlpha ( 40 );
    } else {
      col_fill.setAlpha ( 16 );
      col_pen.setAlpha ( 18 );
    }

    {
      QPen pen;
      pen.setColor ( col_pen );
      pen.setWidth ( 1.0 );
      pd.qpnt.setPen ( pen );
    }
    pd.qpnt.setBrush ( col_fill );
  }

  pd.qpnt.drawPath ( ppath );
}

void
DS_Slider_Painter_Bevelled::paint_bg_ticks ( PData & pd )
{
  const int range_max_idx ( pd.meta_bg->ticks_range_max_idx );
  unsigned int ticks_max_idx;
  int idx_delta ( 1 );
  int y_off;

  y_off = pd.height () - 1;
  y_off -= pd.meta_bg->ticks_range_start;

  const int y_bottom = y_off;
  const int y_top = y_off - range_max_idx;

  // Setup main tick position mapper
  ticks_max_idx = pd.meta_bg->ticks_max_idx;
  if ( ticks_max_idx > 0 ) {
    if ( ( range_max_idx / ticks_max_idx ) < 2 ) {
      ticks_max_idx = range_max_idx / pd.tick_min_dist;
    }
  }

  ::Wdg::UInt_Mapper_Up mapper ( ticks_max_idx, range_max_idx );

  if ( ticks_max_idx > 0 ) {
    const int tmd ( mapper.min_dist () );
    while ( ( idx_delta * tmd ) < pd.tick_min_dist ) {
      ++idx_delta;
    }
  }

  // Paint
  {
    QColor tick_col ( pd.pal.color ( QPalette::WindowText ) );
    tick_col = col_mix ( tick_col, pd.pal.color ( QPalette::Button ), 3, 1 );

    const int idx_start = idx_delta;
    for ( unsigned int ii = idx_start; ii < ticks_max_idx; ii += idx_delta ) {
      int yy = y_bottom - mapper.map ( ii );
      if ( ( yy - y_top ) < pd.tick_min_dist ) {
        break;
      }
      paint_bg_tick ( pd, yy, pd.tick_width[ 1 ], tick_col );
    }
  }

  {
    QColor tick_col ( pd.pal.color ( QPalette::WindowText ) );
    tick_col = col_mix ( tick_col, pd.pal.color ( QPalette::Button ), 4, 1 );

    // Bottom and top ticks
    if ( ticks_max_idx > 0 ) {
      paint_bg_tick ( pd, y_top, pd.tick_width[ 0 ], tick_col );
    }
    paint_bg_tick ( pd, y_bottom, pd.tick_width[ 0 ], tick_col );
  }
}

void
DS_Slider_Painter_Bevelled::paint_bg_tick ( PData & pd,
                                            double tick_pos_n,
                                            double tick_width_n,
                                            const QColor & col_n )
{
  const double tick_start ( ( pd.width () - tick_width_n ) / 2.0 );

  QColor col ( col_n );

  // Glow below solid line
  {
    col.setAlpha ( 32 );
    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( col );

    const unsigned int num_pts ( 9 );
    double xl ( tick_start - 0.5 );
    double xr ( tick_start + tick_width_n + 0.5 );
    const QPointF points[ num_pts ] = {QPointF ( xl, tick_pos_n - 1 ),
                                       QPointF ( xr, tick_pos_n - 1 ),
                                       QPointF ( xr + 1, tick_pos_n ),
                                       QPointF ( xr + 1, tick_pos_n + 1 ),
                                       QPointF ( xr, tick_pos_n + 2 ),
                                       QPointF ( xl, tick_pos_n + 2 ),
                                       QPointF ( xl - 1, tick_pos_n + 1 ),
                                       QPointF ( xl - 1, tick_pos_n ),
                                       QPointF ( xl, tick_pos_n - 1 )};

    pd.qpnt.drawPolygon ( points, num_pts );
  }

  // Solid line
  {
    col.setAlpha ( 200 );
    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( col );

    pd.qpnt.drawRect ( tick_start, tick_pos_n, tick_width_n, 1.0 );
  }
}

// Marker painting

int
DS_Slider_Painter_Bevelled::paint_marker ( ::dpe::Paint_Job * pjob_n,
                                           PData & pd )
{
  int res ( 0 );

  //::std::cout << "DS_Slider_Painter_Bevelled::paint_marker\n";

  switch ( pjob_n->img_idx ) {
  case 0:
    paint_marker_current ( pd );
    break;
  case 1:
    paint_marker_hint ( pd );
    break;
  default:
    res = -1;
    break;
  }

  return res;
}

void
DS_Slider_Painter_Bevelled::paint_marker_current ( PData & pd )
{
  int bevel ( pd.width () / 3 );
  bevel = qMax ( bevel, 2 );

  { // Background
    const unsigned int num_pts ( 9 );

    const double x0 ( 0.0 );
    const double x1 ( bevel );
    const double x2 ( pd.width () - bevel );
    const double x3 ( pd.width () );

    const double y0 ( 0.0 );
    const double y1 ( bevel );
    const double y2 ( pd.height () - bevel );
    const double y3 ( pd.height () );

    const QPointF points[ num_pts ] = {QPointF ( x0, y1 ),
                                       QPointF ( x1, y0 ),
                                       QPointF ( x2, y0 ),
                                       QPointF ( x3, y1 ),
                                       QPointF ( x3, y2 ),
                                       QPointF ( x2, y3 ),
                                       QPointF ( x1, y3 ),
                                       QPointF ( x0, y2 ),
                                       QPointF ( x0, y1 )};

    {
      QColor col ( pd.pal.color ( QPalette::WindowText ) );
      col.setAlpha ( 64 );
      pd.qpnt.setPen ( Qt::NoPen );
      pd.qpnt.setBrush ( col );
    }
    pd.qpnt.drawPolygon ( points, num_pts );
  }

  { // Foreground
    const unsigned int num_pts ( 9 );

    double delta ( pd.width () / 6.0 );
    if ( delta < 1.0 ) {
      delta = 1.0;
    }
    double delta_sq ( delta * ( ::std::sqrt ( 2.0 ) - 1.0 ) );

    const double x0 ( delta );
    const double x1 ( bevel + delta_sq );
    const double x2 ( pd.width () - x1 );
    const double x3 ( pd.width () - x0 );

    const double y0 ( delta );
    const double y1 ( bevel + delta_sq );
    const double y2 ( pd.height () - y1 );
    const double y3 ( pd.height () - y0 );

    const QPointF points[ num_pts ] = {QPointF ( x0, y1 ),
                                       QPointF ( x1, y0 ),
                                       QPointF ( x2, y0 ),
                                       QPointF ( x3, y1 ),
                                       QPointF ( x3, y2 ),
                                       QPointF ( x2, y3 ),
                                       QPointF ( x1, y3 ),
                                       QPointF ( x0, y2 ),
                                       QPointF ( x0, y1 )};

    {
      const QColor & col ( pd.pal.color ( QPalette::WindowText ) );
      pd.qpnt.setPen ( Qt::NoPen );
      pd.qpnt.setBrush ( col );
    }
    pd.qpnt.drawPolygon ( points, num_pts );
  }
}

void
DS_Slider_Painter_Bevelled::paint_marker_hint ( PData & pd )
{
  int bevel ( pd.width () / 3 );
  bevel = qMax ( bevel, 2 );

  {
    const unsigned int num_pts ( 9 );

    const double x0 ( 0.0 );
    const double x1 ( bevel );
    const double x2 ( pd.width () - bevel );
    const double x3 ( pd.width () );

    const double y0 ( 0.0 );
    const double y1 ( bevel );
    const double y2 ( pd.height () - bevel );
    const double y3 ( pd.height () );

    const QPointF points[ num_pts ] = {
        QPointF ( x0, y1 ),
        QPointF ( x1, y0 ),
        QPointF ( x2, y0 ),
        QPointF ( x3, y1 ),
        QPointF ( x3, y2 ),
        QPointF ( x2, y3 ),
        QPointF ( x1, y3 ),
        QPointF ( x0, y2 ),
        QPointF ( x0, y1 ),
    };

    {
      QColor col ( pd.pal.color ( QPalette::WindowText ) );
      col.setAlpha ( 100 );
      pd.qpnt.setPen ( Qt::NoPen );
      pd.qpnt.setBrush ( col );
    }
    pd.qpnt.drawPolygon ( points, num_pts );
  }
}

// Frame painting

int
DS_Slider_Painter_Bevelled::paint_frame ( ::dpe::Paint_Job * pjob_n,
                                          PData & pd )
{
  //::std::cout << "DS_Slider_Painter_Bevelled::paint_frame " << pjob_n->img_idx
  //<< "\n";

  // Calculate state flags
  {
    pd.has_focus = ( pjob_n->img_idx == 0 );
    pd.has_weak_focus = ( pjob_n->img_idx == 1 );
  }

  // Paint
  paint_frame_deco ( pd );

  return 0;
}

void
DS_Slider_Painter_Bevelled::paint_frame_deco ( PData & pd )
{
  const QColor col_norm ( pd.pal.color ( QPalette::Button ) );
  const QColor col_text ( pd.pal.color ( QPalette::ButtonText ) );
  const QColor col_text2 ( pd.pal.color ( QPalette::WindowText ) );
  const QColor col_high ( pd.pal.color ( QPalette::Highlight ) );

  {
    QColor col;
    if ( pd.has_weak_focus ) {
      col = col_high;
    } else {
      col = ::Wdg::col_mix ( col_text, col_text2, 2, 1 );
      col = ::Wdg::col_mix ( col, col_norm, 3, 1 );
    }

    int frw ( qMax ( 1, pd.bw - 2 * pd.ew ) );
    double shrink ( pd.ew );

    if ( frw < 2 ) {
      if ( !pd.has_weak_focus ) {
        if ( pd.bw > 2 ) {
          frw = qMin ( 2, pd.bw );
        } else {
          // shrink = false;
        }
      }
    }

    // Fill
    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( col );
    pd.qpnt.drawPath ( path_bevel_frame ( pd.rectf, pd.bevel, frw, shrink ) );
  }
}

// Handle painting

int
DS_Slider_Painter_Bevelled::paint_handle ( ::dpe::Paint_Job * pjob_n,
                                           PData & pd )
{
  pd.bw = pd.width () / 24;
  pd.bw = qMax ( 2, pd.bw );
  pd.ew = qMax ( 1, ( pd.bw / 4 ) );

  {
    pd.has_focus = ( ( pjob_n->img_idx == 1 ) || ( pjob_n->img_idx == 3 ) );
    pd.mouse_over = ( pjob_n->img_idx >= 2 );
    pd.is_down = ( pjob_n->img_idx == 4 );
  }

  // Painting
  {
    paint_handle_area ( pd );
    paint_handle_frame ( pd );
    paint_handle_items ( pd );
  }

  return 0;
}

void
DS_Slider_Painter_Bevelled::paint_handle_area ( PData & pd )
{
  const int iw ( pd.width () - 2 * pd.bw );
  const int ih ( pd.height () - 2 * pd.bw );

  QPainterPath area_path;
  papp_bevel_area ( area_path, pd.rectf, pd.bevel, pd.bw / 2.0 );

  // Background
  {
    const double grw ( iw / 3.0 );
    const double grwn ( grw / double ( iw ) );

    QColor col_base ( pd.pal.color ( QPalette::Window ) );

    if ( pd.pal.currentColorGroup () == QPalette::Disabled ) {
      QColor col_btn ( pd.pal.color ( QPalette::Button ) );
      col_base = col_mix ( col_base, col_btn, 2, 1 );
    }

    QColor col_edge ( col_base );
    QColor col_center ( col_base );

    col_edge.setAlpha ( 170 );
    col_center.setAlpha ( 145 );

    QLinearGradient lgrad ( QPointF ( pd.bw, 0.0 ),
                            QPointF ( pd.width () - pd.bw, 0.0 ) );

    lgrad.setColorAt ( 0.0, col_edge );
    lgrad.setColorAt ( grwn, col_center );
    lgrad.setColorAt ( 1.0 - grwn, col_center );
    lgrad.setColorAt ( 1.0, col_edge );

    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( lgrad );
  }
  pd.qpnt.drawPath ( area_path );

  // Highlight
  {
    QColor col_edge ( pd.pal.color ( QPalette::Light ) );
    QColor col_trans ( pd.pal.color ( QPalette::Light ) );
    col_edge.setAlpha ( 128 );
    col_trans.setAlpha ( 30 );

    const double hl_dy ( ih / 8.0 );
    const double y_top ( pd.bw );
    const double y_bottom ( pd.height () - pd.bw );

    QLinearGradient lgrad;

    if ( pd.is_down ) {
      lgrad.setStart ( 0.0, y_bottom - hl_dy );
      lgrad.setFinalStop ( 0.0, y_top );
    } else {
      lgrad.setStart ( 0.0, y_top + hl_dy );
      lgrad.setFinalStop ( 0.0, y_bottom );

      if ( pd.mouse_over ) {
        col_edge.setAlpha ( 150 );
      }
    }

    // Adjust gradient pattern
    lgrad.setColorAt ( 0.0, col_edge );
    lgrad.setColorAt ( 1.0, col_trans );
    lgrad.setSpread ( QGradient::ReflectSpread );

    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( lgrad );
  }
  pd.qpnt.drawPath ( area_path );
}

void
DS_Slider_Painter_Bevelled::paint_handle_frame ( PData & pd )
{
  QColor col;
  {
    // const QColor col_btn ( pd.pal.color ( QPalette::Button ) );
    const QColor col_bg ( pd.pal.color ( QPalette::Window ) );
    const QColor col_fg ( pd.pal.color ( QPalette::WindowText ) );
    col = ::Wdg::col_mix ( col_bg, col_fg, 1, 1 );
  }

  paint_bevel_raised_frame ( pd, pd.rectf, pd.bevel, pd.bw, pd.ew, col );
}

void
DS_Slider_Painter_Bevelled::paint_handle_items ( PData & pd )
{
  // Colors
  QColor col_bg ( pd.pal.color ( QPalette::Window ) );

  QColor col_light ( pd.pal.color ( QPalette::Light ) );
  col_light = ::Wdg::col_mix ( col_light, col_bg, 3, 2 );

  QColor col_dark ( pd.pal.color ( QPalette::WindowText ) );
  col_dark = ::Wdg::col_mix ( col_dark, col_bg, 5, 1 );

  // Transforms
  const QTransform trans_hmirror (
      -1.0, 0.0, 0.0, 0.0, 1.0, 0.0, pd.width (), 0.0, 1.0 );

  // Variables
  const double center_v ( pd.height () / 2.0 );
  const double height_sub ( pd.bevel + pd.bw * ( sqrt ( 2.0 ) - 1.0 ) );
  const double line_width_long ( 1.3333 ); // Width of the bright border
  const double line_width_small ( 1.25 );  // Width of the bright border
  const double line_width_fine ( 1.0 );    // Width of the bright border
  double tri_height_base;
  double tri_width;

  {
    const double in_w ( pd.width () - 2 * pd.bw );
    const double in_h ( pd.height () - 2 * pd.bw );
    const double tri_len_h ( in_w / 6.0f );
    const double tri_len_v ( in_h / 6.0f );
    const double tri_len ( qMin ( tri_len_v, tri_len_h ) );
    tri_height_base = tri_len;
    tri_width = tri_len;
    tri_width += 1;
  }

  if ( pd.is_down ) {
    const double down_scale ( 3.0 / 4.0 );
    tri_width *= down_scale;
  }
  // Round
  tri_width = ::std::floor ( tri_width + 0.5 );

  // Install clipping region
  {
    QPainterPath area_path;
    papp_bevel_area ( area_path, pd.rectf, pd.bevel, pd.bw );
    pd.qpnt.setClipPath ( area_path );
  }

  { // Paint long piece
    QPainterPath ppath;

    {
      const double bb_w_loc ( line_width_long / 2.0 );
      const double xoff ( pd.bw );

      double pp_w ( tri_width - bb_w_loc );
      double pp_h ( pd.height () / 2.0 - height_sub - bb_w_loc );

      const QPointF ptop ( xoff, center_v - pp_h );
      const QPointF pbot ( xoff, center_v + pp_h );
      const QPointF pmid_top ( xoff + pp_w, center_v - 0.5 );
      const QPointF pmid_bot ( xoff + pp_w, center_v + 0.5 );
      const double sl_dk_x1 ( pp_w / 2.25 );
      const double sl_dk_y1 ( pp_w / 2.5 );
      const double sl_dk_y2 ( pp_h / 4.0 );

      // Create path
      ppath.moveTo ( 0.0, ptop.y () );
      ppath.lineTo ( ptop.x (), ptop.y () );
      ppath.cubicTo ( QPointF ( ptop.x () + sl_dk_x1, ptop.y () + sl_dk_y1 ),
                      QPointF ( pmid_top.x (), pmid_top.y () - sl_dk_y2 ),
                      QPointF ( pmid_top.x (), pmid_top.y () ) );
      ppath.lineTo ( pmid_bot.x (), pmid_bot.y () );
      ppath.cubicTo ( QPointF ( pmid_bot.x (), pmid_bot.y () + sl_dk_y2 ),
                      QPointF ( pbot.x () + sl_dk_x1, pbot.y () - sl_dk_y1 ),
                      QPointF ( pbot.x (), pbot.y () ) );
      ppath.lineTo ( 0.0, pbot.y () );
      ppath.closeSubpath ();
    }

    {
      QPen pen;
      pen.setWidthF ( line_width_long );
      pen.setColor ( col_light );
      pd.qpnt.setPen ( pen );
    }
    pd.qpnt.setBrush ( col_dark );

    pd.qpnt.drawPath ( ppath );
    ppath = trans_hmirror.map ( ppath );
    pd.qpnt.drawPath ( ppath );
  }

  { // Paint small triangle
    QPainterPath ppath;

    {
      double tri_height ( tri_height_base );
      tri_height = std::floor ( tri_height ) + 0.5;

      const double xoff ( pd.bw );
      const double pp_w ( tri_width );
      const double pp_h ( tri_height );

      // Create path
      ppath.moveTo ( 0, center_v - pp_h );
      ppath.lineTo ( xoff, center_v - pp_h );
      ppath.lineTo ( xoff, center_v - pp_h );
      ppath.lineTo ( xoff + pp_w, center_v );
      ppath.lineTo ( xoff, center_v + pp_h );
      ppath.lineTo ( 0, center_v + pp_h );
      ppath.closeSubpath ();
    }

    {
      QPen pen;
      pen.setWidthF ( line_width_small );
      pen.setColor ( col_light );
      pd.qpnt.setPen ( pen );
    }
    pd.qpnt.setBrush ( col_dark );

    pd.qpnt.drawPath ( ppath );
    ppath = trans_hmirror.map ( ppath );
    pd.qpnt.drawPath ( ppath );
  }

  { // Center line
    const double xoff ( pd.bw + tri_width );

    // Bright background
    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( col_light );
    pd.qpnt.drawRect ( xoff,
                       center_v - 0.5 - line_width_fine,
                       pd.width () - 2 * xoff,
                       1.0 + line_width_fine * 2 );

    // Dark foreground
    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( col_dark );
    pd.qpnt.drawRect ( pd.bw, center_v - 0.5f, pd.width () - 2 * pd.bw, 1.0 );
  }

  // Remove clipping region
  pd.qpnt.setClipping ( false );
}

void
DS_Slider_Painter_Bevelled::papp_bevel_area ( QPainterPath & ppath_n,
                                              const QRectF & area_n,
                                              double bevel_n,
                                              double indent_n )
{
  const double angle_tan ( ::std::tan ( 22.5 / 180.0 * M_PI ) );
  double ds ( indent_n );
  double db ( bevel_n + angle_tan * indent_n );

  double xl[ 2 ] = {area_n.left (), area_n.left () + area_n.width ()};
  double yl[ 2 ] = {area_n.top (), area_n.top () + area_n.height ()};

  ppath_n.moveTo ( xl[ 0 ] + ds, yl[ 0 ] + db ),
      ppath_n.lineTo ( xl[ 0 ] + db, yl[ 0 ] + ds ),
      ppath_n.lineTo ( xl[ 1 ] - db, yl[ 0 ] + ds ),
      ppath_n.lineTo ( xl[ 1 ] - ds, yl[ 0 ] + db ),

      ppath_n.lineTo ( xl[ 1 ] - ds, yl[ 1 ] - db ),
      ppath_n.lineTo ( xl[ 1 ] - db, yl[ 1 ] - ds ),
      ppath_n.lineTo ( xl[ 0 ] + db, yl[ 1 ] - ds ),
      ppath_n.lineTo ( xl[ 0 ] + ds, yl[ 1 ] - db ), ppath_n.closeSubpath ();
}

void
DS_Slider_Painter_Bevelled::paint_bevel_raised_frame ( PData & pd,
                                                       const QRectF & area_n,
                                                       double bevel_n,
                                                       double frame_width_n,
                                                       double edge_width_n,
                                                       const QColor & col_n )
{
  QColor col_light ( pd.pal.color ( QPalette::Light ) );
  QColor col_shadow ( pd.pal.color ( QPalette::Shadow ) );

  QColor col_mid ( col_n );
  QColor col_br ( ::Wdg::col_mix ( col_n, col_light, 5, 4 ) );
  QColor col_dk ( ::Wdg::col_mix ( col_n, col_shadow, 5, 4 ) );

  const double shrink_out ( 0.0 );
  const double shrink_in ( frame_width_n - edge_width_n );

  // Bright area
  {
    QPainterPath pp;
    papp_bevel_frame_edge ( pp, area_n, 3, bevel_n, edge_width_n, shrink_out );
    papp_bevel_frame_corner (
        pp, area_n, 0, bevel_n, edge_width_n, shrink_out );
    papp_bevel_frame_edge ( pp, area_n, 0, bevel_n, edge_width_n, shrink_out );

    papp_bevel_frame_edge ( pp, area_n, 1, bevel_n, edge_width_n, shrink_in );
    papp_bevel_frame_corner ( pp, area_n, 2, bevel_n, edge_width_n, shrink_in );
    papp_bevel_frame_edge ( pp, area_n, 2, bevel_n, edge_width_n, shrink_in );

    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( col_br );
    pd.qpnt.drawPath ( pp );
  }

  // Mid area
  {
    QPainterPath pp;
    papp_bevel_frame_corner (
        pp, area_n, 1, bevel_n, edge_width_n, shrink_out );
    papp_bevel_frame_corner ( pp, area_n, 1, bevel_n, edge_width_n, shrink_in );
    papp_bevel_frame_corner (
        pp, area_n, 3, bevel_n, edge_width_n, shrink_out );
    papp_bevel_frame_corner ( pp, area_n, 3, bevel_n, edge_width_n, shrink_in );

    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( col_mid );
    pd.qpnt.drawPath ( pp );
  }

  // Dark area
  {
    QPainterPath pp;
    papp_bevel_frame_edge ( pp, area_n, 3, bevel_n, edge_width_n, shrink_in );
    papp_bevel_frame_corner ( pp, area_n, 0, bevel_n, edge_width_n, shrink_in );
    papp_bevel_frame_edge ( pp, area_n, 0, bevel_n, edge_width_n, shrink_in );

    papp_bevel_frame_edge ( pp, area_n, 1, bevel_n, edge_width_n, shrink_out );
    papp_bevel_frame_corner (
        pp, area_n, 2, bevel_n, edge_width_n, shrink_out );
    papp_bevel_frame_edge ( pp, area_n, 2, bevel_n, edge_width_n, shrink_out );

    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( col_dk );
    pd.qpnt.drawPath ( pp );
  }

  // Frame center line area
  const double mid_width ( frame_width_n - 2.0 * edge_width_n );
  if ( mid_width > 0.0 ) {
    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( col_mid );
    pd.qpnt.drawPath (
        path_bevel_frame ( area_n, bevel_n, mid_width, edge_width_n ) );
  }
}

QPainterPath
DS_Slider_Painter_Bevelled::path_bevel_frame ( const QRectF & area_n,
                                               double bevel_n,
                                               double frame_width_n,
                                               double indent_n )
{
  QPainterPath ppath;
  papp_bevel_area ( ppath, area_n, bevel_n, indent_n );
  papp_bevel_area ( ppath, area_n, bevel_n, indent_n + frame_width_n );
  return ppath;
}

void
DS_Slider_Painter_Bevelled::papp_bevel_frame_corner ( QPainterPath & ppath_n,
                                                      const QRectF & area_n,
                                                      unsigned int edge_n,
                                                      double bevel_n,
                                                      double width_n,
                                                      double indent_n )
{
  double xx[ 4 ];
  double yy[ 4 ];

  {
    const double angle_tan ( ::std::tan ( 22.5 / 180.0 * M_PI ) );
    double ds1 ( indent_n );
    double db1 ( bevel_n + angle_tan * ds1 );
    double ds2 ( indent_n + width_n );
    double db2 ( bevel_n + angle_tan * ds2 );

    xx[ 0 ] = ds1;
    xx[ 1 ] = db1;
    xx[ 2 ] = db2;
    xx[ 3 ] = ds2;

    yy[ 0 ] = db1;
    yy[ 1 ] = ds1;
    yy[ 2 ] = ds2;
    yy[ 3 ] = db2;
  }

  {
    double x_off ( area_n.left () );
    double y_off ( area_n.top () );
    double x_scale ( 1.0 );
    double y_scale ( 1.0 );

    if ( edge_n == 0 ) {
      // pass
    } else if ( edge_n == 1 ) {
      x_off += area_n.width ();
      x_scale = -1;
    } else if ( edge_n == 2 ) {
      x_off += area_n.width ();
      y_off += area_n.height ();
      x_scale = -1;
      y_scale = -1;
    } else {
      y_off += area_n.height ();
      y_scale = -1;
    }

    for ( unsigned int ii = 0; ii < 4; ++ii ) {
      xx[ ii ] *= x_scale;
    }
    for ( unsigned int ii = 0; ii < 4; ++ii ) {
      yy[ ii ] *= y_scale;
    }
    for ( unsigned int ii = 0; ii < 4; ++ii ) {
      xx[ ii ] += x_off;
    }
    for ( unsigned int ii = 0; ii < 4; ++ii ) {
      yy[ ii ] += y_off;
    }
  }

  ppath_n.moveTo ( xx[ 0 ], yy[ 0 ] );
  ppath_n.lineTo ( xx[ 1 ], yy[ 1 ] );
  ppath_n.lineTo ( xx[ 2 ], yy[ 2 ] );
  ppath_n.lineTo ( xx[ 3 ], yy[ 3 ] );
  ppath_n.closeSubpath ();
}

void
DS_Slider_Painter_Bevelled::papp_bevel_frame_edge ( QPainterPath & ppath_n,
                                                    const QRectF & area_n,
                                                    unsigned int edge_n,
                                                    double bevel_n,
                                                    double width_n,
                                                    double indent_n )
{
  double xx[ 4 ];
  double yy[ 4 ];

  {
    const double angle_tan ( ::std::tan ( 22.5 / 180.0 * M_PI ) );
    double ds1 ( indent_n );
    double db1 ( bevel_n + angle_tan * ds1 );
    double ds2 ( indent_n + width_n );
    double db2 ( bevel_n + angle_tan * ds2 );

    if ( ( edge_n % 2 ) == 0 ) { // top / bottom
      xx[ 0 ] = area_n.left () + db1;
      xx[ 1 ] = area_n.left () + area_n.width () - db1;
      xx[ 2 ] = area_n.left () + area_n.width () - db2;
      xx[ 3 ] = area_n.left () + db2;

      yy[ 0 ] = ds1;
      yy[ 1 ] = ds1;
      yy[ 2 ] = ds2;
      yy[ 3 ] = ds2;
    } else { // left / right
      xx[ 0 ] = ds1;
      xx[ 1 ] = ds1;
      xx[ 2 ] = ds2;
      xx[ 3 ] = ds2;

      yy[ 0 ] = area_n.top () + area_n.height () - db1;
      yy[ 1 ] = area_n.top () + db1;
      yy[ 2 ] = area_n.top () + db2;
      yy[ 3 ] = area_n.top () + area_n.height () - db2;
    }
  }

  // Flip sides
  if ( edge_n == 1 ) { // flip left to right
    for ( unsigned int ii = 0; ii < 4; ++ii ) {
      xx[ ii ] *= -1;
    }
    const double x_off ( area_n.left () + area_n.width () );
    for ( unsigned int ii = 0; ii < 4; ++ii ) {
      xx[ ii ] += x_off;
    }
  } else if ( edge_n == 2 ) { // flip top to bottom
    for ( unsigned int ii = 0; ii < 4; ++ii ) {
      yy[ ii ] *= -1;
    }
    const double y_off ( area_n.top () + area_n.height () );
    for ( unsigned int ii = 0; ii < 4; ++ii ) {
      yy[ ii ] += y_off;
    }
  }

  ppath_n.moveTo ( xx[ 0 ], yy[ 0 ] );
  ppath_n.lineTo ( xx[ 1 ], yy[ 1 ] );
  ppath_n.lineTo ( xx[ 2 ], yy[ 2 ] );
  ppath_n.lineTo ( xx[ 3 ], yy[ 3 ] );
  ppath_n.closeSubpath ();
}

} // namespace Painter
} // namespace Wdg
