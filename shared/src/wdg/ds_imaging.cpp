/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "ds_imaging.hpp"
#include "dpe/image_allocator.hpp"
#include "dpe/image_request.hpp"
#include "dpe/image_set_meta.hpp"
#include "wdg/ds_widget_types.hpp"
#include <QWidget>

namespace Wdg
{

DS_Imaging::DS_Imaging ( unsigned int num_image_groups_n,
                         ::dpe::Image_Allocator * alloc_n )
: _images ( num_image_groups_n )
, _image_request ( 0 )
, _image_alloc ( alloc_n )
, _request_sent ( false )
{
  _image_request = new ::dpe::Image_Request ( &_images );
}

DS_Imaging::~DS_Imaging ()
{
  set_image_alloc ( 0 );
  delete _image_request;
}

void
DS_Imaging::set_image_alloc ( ::dpe::Image_Allocator * alloc_n )
{
  if ( _image_alloc == alloc_n ) {
    return;
  }

  if ( _image_alloc != 0 ) {
    wait_for_request ();
    _image_alloc->return_group ( &_images );
  }

  _image_alloc = alloc_n;
}

void
DS_Imaging::set_images_variant_id ( unsigned int variant_n )
{
  for ( unsigned int ii = 0; ii < image_request ()->meta.size (); ++ii ) {
    image_request ()->meta[ ii ]->group_variant = variant_n;
  }
}

void
DS_Imaging::set_images_style_id ( unsigned int style_n )
{
  for ( unsigned int ii = 0; ii < image_request ()->meta.size (); ++ii ) {
    image_request ()->meta[ ii ]->style_id = style_n;
  }
}

void
DS_Imaging::set_images_style_sub_id ( unsigned int style_n )
{
  for ( unsigned int ii = 0; ii < image_request ()->meta.size (); ++ii ) {
    image_request ()->meta[ ii ]->style_sub_id = style_n;
  }
}

unsigned int
DS_Imaging::style_sub_id ( QWidget * wdg_n )
{
  unsigned int res ( ::Wdg::ST_NORMAL );
  if ( !wdg_n->isEnabled () ) {
    res = ::Wdg::ST_DISABLED;
  } else if ( !wdg_n->isActiveWindow () ) {
    res = ::Wdg::ST_INACTIVE;
  }
  return res;
}

} // namespace Wdg
