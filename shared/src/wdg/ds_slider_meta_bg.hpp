/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_ds_slider_meta_bg_hpp__
#define __INC_ds_slider_meta_bg_hpp__

#include "dpe/image_set_meta.hpp"

namespace Wdg
{

/// @brief DS_Slider_Meta_Bg
///
class DS_Slider_Meta_Bg : public ::dpe::Image_Set_Meta
{
  // Public methods
  public:
  DS_Slider_Meta_Bg ( unsigned int num_images_n,
                      unsigned int group_type_n,
                      unsigned int type_id_n );

  ~DS_Slider_Meta_Bg ();

  bool
  matches ( const ::dpe::Image_Set_Meta * meta_n ) const;

  ::dpe::Image_Set_Meta *
  new_copy () const;

  // Public attributes
  unsigned int ticks_max_idx;
  int ticks_range_max_idx;
  int ticks_range_start;

  // Style
  int bg_show_image;
  unsigned int bg_tick_min_idx; // Which tick has the minimum width
};

} // namespace Wdg

#endif
