//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "controls.hpp"
#include <iostream>

namespace QSnd2
{


Controls::Controls ( ) :
_num_pollfds ( 0 )
{
}

Controls::~Controls ( )
{
}

void
Controls::clear_groups ( )
{
	::QSnd2::destroy_list_entries ( _groups );
}

void
Controls::append_group (
	::QSnd2::Proxies_Group4 * grp_n )
{
	_groups.append ( grp_n );
}

void
Controls::clear_pollfds ( )
{
	set_num_pollfds ( 0 );
}

void
Controls::set_num_pollfds (
	unsigned int num_n )
{
	if ( num_n != _num_pollfds ) {
		_num_pollfds = num_n;
		pollfd * pfd_new ( 0 );
		if ( num_n > 0 ) {
			pfd_new = new pollfd[_num_pollfds];
		}
		_pollfds.reset ( pfd_new );
	}
}

void
Controls::socket_data (
	int socket_id_n )
{
	(void) socket_id_n;
	// Default implementation
	::std::cout << "Controls::socket_data " << socket_id_n << "\n";
}


} // End of namespace
