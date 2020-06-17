/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "painter_thread.hpp"
#include "painter_thread_shared.hpp"
#include <iostream>

namespace dpe
{

Painter_Thread::Painter_Thread ( ::dpe::Painter_Thread_Shared & shared_n )
: _shared ( shared_n )
{
}

void
Painter_Thread::run ()
{
  while ( true ) {
    ::dpe::Paint_Job * pjob ( _shared.fetch_job () );

    if ( pjob == 0 ) {
      // Exit loop on invalid job
      break;
    }

    _shared.paint_job ( pjob );
    _shared.job_finished ( pjob );
  }
}

} // namespace dpe
