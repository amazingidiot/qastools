/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_wdg_ds_imaging_hpp__
#define __INC_wdg_ds_imaging_hpp__

#include "dpe/image_allocator.hpp"
#include "dpe/image_request.hpp"
#include "dpe/image_set_group.hpp"

// Forward declaration
class QWidget;

namespace Wdg
{

/// @brief DS_Imaging
///
class DS_Imaging
{
  // Public methods
  public:
  DS_Imaging ( unsigned int num_image_groups_n,
               ::dpe::Image_Allocator * alloc_n = 0 );

  ~DS_Imaging ();

  // Image set group allocation

  ::dpe::Image_Allocator *
  image_alloc () const;

  void
  set_image_alloc ( ::dpe::Image_Allocator * alloc_n );

  ::dpe::Image_Set_Group &
  images ();

  ::dpe::Image_Request *
  image_request () const;

  void
  send_request ();

  void
  wait_for_request ();

  void
  set_images_variant_id ( unsigned int variant_n );

  void
  set_images_style_id ( unsigned int style_n );

  void
  set_images_style_sub_id ( unsigned int style_n );

  static unsigned int
  style_sub_id ( QWidget * wdg_n );

  // Private attributes
  private:
  // Image allocation sets
  ::dpe::Image_Set_Group _images;

  ::dpe::Image_Request * _image_request;
  ::dpe::Image_Allocator * _image_alloc;
  bool _request_sent;
};

inline ::dpe::Image_Set_Group &
DS_Imaging::images ()
{
  return _images;
}

inline ::dpe::Image_Request *
DS_Imaging::image_request () const
{
  return _image_request;
}

inline ::dpe::Image_Allocator *
DS_Imaging::image_alloc () const
{
  return _image_alloc;
}

inline void
DS_Imaging::send_request ()
{
  _request_sent = true;
  _image_alloc->send_request ( image_request () );
}

inline void
DS_Imaging::wait_for_request ()
{
  if ( _request_sent ) {
    _request_sent = false;
    image_request ()->wait_for_finish ();
  }
}

} // namespace Wdg

#endif
