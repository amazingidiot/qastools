/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_ds_switch_painter_circle_hpp__
#define __INC_ds_switch_painter_circle_hpp__

#include "wdg/ds_widget_painter.hpp"

namespace Wdg
{
namespace Painter
{

class DS_Switch_Painter_Circle : public ::Wdg::Painter::DS_Widget_Painter
{
  // Public methods
  public:
  DS_Switch_Painter_Circle ();

  // Protected methods
  protected:
  int
  paint_image ( ::dpe::Paint_Job * pjob_n );

  // Private methods
  private:
  // Declaration
  struct PData;

  // Background

  int
  paint_bg ( ::dpe::Paint_Job * pjob_n, PData & pd );

  void
  paint_bg_area ( PData & pd );

  void
  paint_bg_deco ( PData & pd );

  void
  paint_bg_border ( PData & pd );

  // Handle

  int
  paint_handle ( ::dpe::Paint_Job * pjob_n, PData & pd );

  void
  paint_handle_area ( PData & pd );

  void
  paint_handle_deco ( PData & pd );

  // Shared

  void
  paint_highlight ( PData & pd );

  void
  paint_focus_path ( PData & pd );
};

} // namespace Painter
} // namespace Wdg

#endif
