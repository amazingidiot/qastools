//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "controls_proxies.hpp"

namespace QSnd2
{


Proxy::Proxy (
	unsigned int control_type_n,
	::QSnd2::Proxies_Group1 * group_n ) :
_control_type ( control_type_n ),
_pgroup ( group_n )
{
}

Proxy::~Proxy ( )
{
}

bool
Proxy::string_val (
	QString & str_n,
	unsigned int key_n ) const
{
	// Default implementation
	return false;
}

void
Proxy::set_pgroup (
	::QSnd2::Proxies_Group1 * group_n )
{
	_pgroup = group_n;
}

void
Proxy::set_val_change_callback (
	const ::Context_Callback & callback_n )
{
	_val_change_callback = callback_n;
}

void
Proxy::reset_val_change_callback ( )
{
	_val_change_callback.reset();
}

void
Proxy::notify_value_changed ( ) const
{
	val_change_callback().call_if_valid();
}



Proxy_Slider::Proxy_Slider ( ) :
::QSnd2::Proxy ( ::QSnd2::ETYPE_SLIDER )
{
}

Proxy_Slider::~Proxy_Slider ( )
{
}



Proxy_Switch::Proxy_Switch ( ) :
::QSnd2::Proxy ( ::QSnd2::ETYPE_SWITCH )
{
}

Proxy_Switch::~Proxy_Switch ( )
{
}

void
Proxy_Switch::toggle_switch_state ( )
{
	this->set_switch_state ( !this->switch_state() );
}



Proxy_Enum::Proxy_Enum ( ) :
::QSnd2::Proxy ( ::QSnd2::ETYPE_ENUM )
{
}

Proxy_Enum::~Proxy_Enum ( )
{
}



Proxies_Group::Proxies_Group (
	unsigned int level_n ) :
_group_level ( level_n )
{
}

Proxies_Group::~Proxies_Group ( )
{
}

bool
Proxies_Group::string_val (
	QString & str_n,
	unsigned int key_n ) const
{
	// Default implementation
	return false;
}



Proxies_Group1::Proxies_Group1 (
	unsigned int control_type_n ) :
::QSnd2::Proxies_Group ( 1 ),
_control_type ( control_type_n ),
_feature_flags ( 0 )
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

void
Proxies_Group1::append_proxy (
	::QSnd2::Proxy * proxy_n )
{
	proxy_n->set_pgroup ( this );
	_proxies.append ( proxy_n );
}

void
Proxies_Group1::set_val_change_callback (
	const ::Context_Callback & callback_n )
{
	_val_change_callback = callback_n;
}

void
Proxies_Group1::reset_val_change_callback ( )
{
	_val_change_callback.reset();
}

void
Proxies_Group1::notify_proxies_value_changed ( )
{
	val_change_callback().call_if_valid();
	for ( unsigned int ii=0; ii < num_proxies(); ++ii ) {
		proxy ( ii )->notify_value_changed();
	}
}



Proxies_Group1_Slider::Proxies_Group1_Slider ( ) :
::QSnd2::Proxies_Group1 ( ::QSnd2::ETYPE_SLIDER )
{
}

Proxies_Group1_Slider::~Proxies_Group1_Slider ( )
{
}

long
Proxies_Group1_Slider::int_value_joined ( )
{
	long res ( 0 );
	if ( num_proxies() > 0 ) {
		res = slider_proxy ( 0 )->int_value();
	}
	return res;
}

void
Proxies_Group1_Slider::set_int_value_joined (
	long value_n )
{
	for ( unsigned int ii=0; ii < num_proxies(); ++ii ) {
		slider_proxy ( ii )->set_int_value ( value_n );
	}
}

bool
Proxies_Group1_Slider::values_equal ( ) const
{
	bool res ( false );
	if ( num_proxies() > 0 ) {
		res = true;
		if ( num_proxies() > 1 ) {
			const long int_val_test ( slider_proxy ( 0 )->int_value() );
			for ( unsigned int ii=1; ii < num_proxies(); ++ii ) {
				if ( slider_proxy ( ii )->int_value() != int_val_test ) {
					res = false;
					break;
				}
			}
		}
	}
	return res;
}


Proxies_Group1_Switch::Proxies_Group1_Switch ( ) :
::QSnd2::Proxies_Group1 ( ::QSnd2::ETYPE_SWITCH )
{
}

Proxies_Group1_Switch::~Proxies_Group1_Switch ( )
{
}

bool
Proxies_Group1_Switch::switches_state_joined ( )
{
	bool res ( false );
	if ( num_proxies() > 0 ) {
		res = switch_proxy ( 0 )->switch_state();
	}
	return res;
}

void
Proxies_Group1_Switch::set_switches_state_joined (
	bool state_n )
{
	for ( unsigned int ii=0; ii < num_proxies(); ++ii ) {
		switch_proxy ( ii )->set_switch_state ( state_n );
	}
}

void
Proxies_Group1_Switch::toggle_switches ( )
{
	for ( unsigned int ii=0; ii < num_proxies(); ++ii ) {
		switch_proxy ( ii )->toggle_switch_state();
	}
}

bool
Proxies_Group1_Switch::values_equal ( ) const
{
	bool res ( false );
	if ( num_proxies() > 0 ) {
		res = true;
		if ( num_proxies() > 1 ) {
			const bool state_test ( switch_proxy ( 0 )->switch_state() );
			for ( unsigned int ii=1; ii < num_proxies(); ++ii ) {
				if ( switch_proxy ( ii )->switch_state() != state_test ) {
					res = false;
					break;
				}
			}
		}
	}
	return res;
}


Proxies_Group1_Enum::Proxies_Group1_Enum ( ) :
::QSnd2::Proxies_Group1 ( ::QSnd2::ETYPE_ENUM )
{
}

Proxies_Group1_Enum::~Proxies_Group1_Enum ( )
{
}



Proxies_Group2::Proxies_Group2 ( ) :
::QSnd2::Proxies_Group ( 2 )
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

void
Proxies_Group2::notify_proxies_value_changed ( )
{
	for ( int ii=0; ii < _groups.size(); ++ii ) {
		_groups[ii]->notify_proxies_value_changed();
	}
}


Proxies_Group3::Proxies_Group3 ( ) :
::QSnd2::Proxies_Group ( 3 )
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

void
Proxies_Group3::notify_proxies_value_changed ( )
{
	for ( int ii=0; ii < _groups.size(); ++ii ) {
		_groups[ii]->notify_proxies_value_changed();
	}
}



Proxies_Group4::Proxies_Group4 ( ) :
::QSnd2::Proxies_Group ( 4 )
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
