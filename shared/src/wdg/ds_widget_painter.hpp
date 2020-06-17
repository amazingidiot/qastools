/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_wdg_ds_widget_painter_hpp__
#define __INC_wdg_ds_widget_painter_hpp__

#include "dpe/painter.hpp"

// Forward declaration
namespace dpe
{
class Image;
class Image_Set_Meta;
} // namespace dpe
namespace Wdg
{
class DS_Widget_Style_Db;
}

namespace Wdg
{
namespace Painter
{

class DS_Widget_Painter : public ::dpe::Painter
{
  // Public methods
  public:
  DS_Widget_Painter ( unsigned int group_type_n,
                      unsigned int group_variant_n = 0 );

  // Widget style database

  ::Wdg::DS_Widget_Style_Db *
  wdg_style_db () const;

  void
  set_wdg_style_db ( ::Wdg::DS_Widget_Style_Db * style_db_n );

  // Protected methods
  protected:
  int
  create_image_data ( ::dpe::Image * img_n,
                      const ::dpe::Image_Set_Meta * meta_n );

  // Private methods
  private:
  ::Wdg::DS_Widget_Style_Db * _wdg_style_db;
};

inline ::Wdg::DS_Widget_Style_Db *
DS_Widget_Painter::wdg_style_db () const
{
  return _wdg_style_db;
}

} // namespace Painter
} // namespace Wdg

#endif
