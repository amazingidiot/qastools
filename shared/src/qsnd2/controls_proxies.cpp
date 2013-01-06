//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "controls_proxies.hpp"

namespace QSnd2
{


Proxy_Object::Proxy_Object (
	unsigned int proxy_type_n ) :
_proxy_type ( proxy_type_n )
{
}

Proxy_Object::~Proxy_Object ( )
{
}



Control_Proxy::Control_Proxy (
	unsigned int proxy_type_n,
	::QSnd2::Proxies_Group_Controls * group_n ) :
::QSnd2::Proxy_Object ( proxy_type_n ),
_controls_group ( group_n )
{
}

Control_Proxy::~Control_Proxy ( )
{
}

void
Control_Proxy::set_controls_group (
	::QSnd2::Proxies_Group_Controls * group_n )
{
	_controls_group = group_n;
}

bool
Control_Proxy::string_val (
	QString & str_n,
	unsigned int key_n ) const
{
	// Default implementation
	return false;
}

void
Control_Proxy::set_val_change_callback (
	const ::Context_Callback & callback_n )
{
	_val_change_callback = callback_n;
}

void
Control_Proxy::clear_val_change_callback ( )
{
	_val_change_callback.reset();
}

void
Control_Proxy::call_val_change_callback ( ) const
{
	val_change_callback().call_if_valid();
}



Proxy_Slider::Proxy_Slider ( ) :
::QSnd2::Control_Proxy ( ::QSnd2::PTYPE_CONTROL_SLIDER )
{
}

Proxy_Slider::~Proxy_Slider ( )
{
}



Proxy_Switch::Proxy_Switch ( ) :
::QSnd2::Control_Proxy ( ::QSnd2::PTYPE_CONTROL_SWITCH )
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
::QSnd2::Control_Proxy ( ::QSnd2::PTYPE_CONTROL_ENUM )
{
}

Proxy_Enum::~Proxy_Enum ( )
{
}



Proxies_Group::Proxies_Group (
	unsigned int proxy_type_n ) :
::QSnd2::Proxy_Object ( proxy_type_n )
{
}

Proxies_Group::~Proxies_Group ( )
{
	clear_children();
}

void
Proxies_Group::clear_children ( )
{
	if ( _proxies.size() > 0 ) {
		for ( unsigned int ii=0; ii < _proxies.size(); ++ii ) {
			delete _proxies[ii];
		}
		_proxies.clear();
	}
}

void
Proxies_Group::append_child_proxy (
	::QSnd2::Proxy_Object * proxy_n )
{
	if ( proxy_n != 0 ) {
		_proxies.append ( proxy_n );
	}
}

bool
Proxies_Group::string_val (
	QString & str_n,
	unsigned int key_n ) const
{
	// Default implementation
	return false;
}



Proxies_Group_Controls::Proxies_Group_Controls (
	unsigned int proxy_type_n ) :
::QSnd2::Proxies_Group ( proxy_type_n ),
_feature_flags ( 0 )
{
}

Proxies_Group_Controls::~Proxies_Group_Controls ( )
{
}

void
Proxies_Group_Controls::append_control_proxy (
	::QSnd2::Control_Proxy * proxy_n )
{
	if ( proxy_n != 0 ) {
		proxy_n->set_controls_group ( this );
		append_child_proxy ( proxy_n );
	}
}

void
Proxies_Group_Controls::set_val_change_callback (
	const ::Context_Callback & callback_n )
{
	_val_change_callback = callback_n;
}

void
Proxies_Group_Controls::clear_val_change_callback ( )
{
	_val_change_callback.reset();
}

void
Proxies_Group_Controls::call_val_changed_callbacks ( )
{
	val_change_callback().call_if_valid();
	for ( unsigned int ii=0; ii < num_children(); ++ii ) {
		control_proxy ( ii )->call_val_change_callback();
	}
}



Proxies_Group_Sliders::Proxies_Group_Sliders ( ) :
::QSnd2::Proxies_Group_Controls ( ::QSnd2::PTYPE_CGROUP_SLIDERS )
{
}

Proxies_Group_Sliders::~Proxies_Group_Sliders ( )
{
}

long
Proxies_Group_Sliders::int_value_joined ( )
{
	long res ( 0 );
	if ( num_children() > 0 ) {
		res = slider_proxy ( 0 )->int_value();
	}
	return res;
}

void
Proxies_Group_Sliders::set_int_value_joined (
	long value_n )
{
	for ( unsigned int ii=0; ii < num_children(); ++ii ) {
		slider_proxy ( ii )->set_int_value ( value_n );
	}
}

bool
Proxies_Group_Sliders::values_equal ( ) const
{
	bool res ( false );
	if ( num_children() > 0 ) {
		res = true;
		if ( num_children() > 1 ) {
			const long int_val_test ( slider_proxy ( 0 )->int_value() );
			for ( unsigned int ii=1; ii < num_children(); ++ii ) {
				if ( slider_proxy ( ii )->int_value() != int_val_test ) {
					res = false;
					break;
				}
			}
		}
	}
	return res;
}


Proxies_Group_Switches::Proxies_Group_Switches ( ) :
::QSnd2::Proxies_Group_Controls ( ::QSnd2::PTYPE_CGROUP_SWITCHES )
{
}

Proxies_Group_Switches::~Proxies_Group_Switches ( )
{
}

bool
Proxies_Group_Switches::switches_state_joined ( )
{
	bool res ( false );
	if ( num_children() > 0 ) {
		res = switch_proxy ( 0 )->switch_state();
	}
	return res;
}

void
Proxies_Group_Switches::set_switches_state_joined (
	bool state_n )
{
	for ( unsigned int ii=0; ii < num_children(); ++ii ) {
		switch_proxy ( ii )->set_switch_state ( state_n );
	}
}

void
Proxies_Group_Switches::toggle_switches ( )
{
	for ( unsigned int ii=0; ii < num_children(); ++ii ) {
		switch_proxy ( ii )->toggle_switch_state();
	}
}

bool
Proxies_Group_Switches::values_equal ( ) const
{
	bool res ( false );
	if ( num_children() > 0 ) {
		res = true;
		if ( num_children() > 1 ) {
			const bool state_test ( switch_proxy ( 0 )->switch_state() );
			for ( unsigned int ii=1; ii < num_children(); ++ii ) {
				if ( switch_proxy ( ii )->switch_state() != state_test ) {
					res = false;
					break;
				}
			}
		}
	}
	return res;
}


Proxies_Group_Enums::Proxies_Group_Enums ( ) :
::QSnd2::Proxies_Group_Controls ( ::QSnd2::PTYPE_CGROUP_ENUMS )
{
}

Proxies_Group_Enums::~Proxies_Group_Enums ( )
{
}



Proxies_Group2::Proxies_Group2 ( ) :
::QSnd2::Proxies_Group ( ::QSnd2::PTYPE_GROUP2 )
{
}

Proxies_Group2::~Proxies_Group2 ( )
{
}

void
Proxies_Group2::append_controls_group (
	::QSnd2::Proxies_Group_Controls * group_n )
{
	append_child_proxy ( group_n );
}

::QSnd2::Proxies_Group_Controls *
Proxies_Group2::find_group_type (
	unsigned int proxy_type_n ) const
{
	::QSnd2::Proxies_Group_Controls * res ( 0 );
	for ( unsigned int ii=0; ii < num_children(); ++ii ) {
		::QSnd2::Proxy_Object * item ( child_proxy ( ii ) );
		if ( item->proxy_type() == proxy_type_n ) {
			res = static_cast < ::QSnd2::Proxies_Group_Controls * > ( item );
			break;
		}
	}
	return res;
}

::QSnd2::Proxies_Group_Sliders *
Proxies_Group2::sliders ( ) const
{
	::QSnd2::Proxies_Group_Sliders * res ( 0 );
	{
		::QSnd2::Proxies_Group_Controls * item (
			find_group_type ( ::QSnd2::PTYPE_CGROUP_SLIDERS ) );
		if ( item != 0 ) {
			res = static_cast < ::QSnd2::Proxies_Group_Sliders * > ( item );
		}
	}
	return res;
}

::QSnd2::Proxies_Group_Switches *
Proxies_Group2::switches ( ) const
{
	::QSnd2::Proxies_Group_Switches * res ( 0 );
	{
		::QSnd2::Proxies_Group_Controls * item (
			find_group_type ( ::QSnd2::PTYPE_CGROUP_SWITCHES ) );
		if ( item != 0 ) {
			res = static_cast < ::QSnd2::Proxies_Group_Switches * > ( item );
		}
	}
	return res;
}

::QSnd2::Proxies_Group_Enums *
Proxies_Group2::enums ( ) const
{
	::QSnd2::Proxies_Group_Enums * res ( 0 );
	{
		::QSnd2::Proxies_Group_Controls * item (
			find_group_type ( ::QSnd2::PTYPE_CGROUP_ENUMS ) );
		if ( item != 0 ) {
			res = static_cast < ::QSnd2::Proxies_Group_Enums * > ( item );
		}
	}
	return res;
}

void
Proxies_Group2::call_val_changed_callbacks ( )
{
	for ( unsigned int ii=0; ii < num_children(); ++ii ) {
		controls_group ( ii )->call_val_changed_callbacks();
	}
}


Proxies_Group3::Proxies_Group3 ( ) :
::QSnd2::Proxies_Group ( ::QSnd2::PTYPE_GROUP3 )
{
}

Proxies_Group3::~Proxies_Group3 ( )
{
}

void
Proxies_Group3::append_child_group (
	::QSnd2::Proxies_Group2 * group_n )
{
	append_child_proxy ( group_n );
}

void
Proxies_Group3::call_val_changed_callbacks ( )
{
	for ( unsigned int ii=0; ii < num_children(); ++ii ) {
		child_group ( ii )->call_val_changed_callbacks();
	}
}



Proxies_Group4::Proxies_Group4 ( ) :
::QSnd2::Proxies_Group ( ::QSnd2::PTYPE_GROUP4 )
{
}

Proxies_Group4::~Proxies_Group4 ( )
{
}

void
Proxies_Group4::append_child_group (
	::QSnd2::Proxies_Group3 * group_n )
{
	append_child_proxy ( group_n );
}


} // End of namespace
