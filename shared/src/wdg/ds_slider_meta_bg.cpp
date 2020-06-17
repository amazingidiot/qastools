/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "ds_slider_meta_bg.hpp"
#include "wdg/ds_widget_types.hpp"

namespace Wdg
{

DS_Slider_Meta_Bg::DS_Slider_Meta_Bg ( unsigned int num_images_n,
                                       unsigned int group_type_n,
                                       unsigned int type_id_n )
: ::dpe::Image_Set_Meta ( num_images_n, group_type_n, type_id_n )
, ticks_max_idx ( 0 )
, ticks_range_max_idx ( 0 )
, ticks_range_start ( 0 )
, bg_show_image ( 0 )
, bg_tick_min_idx ( 0 )
{
}

DS_Slider_Meta_Bg::~DS_Slider_Meta_Bg () {}

bool
DS_Slider_Meta_Bg::matches ( const ::dpe::Image_Set_Meta * meta_n ) const
{
  bool res ( false );

  const ::Wdg::DS_Slider_Meta_Bg * meta_c (
      dynamic_cast< const ::Wdg::DS_Slider_Meta_Bg * > ( meta_n ) );
  if ( meta_c != 0 ) {
    res = ::dpe::Image_Set_Meta::matches ( meta_n );
    res = res && ( meta_c->ticks_max_idx == ticks_max_idx );
    res = res && ( meta_c->ticks_range_max_idx == ticks_range_max_idx );
    res = res && ( meta_c->ticks_range_start == ticks_range_start );
    res = res && ( meta_c->bg_show_image == bg_show_image );
    res = res && ( meta_c->bg_tick_min_idx == bg_tick_min_idx );
  }

  return res;
}

::dpe::Image_Set_Meta *
DS_Slider_Meta_Bg::new_copy () const
{
  return new ::Wdg::DS_Slider_Meta_Bg ( *this );
}

} // namespace Wdg
