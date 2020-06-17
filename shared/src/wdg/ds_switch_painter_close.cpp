/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "ds_switch_painter_close.hpp"
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

struct DS_Switch_Painter_Close::PData
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

  double center_x;
  double center_y;

  bool checked;
  bool half_checked;
  bool has_focus;
  bool mouse_over;

  ::dpe::Image * img;
};

DS_Switch_Painter_Close::DS_Switch_Painter_Close ()
: ::Wdg::Painter::DS_Widget_Painter ( ::Wdg::DS_SWITCH )
{
}

int
DS_Switch_Painter_Close::paint_image ( ::dpe::Paint_Job * pjob_n )
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

    pd.min_len = qMin ( pd.width (), pd.height () );
    pd.max_len = qMax ( pd.width (), pd.height () );
    pd.border_width = qMax ( pd.min_len / 16, 1 );

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
DS_Switch_Painter_Close::paint_bg ( ::dpe::Paint_Job * pjob_n, PData & pd )
{
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
    paint_bg_border ( pd );
  }

  return 0;
}

void
DS_Switch_Painter_Close::paint_bg_area ( PData & pd )
{
  {
    double rad ( ( pd.min_len - pd.border_width ) / 2.0 );
    if ( pd.mouse_over ) {
      QColor col0 ( pd.pal.color ( QPalette::Button ) );
      QColor col1 ( pd.pal.color ( QPalette::Highlight ) );

      QRadialGradient rgrad ( QPointF ( pd.center_x, pd.center_y ), rad );
      rgrad.setColorAt ( 0.0, col0 );
      rgrad.setColorAt ( 0.75, col1 );
      rgrad.setColorAt ( 1.0, col1 );

      pd.qpnt.setPen ( Qt::NoPen );
      pd.qpnt.setBrush ( rgrad );
    } else {
      QColor col ( pd.pal.color ( QPalette::Button ) );

      pd.qpnt.setPen ( Qt::NoPen );
      pd.qpnt.setBrush ( col );
    }

    pd.qpnt.drawEllipse ( QPointF ( pd.center_x, pd.center_y ), rad, rad );
  }
}

void
DS_Switch_Painter_Close::paint_bg_border ( PData & pd )
{
  // Border
  {
    double rad ( ( pd.min_len - pd.border_width ) / 2.0 );

    {
      QPen pen;
      pen.setWidthF ( pd.border_width );
      {
        QColor col_bg ( pd.pal.color ( QPalette::Button ) );
        QColor col_fg ( pd.pal.color ( QPalette::ButtonText ) );
        pen.setColor ( ::Wdg::col_mix ( col_bg, col_fg, 1, 2 ) );
      }

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
DS_Switch_Painter_Close::paint_handle ( ::dpe::Paint_Job * pjob_n, PData & pd )
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
    if ( ( pjob_n->img_idx > 3 ) && ( pjob_n->img_idx < 8 ) ) {
      // Empty images for the first variants
      pd.img->clear ();
    } else {

      // Init painter
      pd.qpnt.begin ( &pd.img->qimage () );
      pd.qpnt.setRenderHints ( QPainter::Antialiasing |
                               QPainter::SmoothPixmapTransform );

      // Paint
      paint_handle_area ( pd );
    }
  }

  return 0;
}

void
DS_Switch_Painter_Close::paint_handle_area ( PData & pd )
{
  QColor col_full;
  if ( pd.half_checked || pd.mouse_over ) {
    col_full = pd.pal.color ( QPalette::HighlightedText );
  } else {
    const QColor col_bg ( pd.pal.color ( QPalette::Button ) );
    const QColor col_fg ( pd.pal.color ( QPalette::ButtonText ) );
    col_full = ::Wdg::col_mix ( col_bg, col_fg, 1, 3 );
  }

  double gap_width = qMax ( pd.min_len / 4, 4 );

  if ( pd.half_checked ) {
    QColor col_trans ( col_full );
    col_trans.setAlpha ( 128 );

    double rad ( pd.min_len / 2.0 - pd.border_width - gap_width );

    QRadialGradient rgrad ( QPointF ( 0, 0 ), rad );
    rgrad.setColorAt ( 0.0, col_full );
    rgrad.setColorAt ( 1.0, col_trans );

    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( rgrad );
  } else {
    pd.qpnt.setPen ( Qt::NoPen );
    pd.qpnt.setBrush ( col_full );
  }

  double rwidth ( pd.min_len - pd.border_width - gap_width );
  double rheight ( 1.5 );
  if ( rheight < pd.border_width ) {
    rheight = pd.border_width;
  }

  // Paths
  pd.qpnt.translate ( pd.center_x, pd.center_y );
  pd.qpnt.rotate ( 45 );
  pd.qpnt.drawRect (
      QRectF ( -rwidth / 2.0, -rheight / 2.0, rwidth, rheight ) );
  pd.qpnt.drawRect (
      QRectF ( -rheight / 2.0, -rwidth / 2.0, rheight, rwidth ) );
  pd.qpnt.resetTransform ();
}

void
DS_Switch_Painter_Close::paint_focus_path ( PData & pd )
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

    QRectF brect ( QPointF ( 0, 0 ), pd.size () );
    const double adj ( pd.border_width / 2.0 );
    brect.adjust ( adj, adj, -adj, -adj );

    ppathc.addEllipse ( brect );
    for ( unsigned int ii = 0; ii < 4; ++ii ) {
      ppath[ ii ] = ppath[ ii ].subtracted ( ppathc );
    }
  }

  pd.qpnt.setPen ( Qt::NoPen );
  {
    const QColor col_bg ( pd.pal.color ( QPalette::Button ) );
    const QColor col_fg ( pd.pal.color ( QPalette::ButtonText ) );
    pd.qpnt.setBrush ( ::Wdg::col_mix ( col_bg, col_fg, 1, 3 ) );
  }
  for ( unsigned int ii = 0; ii < 4; ++ii ) {
    pd.qpnt.drawPath ( ppath[ ii ] );
  }
}

} // namespace Painter
} // namespace Wdg
