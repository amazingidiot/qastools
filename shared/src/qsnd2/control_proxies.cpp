//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "control_proxies.hpp"

namespace QSnd2
{



Proxy_Object::Proxy_Object (
	unsigned int level_n ) :
_group_level ( level_n )
{
}

Proxy_Object::~Proxy_Object ( )
{
}



Proxy::Proxy (
	unsigned int control_type_n,
	::QSnd2::Proxies_Group1 * group_n ) :
::QSnd2::Proxy_Object ( 0 ),
_control_type ( control_type_n ),
_pgroup ( group_n )
{
}

Proxy::~Proxy ( )
{
}

void
Proxy::set_pgroup (
	::QSnd2::Proxies_Group1 * group_n )
{
	_pgroup = group_n;
}



Proxies_Group1::Proxies_Group1 (
	unsigned int control_type_n ) :
::QSnd2::Proxy_Object ( 1 ),
_control_type ( control_type_n )
{
}

Proxies_Group1::~Proxies_Group1 ( )
{
	clear_children();
}

void
Proxies_Group1::clear_children ( )
{
	::QSnd2::destroy_list_entries ( _proxies );
}



Slider_Proxy::Slider_Proxy ( ) :
::QSnd2::Proxy ( ::QSnd2::ETYPE_SLIDER )
{
}

Slider_Proxy::~Slider_Proxy ( )
{
}



Proxies_Group1_Slider::Proxies_Group1_Slider ( ) :
::QSnd2::Proxies_Group1 ( ::QSnd2::ETYPE_SLIDER )
{
}

Proxies_Group1_Slider::~Proxies_Group1_Slider ( )
{
}



Switch_Proxy::Switch_Proxy ( ) :
::QSnd2::Proxy ( ::QSnd2::ETYPE_SWITCH )
{
}

Switch_Proxy::~Switch_Proxy ( )
{
}



Proxies_Group1_Switch::Proxies_Group1_Switch ( ) :
::QSnd2::Proxies_Group1 ( ::QSnd2::ETYPE_SWITCH )
{
}

Proxies_Group1_Switch::~Proxies_Group1_Switch ( )
{
}



Enum_Proxy::Enum_Proxy ( ) :
::QSnd2::Proxy ( ::QSnd2::ETYPE_ENUM )
{
}

Enum_Proxy::~Enum_Proxy ( )
{
}



Proxies_Group1_Enum::Proxies_Group1_Enum ( ) :
::QSnd2::Proxies_Group1 ( ::QSnd2::ETYPE_ENUM )
{
}

Proxies_Group1_Enum::~Proxies_Group1_Enum ( )
{
}



Proxies_Group2::Proxies_Group2 ( ) :
::QSnd2::Proxy_Object ( 2 )
{
}

Proxies_Group2::~Proxies_Group2 ( )
{
	clear_children();
}

void
Proxies_Group2::clear_children ( )
{
	::QSnd2::destroy_list_entries ( _groups );
}

void
Proxies_Group2::append_group (
	::QSnd2::Proxies_Group1 * grp_n )
{
	_groups.append ( grp_n );
}

::QSnd2::Proxies_Group1_Slider *
Proxies_Group2::sliders ( ) const
{
	::QSnd2::Proxies_Group1_Slider * res ( 0 );
	::QSnd2::Proxies_Group1 * item ( find_group_type ( ::QSnd2::ETYPE_SLIDER ) );
	if ( item != 0 ) {
		res = static_cast < ::QSnd2::Proxies_Group1_Slider * > ( item );
	}
	return res;
}

::QSnd2::Proxies_Group1_Switch *
Proxies_Group2::switches ( ) const
{
	::QSnd2::Proxies_Group1_Switch * res ( 0 );
	::QSnd2::Proxies_Group1 * item ( find_group_type ( ::QSnd2::ETYPE_SWITCH ) );
	if ( item != 0 ) {
		res = static_cast < ::QSnd2::Proxies_Group1_Switch * > ( item );
	}
	return res;
}

::QSnd2::Proxies_Group1_Enum *
Proxies_Group2::enums ( ) const
{
	::QSnd2::Proxies_Group1_Enum * res ( 0 );
	::QSnd2::Proxies_Group1 * item ( find_group_type ( ::QSnd2::ETYPE_ENUM ) );
	if ( item != 0 ) {
		res = static_cast < ::QSnd2::Proxies_Group1_Enum * > ( item );
	}
	return res;
}

::QSnd2::Proxies_Group1 *
Proxies_Group2::find_group_type (
	unsigned int elem_type_n ) const
{
	::QSnd2::Proxies_Group1 * res ( 0 );
	for ( int ii=0; ii < _groups.size(); ++ii ) {
		::QSnd2::Proxies_Group1 * item ( _groups[ii] );
		if ( item->control_type() == elem_type_n ) {
			res = item;
			break;
		}
	}
	return res;
}



Proxies_Group3::Proxies_Group3 ( ) :
::QSnd2::Proxy_Object ( 3 )
{
}

Proxies_Group3::~Proxies_Group3 ( )
{
	clear_children();
}

void
Proxies_Group3::clear_children ( )
{
	::QSnd2::destroy_list_entries ( _groups );
}

void
Proxies_Group3::append_group (
	::QSnd2::Proxies_Group2 * grp_n )
{
	_groups.append ( grp_n );
}



Proxies_Group4::Proxies_Group4 ( ) :
::QSnd2::Proxy_Object ( 4 )
{
}

Proxies_Group4::~Proxies_Group4 ( )
{
	clear_children();
}

void
Proxies_Group4::clear_children ( )
{
	::QSnd2::destroy_list_entries ( _groups );
}

void
Proxies_Group4::append_group (
	::QSnd2::Proxies_Group3 * grp_n )
{
	_groups.append ( grp_n );
}


} // End of namespace
