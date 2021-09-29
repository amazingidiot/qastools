/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "painter_simple.hpp"
#include "image_set.hpp"
#include "image_set_meta.hpp"
#include "paint_job.hpp"
#include <QColor>
#include <QImage>
#include <QRandomGenerator>

namespace dpe
{

Painter_Simple::Painter_Simple ()
: ::dpe::Painter ( 0 )
{
}

Painter_Simple::~Painter_Simple () {}

int
Painter_Simple::paint_image ( ::dpe::Paint_Job * pjob_n )
{
  int res ( 0 );

  ::dpe::Image & img ( pjob_n->img_set->image ( pjob_n->img_idx ) );
  ::dpe::Image_Set_Meta * meta ( pjob_n->meta );

  img.set_size (
      meta->size.width (), meta->size.height (), meta->size.width () * 4 );

  {
    QColor col;
    {
        col.setRgb(QRandomGenerator::system()->bounded(256), QRandomGenerator::system()->bounded(256), QRandomGenerator::system()->bounded(256), 128);
    }
    img.qimage ().fill ( col.rgba () );
  }

  return res;
}

} // namespace dpe
