/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "ds_switch_painter_circle.hpp"
#include "dpe/image_set.hpp"
#include "dpe/image_set_meta.hpp"
#include "dpe/paint_job.hpp"
#include "wdg/color_methods.hpp"
#include "wdg/ds_widget_style_db.hpp"
#include "wdg/ds_widget_types.hpp"
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QRadialGradient>
#include <QScopedPointer>

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

namespace Wdg
{
namespace Painter
{

struct DS_Switch_Painter_Circle::PData
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

  int max_len;
  int min_len;
  int border_width;
  int gap_width;

  double center_x;
  double center_y;

  bool checked;
  bool half_checked;
  bool has_focus;
  bool mouse_over;

  QColor col_border;
  QColor col_focus;
  QRectF rectf;

  ::dpe::Image * img;
};

DS_Switch_Painter_Circle::DS_Switch_Painter_Circle ()
: ::Wdg::Painter::DS_Widget_Painter ( ::Wdg::DS_SWITCH )
{
}

int
DS_Switch_Painter_Circle::paint_image ( ::dpe::Paint_Job * pjob_n )
{
  int res ( 0 );

  // Init paint data
  PData pd;

  pd.meta = pjob_n->meta;
  pd.img = &pjob_n->img_set->image ( pjob_n->img_idx );

  res = create_image_data ( pd.img, pd.meta );

  if ( res == 0 ) {

    if ( wdg_style_db () != 0 ) {
      pd.pal = wdg_style_db ()->palettes[ pd.meta->style_id ];
      pd.pal.setCurrentColorGroup (
          wdg_style_db ()->color_group ( pd.meta->style_sub_id ) );
    }
    pd.rectf.setRect ( 0.0, 0.0, pd.width (), pd.height () );

    pd.min_len = qMin ( pd.width (), pd.height () );
    pd.max_len = qMax ( pd.width (), pd.height () );
    pd.border_width = qMax ( pd.min_len / 16, 1 );
    pd.gap_width = qMax ( pd.min_len / 8, 2 );

    pd.center_x = pd.width () / 2.0;
    pd.center_y = pd.height () / 2.0;

    switch ( pd.meta->type_id ) {
    case 0:
      res = paint_bg ( pjob_n, pd );
      break;
    case 1:
      res = paint_handle ( pjob_n, pd );
      break;
    default:
      break;
    }
  }

  return res;
}

int
DS_Switch_Painter_Circle::paint_bg ( ::dpe::Paint_Job * pjob_n, PData & pd )
{
  {
    const QColor col_bg ( pd.pal.color ( QPalette::Button ) );
    const QColor col_fg ( pd.pal.color ( QPalette::ButtonText ) );
    pd.col_border = ::Wdg::col_mix ( col_bg, col_fg, 1, 1 );
  }

  {
    const QColor col_bg ( pd.pal.color ( QPalette::Button ) );
    const QColor col_fg ( pd.pal.color ( QPalette::ButtonText ) );
    pd.col_focus = ::Wdg::col_mix ( col_bg, col_fg, 1, 2 );
  }

  // Calc state flags
  {
    unsigned int state_mod ( pjob_n->img_idx % 2 );
    unsigned int state_div ( pjob_n->img_idx / 2 );
    pd.has_focus = ( state_mod != 0 );
    pd.mouse_over = ( state_div != 0 );
  }

  {
    // Init painter
    pd.qpnt.begin ( &pd.img->qimage () );
    pd.qpnt.setRenderHints ( QPainter::Antialiasing |
                             QPainter::SmoothPixmapTransform );

    // Painting
    paint_bg_area ( pd );
    paint_bg_deco ( pd );
    paint_bg_border ( pd );
  }

  return 0;
}

void
DS_Switch_Painter_Circle::paint_bg_area ( PData & pd )
{
  {
    double rad ( ( pd.min_len - pd.border_width ) / 2.0 );

    QRadialGradient rgrad ( QPointF ( pd.center_x, pd.center_y ), rad );

    {
      // Color mixing
      int w_dk ( 40 );
      if ( pd.mouse_over ) {
        w_dk = 45;
      }

      QColor col_dk ( pd.pal.color ( QPalette::Window ) );
      QColor col_win ( pd.pal.color ( QPalette::Button ) );
      QColor col_mix ( ::Wdg::col_mix ( col_dk, col_win, w_dk, 255 - w_dk ) );

      rgrad.setColorAt ( 0.0, col_win );
      rgrad.setColorAt ( 1.0, col_mix );

      pd.qpnt.setPen ( Qt::NoPen );
      pd.qpnt.setBrush ( rgrad );
    }

    pd.qpnt.drawEllipse ( QPointF ( pd.center_x, pd.center_y ), rad, rad );
  }
}

void
DS_Switch_Painter_Circle::paint_bg_deco ( PData & pd )
{
  // Mouse over
  if ( pd.mouse_over ) {
    paint_highlight ( pd );
  }
}

void
DS_Switch_Painter_Circle::paint_bg_border ( PData & pd )
{
  // Border
  {
    double rad ( ( pd.min_len - pd.border_width ) / 2.0 );

    {
      QPen pen;
      pen.setWidthF ( pd.border_width );
      pen.setColor ( pd.col_border );

      pd.qpnt.setBrush ( Qt::NoBrush );
      pd.qpnt.setPen ( pen );
    }

    pd.qpnt.drawEllipse ( QPointF ( pd.center_x, pd.center_y ), rad, rad );
  }

  if ( pd.has_focus ) {
    paint_focus_path ( pd );
  }
}

// Handle

int
DS_Switch_Painter_Circle::paint_handle ( ::dpe::Paint_Job * pjob_n, PData & pd )
{
  // Calc state flags
  {
    unsigned int state_mod ( pjob_n->img_idx % 2 );
    pd.has_focus = ( state_mod != 0 );
    pd.mouse_over = ( ( pjob_n->img_idx == 2 ) || ( pjob_n->img_idx == 3 ) ||
                      ( pjob_n->img_idx == 6 ) || ( pjob_n->img_idx == 7 ) );
    pd.half_checked = ( pjob_n->img_idx > 7 );
  }

  // Painting
  {
    if ( pjob_n->img_idx < 4 ) {
      // Empty images for the first variants
      pd.img->clear ();
    } else {
      // Init painter
      pd.qpnt.begin ( &pd.img->qimage () );
      pd.qpnt.setRenderHints ( QPainter::Antialiasing |
                               QPainter::SmoothPixmapTransform );

      // Painting
      paint_handle_area ( pd );
      paint_handle_deco ( pd );
    }
  }

  return 0;
}

void
DS_Switch_Painter_Circle::paint_handle_area ( PData & pd )
{
  double rad ( pd.min_len / 2.0 - pd.border_width - pd.gap_width );
  double fade_offset ( 0.2 );

  QColor col_full ( pd.pal.color ( QPalette::WindowText ) );

  if ( pd.half_checked ) {
    // Gradient color circle
    QColor col_trans ( col_full );
    col_trans.setAlpha ( 64 );

    QRadialGradient rgrad ( QPointF ( pd.center_x, pd.center_y ), rad );
    rgrad.setColorAt ( 0.0, col_full );
    rgrad.setColorAt ( fade_offset, col_full );
    rgrad.setColorAt ( 1.0, col_trans );

    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( rgrad );
  } else {
    // Full color circle
    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( col_full );
  }

  pd.qpnt.drawEllipse ( QPointF ( pd.center_x, pd.center_y ), rad, rad );
}

void
DS_Switch_Painter_Circle::paint_handle_deco ( PData & pd )
{
  if ( pd.mouse_over ) {
    paint_highlight ( pd );
  }
}

void
DS_Switch_Painter_Circle::paint_highlight ( PData & pd )
{
  const double rad ( pd.min_len / 2.0 - pd.border_width );

  {
    const double diam0 ( pd.min_len - pd.border_width );
    const double diam1 ( pd.min_len - pd.border_width - pd.gap_width );
    const double diam2 (
        qMax ( ( diam1 - pd.gap_width * 6.0 ), pd.gap_width * 3.0 ) );

    QColor cf ( pd.pal.color ( QPalette::Highlight ) );
    QColor ca ( cf );

    cf.setAlpha ( 180 );
    ca.setAlpha ( 0 );

    double w0 = 0.0;
    double w1 = diam2 / diam0;
    double w2 = diam1 / diam0;
    double w3 = 1.0;

    QRadialGradient rgrad ( QPointF ( pd.center_x, pd.center_y ), rad );

    rgrad.setColorAt ( w0, ca );
    rgrad.setColorAt ( w1, ca );
    rgrad.setColorAt ( w2, cf );
    rgrad.setColorAt ( w3, cf );

    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( rgrad );
  }

  pd.qpnt.drawEllipse ( QPointF ( pd.center_x, pd.center_y ), rad, rad );
}

void
DS_Switch_Painter_Circle::paint_focus_path ( PData & pd )
{
  QPainterPath ppath[ 4 ];

  {
    const QPointF center ( pd.center_x, pd.center_y );
    const double rad ( pd.min_len / 2.0 );
    const double big_rad ( rad - pd.border_width );

    const double rscale ( M_PI / 180.0 );
    const double ang_delta ( 90.0 * rscale );
    double angle ( ( 90.0 / 2.0 ) * rscale );

    double crad ( rad - big_rad * ::std::cos ( angle ) );
    crad *= 2.0 / 3.0;

    for ( unsigned int ii = 0; ii < 4; ++ii ) {
      QPointF ccen ( ::std::cos ( angle ), ::std::sin ( angle ) );
      ccen *= big_rad;
      ccen += center;
      ppath[ ii ].addEllipse ( ccen, crad, crad );
      angle += ang_delta;
    }
  }

  // Subtract inner circle
  {
    QPainterPath ppathc;

    QRectF brect ( pd.rectf );
    const double adj ( pd.border_width / 2.0 );
    brect.adjust ( adj, adj, -adj, -adj );

    ppathc.addEllipse ( brect );
    for ( unsigned int ii = 0; ii < 4; ++ii ) {
      ppath[ ii ] = ppath[ ii ].subtracted ( ppathc );
    }
  }

  pd.qpnt.setPen ( Qt::NoPen );
  pd.qpnt.setBrush ( pd.col_focus );
  for ( unsigned int ii = 0; ii < 4; ++ii ) {
    pd.qpnt.drawPath ( ppath[ ii ] );
  }
}

} // namespace Painter
} // namespace Wdg
