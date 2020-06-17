/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "image_set_meta.hpp"

namespace dpe
{

Image_Set_Meta::Image_Set_Meta ( unsigned int num_images_n,
                                 unsigned int group_type_n,
                                 unsigned int type_id_n )
: type_id ( type_id_n )
, style_id ( 0 )
, style_sub_id ( 0 )
, group_type ( group_type_n )
, group_variant ( 0 )
, num_images ( num_images_n )
{
}

Image_Set_Meta::~Image_Set_Meta () {}

bool
Image_Set_Meta::matches ( const Image_Set_Meta * meta_n ) const
{
  return ( ( meta_n->type_id == type_id ) && ( meta_n->style_id == style_id ) &&
           ( meta_n->style_sub_id == style_sub_id ) &&
           ( meta_n->size == size ) && ( meta_n->group_type == group_type ) &&
           ( meta_n->group_variant == group_variant ) &&
           ( meta_n->num_images == num_images ) );
}

Image_Set_Meta *
Image_Set_Meta::new_copy () const
{
  Image_Set_Meta * res ( new Image_Set_Meta ( *this ) );
  return res;
}

} // namespace dpe
