//
// C++ Implementation:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "painter_thread.hpp"

#include "painter_thread_shared.hpp"

#include <iostream>


namespace dpe
{


Painter_Thread::Painter_Thread (
	::dpe::Painter_Thread_Shared & shared_n ) :
_shared ( shared_n )
{
}


void
Painter_Thread::run ( )
{
	while ( true ) {
		::dpe::Paint_Job * pjob ( _shared.fetch_job() );

		if ( pjob == 0 ) {
			// Exit loop on invalid job
			break;
		}

		_shared.paint_job ( pjob );
		_shared.job_finished ( pjob );
	}
}


} // End of namespace

