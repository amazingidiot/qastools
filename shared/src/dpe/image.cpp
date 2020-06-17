/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "image.hpp"
#include <QPixmap>

namespace dpe
{

Image::Image ()
: _width ( 0 )
, _height ( 0 )
, _stride ( 0 )
{
}

Image::~Image () {}

void
Image::clear ()
{
  _pixmap.reset ();
  _width = 0;
  _height = 0;
  _stride = 0;
}

void
Image::set_size ( unsigned int width_n,
                  unsigned int height_n,
                  unsigned int stride_n )
{
  clear ();
  _width = width_n;
  _height = height_n;
  _stride = stride_n;
  _qimage = QImage ( _width, _height, QImage::Format_ARGB32_Premultiplied );
}

QPixmap *
Image::convert_to_pixmap ()
{
  if ( ( pixmap () == 0 ) || ( !qimage ().isNull () ) ) {
    _pixmap.reset ( new QPixmap ( QPixmap::fromImage ( _qimage ) ) );
    _qimage = QImage ();
  }
  return pixmap ();
}

} // namespace dpe
