/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "painter.hpp"
#include "image_set_meta.hpp"
#include "paint_job.hpp"

namespace dpe
{

Painter::Painter ( unsigned int group_type_n, unsigned int group_variant_n )
: _group_type ( group_type_n )
, _group_variant ( group_variant_n )
{
}

Painter::~Painter () {}

void
Painter::set_group_variant ( unsigned int variant_n )
{
  _group_variant = variant_n;
}

int
Painter::check_and_paint ( ::dpe::Paint_Job * pjob_n )
{
  int res ( -1 );
  if ( pjob_n->meta == 0 ) {
    return res;
  }
  {
    const QSize & isize ( pjob_n->meta->size );
    if ( ( isize.width () <= 0 ) || ( isize.height () <= 0 ) ) {
      return res;
    }
  }

  res = this->paint_image ( pjob_n );
  return res;
}

} // namespace dpe
