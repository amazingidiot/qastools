/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_dpe_painter_simple_hpp__
#define __INC_dpe_painter_simple_hpp__

#include "painter.hpp"

namespace dpe
{

/// @brief Painter_Simple
///
class Painter_Simple : public ::dpe::Painter
{
  // Public methods
  public:
  Painter_Simple ();

  ~Painter_Simple ();

  int
  paint_image ( ::dpe::Paint_Job * pjob_n );

  // Private attributes
  private:
};

} // namespace dpe

#endif
