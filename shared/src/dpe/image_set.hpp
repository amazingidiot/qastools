/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_dpe_image_set_hpp__
#define __INC_dpe_image_set_hpp__

#include "image.hpp"
#include <vector>

namespace dpe
{

/// @brief Image_Set
///
/// An image set that can be shared by multiple users.
/// Right after creation the user count is always 0.
///
class Image_Set
{
  // Public methods
  public:
  Image_Set ( unsigned int num_images_n );

  virtual ~Image_Set ();

  void
  convert_to_pixmap ();

  QPixmap *
  convert_to_pixmap ( unsigned int idx_n );

  // Number of images

  unsigned int
  num_images () const;

  unsigned int
  byte_count () const;

  const ::dpe::Image &
  image ( unsigned int index_n ) const;

  ::dpe::Image &
  image ( unsigned int index_n );

  QPixmap *
  pixmap ( unsigned int index_n );

  // Private attributes
  private:
  unsigned int _num_images;
  QScopedArrayPointer<::dpe::Image > _images;
};

inline unsigned int
Image_Set::num_images () const
{
  return _num_images;
}

inline const ::dpe::Image &
Image_Set::image ( unsigned int index_n ) const
{
  return _images[ index_n ];
}

inline ::dpe::Image &
Image_Set::image ( unsigned int index_n )
{
  return _images[ index_n ];
}

inline QPixmap *
Image_Set::pixmap ( unsigned int index_n )
{
  return image ( index_n ).pixmap ();
}

inline QPixmap *
Image_Set::convert_to_pixmap ( unsigned int idx_n )
{
  return image ( idx_n ).convert_to_pixmap ();
}

} // namespace dpe

#endif
