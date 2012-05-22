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
	::QSnd2::Proxies_Group0 * group_n ) :
_group ( group_n )
{
}

Proxy::~Proxy ( )
{
}

void
Proxy::set_group (
	::QSnd2::Proxies_Group0 * group_n )
{
	_group = group_n;
}


Proxies_Group0::Proxies_Group0 ( )
{
}

Proxies_Group0::~Proxies_Group0 ( )
{
	clear();
}

void
Proxies_Group0::clear ( )
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


Enum_Proxy::Enum_Proxy ( )
{
}

Enum_Proxy::~Enum_Proxy ( )
{
}


Enum_Proxies_Group::Enum_Proxies_Group ( )
{
}

Enum_Proxies_Group::~Enum_Proxies_Group ( )
{
}


Proxies_Group1::Proxies_Group1 ( ) :
_sliders ( 0 ),
_switches ( 0 ),
_enums ( 0 )
{
}

Proxies_Group1::~Proxies_Group1 ( )
{
}

void
Proxies_Group1::clear_groups ( )
{
	_sliders.reset();
	_switches.reset();
	_enums.reset();
}

void
Proxies_Group1::set_sliders (
	::QSnd2::Slider_Proxies_Group * grp_n )
{
	_sliders.reset ( grp_n );
}

void
Proxies_Group1::set_switches (
	::QSnd2::Switch_Proxies_Group * grp_n )
{
	_switches.reset ( grp_n );
}

void
Proxies_Group1::set_enums (
	::QSnd2::Enum_Proxies_Group * grp_n )
{
	_enums.reset ( grp_n );
}


Proxies_Group2::Proxies_Group2 ( )
{
}

Proxies_Group2::~Proxies_Group2 ( )
{
}

void
Proxies_Group2::clear_groups ( )
{
	_groups.clear();
}

void
Proxies_Group2::append_group (
	::QSnd2::Proxies_Group1 * grp_n )
{
	_groups.append ( grp_n );
}


} // End of namespace
