//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "proxies.hpp"

namespace QSnd2
{


Proxy::Proxy (
	::QSnd2::Proxies_Group * group_n ) :
_group ( group_n )
{
}

Proxy::~Proxy ( )
{
}

void
Proxy::set_group (
	::QSnd2::Proxies_Group * group_n )
{
	_group = group_n;
}


Proxies_Group::Proxies_Group ( )
{
}

Proxies_Group::~Proxies_Group ( )
{
	clear();
}

void
Proxies_Group::clear ( )
{
	for ( unsigned int ii=0; ii < num_proxies(); ++ii ) {
		delete _proxies[ii];
	}
	_proxies.clear();
}


Slider_Proxy::Slider_Proxy ( )
{
}

Slider_Proxy::~Slider_Proxy ( )
{
}


Slider_Proxies_Group::Slider_Proxies_Group ( )
{
}

Slider_Proxies_Group::~Slider_Proxies_Group ( )
{
}


Switch_Proxy::Switch_Proxy ( )
{
}

Switch_Proxy::~Switch_Proxy ( )
{
}


Switch_Proxies_Group::Switch_Proxies_Group ( )
{
}

Switch_Proxies_Group::~Switch_Proxies_Group ( )
{
}


List_Proxy::List_Proxy ( )
{
}

List_Proxy::~List_Proxy ( )
{
}


List_Proxies_Group::List_Proxies_Group ( )
{
}

List_Proxies_Group::~List_Proxies_Group ( )
{
}


Group1::Group1 ( ) :
_sliders ( 0 ),
_switches ( 0 ),
_lists ( 0 )
{
}

Group1::~Group1 ( )
{
}

void
Group1::set_sliders (
	::QSnd2::Slider_Proxies_Group * grp_n )
{
	_sliders.reset ( grp_n );
}

void
Group1::set_switches (
	::QSnd2::Switch_Proxies_Group * grp_n )
{
	_switches.reset ( grp_n );
}

void
Group1::set_lists (
	::QSnd2::List_Proxies_Group * grp_n )
{
	_lists.reset ( grp_n );
}


Group2::Group2 ( )
{
}

Group2::~Group2 ( )
{
}


} // End of namespace
