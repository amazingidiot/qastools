/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_dpe_image_hpp__
#define __INC_dpe_image_hpp__

#include <QPixmap>
#include <QScopedPointer>

namespace dpe
{

/// @brief Image
///
/// An image set that can be shared by multiple users.
/// Right after creation the user count is always 0.
///
class Image
{
  // Public methods
  public:
  Image ();

  ~Image ();

  void
  clear ();

  void
  set_size ( unsigned int width_n,
             unsigned int height_n,
             unsigned int stride_n );

  unsigned int
  width () const;

  unsigned int
  height () const;

  unsigned int
  stride () const;

  unsigned int
  byte_count () const;

  QPixmap *
  convert_to_pixmap ();

  QPixmap *
  pixmap () const;

  QImage &
  qimage ();

  // Private attributes
  private:
  QScopedPointer< QPixmap > _pixmap;
  QImage _qimage;

  unsigned int _width;
  unsigned int _height;
  unsigned int _stride;
};

inline unsigned int
Image::width () const
{
  return _width;
}

inline unsigned int
Image::height () const
{
  return _height;
}

inline unsigned int
Image::stride () const
{
  return _stride;
}

inline unsigned int
Image::byte_count () const
{
  return height () * stride ();
}

inline QPixmap *
Image::pixmap () const
{
  return _pixmap.data ();
}

inline QImage &
Image::qimage ()
{
  return _qimage;
}

} // namespace dpe

#endif
