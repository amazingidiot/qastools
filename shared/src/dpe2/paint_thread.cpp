//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "paint_thread.hpp"
#include "pixmap_server_shared.hpp"
#include <iostream>

namespace dpe2
{

Paint_Thread::Paint_Thread (
	::dpe2::Pixmap_Server_Shared & shared_n ) :
_shared ( shared_n )
{
}

void
Paint_Thread::run ( )
{
	/*
	while ( true ) {
		::dpe::Paint_Job * pjob ( _shared.fetch_job() );
		if ( pjob == 0 ) {
			// Exit loop on invalid job
			break;
		}
		_shared.paint_job ( pjob );
		_shared.job_finished ( pjob );
	}
	*/
}


} // End of namespace

