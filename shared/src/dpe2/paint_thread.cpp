//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
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
	while ( true ) {
		::dpe2::Pixmap_Request * request ( _shared.fetch_request() );
		if ( request == 0 ) {
			// Exit loop on NULL job
			break;
		}
		_shared.process_request ( request );
	}
}


} // End of namespace

