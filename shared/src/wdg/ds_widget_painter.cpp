/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "ds_widget_painter.hpp"
#include "dpe/image.hpp"
#include "dpe/image_set_meta.hpp"
#include <cstring>

namespace Wdg
{
namespace Painter
{

DS_Widget_Painter::DS_Widget_Painter ( unsigned int group_type_n,
                                       unsigned int group_variant_n )
: ::dpe::Painter ( group_type_n, group_variant_n )
, _wdg_style_db ( 0 )
{
}

void
DS_Widget_Painter::set_wdg_style_db ( ::Wdg::DS_Widget_Style_Db * style_db_n )
{
  _wdg_style_db = style_db_n;
}

int
DS_Widget_Painter::create_image_data ( ::dpe::Image * img_n,
                                       const ::dpe::Image_Set_Meta * meta_n )
{
  if ( !meta_n->size.isValid () ) {
    return -1;
  }

  img_n->set_size ( meta_n->size.width (),
                    meta_n->size.height (),
                    meta_n->size.width () * 4 );
  img_n->qimage ().fill ( 0 );

  return 0;
}

} // namespace Painter
} // namespace Wdg
