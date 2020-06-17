/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_ds_slider_painter_bevelled_hpp__
#define __INC_ds_slider_painter_bevelled_hpp__

#include "wdg/ds_widget_painter.hpp"

// Forward declaration
class QColor;
class QRectF;
class QPainterPath;

namespace Wdg
{
namespace Painter
{

class DS_Slider_Painter_Bevelled : public ::Wdg::Painter::DS_Widget_Painter
{
  // Public methods
  public:
  DS_Slider_Painter_Bevelled ();

  // Protected methods
  protected:
  int
  paint_image ( ::dpe::Paint_Job * pjob_n );

  // Private methods
  private:
  // Declaration
  struct PData;

  // Backgournd painting

  int
  paint_bg ( ::dpe::Paint_Job * pjob_n, PData & pd );

  void
  paint_bg_area ( PData & pd );

  void
  paint_bg_frame ( PData & pd );

  void
  paint_bg_area_deco ( PData & pd );

  void
  paint_bg_ticks ( PData & pd );

  void
  paint_bg_tick ( PData & pd,
                  double tick_pos_n,
                  double tick_width_n,
                  const QColor & col_n );

  // Marker painting

  int
  paint_marker ( ::dpe::Paint_Job * pjob_n, PData & pd );

  void
  paint_marker_current ( PData & pd );

  void
  paint_marker_hint ( PData & pd );

  // Frame painting

  int
  paint_frame ( ::dpe::Paint_Job * pjob_n, PData & pd );

  void
  paint_frame_deco ( PData & pd );

  // Handle painting

  int
  paint_handle ( ::dpe::Paint_Job * pjob_n, PData & pd );

  void
  paint_handle_area ( PData & pd );

  void
  paint_handle_frame ( PData & pd );

  void
  paint_handle_items ( PData & pd );

  // Shared / Utility

  void
  papp_bevel_area ( QPainterPath & ppath_n,
                    const QRectF & area_n,
                    double bevel_n,
                    double indent_n = 0.0 );

  QPainterPath
  path_bevel_frame ( const QRectF & area_n,
                     double bevel_n,
                     double width_n,
                     double indent_n = 0.0 );

  void
  paint_bevel_raised_frame ( PData & pd,
                             const QRectF & area_n,
                             double bevel_n,
                             double width_n,
                             double edge_n,
                             const QColor & col_n );

  void
  papp_bevel_frame_corner ( QPainterPath & ppath_n,
                            const QRectF & area_n,
                            unsigned int edge_n,
                            double bevel_n,
                            double width_n,
                            double indent_n );

  void
  papp_bevel_frame_edge ( QPainterPath & ppath_n,
                          const QRectF & area_n,
                          unsigned int edge_n,
                          double bevel_n,
                          double width_n,
                          double indent_n );
};

} // namespace Painter
} // namespace Wdg

#endif
