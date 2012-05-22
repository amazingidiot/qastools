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


Proxy::Proxy ( )
{
}

Proxy::~Proxy ( )
{
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


} // End of namespace
