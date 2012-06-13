//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "key_values.hpp"


namespace dpe2
{



User_Value::~User_Value ( )
{
}

bool
User_Value::operator!= (
	const ::dpe2::User_Value & val_n )
{
	// Default implementation
	return !(this->operator== ( val_n ));
}



User_Value_String::User_Value_String ( )
{
}

User_Value_String::User_Value_String (
	const QString & str_n ) :
_str ( str_n )
{
}

::dpe2::User_Value *
User_Value_String::duplicate ( ) const
{
	::dpe2::User_Value_String * res (
		new ::dpe2::User_Value_String ( str() ) );
	return res;
}

bool
User_Value_String::operator== (
	const ::dpe2::User_Value & val_n )
{
	const ::dpe2::User_Value_String * uval (
		dynamic_cast < const ::dpe2::User_Value_String * > ( &val_n ) );
	if ( uval != 0 ) {
		return ( str() == uval->str() );
	}
	return false;
}

bool
User_Value_String::operator!= (
	const ::dpe2::User_Value & val_n )
{
	const ::dpe2::User_Value_String * uval (
		dynamic_cast < const ::dpe2::User_Value_String * > ( &val_n ) );
	if ( uval != 0 ) {
		return ( str() != uval->str() );
	}
	return true;
}



Value_Item::~Value_Item ( )
{
	reset();
}

Value_Item::Value_Item (
	const ::dpe2::Value_Item & item_n )
{
	assign ( item_n );
}

void
Value_Item::reset ( )
{
	if ( type() != ::dpe2::Value_Item::TYPE_INVALID ) {
		if ( type() == ::dpe2::Value_Item::TYPE_USER ) {
			delete _value.user_val;
		}
		_type = ::dpe2::Value_Item::TYPE_INVALID;
	}
}

void
Value_Item::assign (
	const ::dpe2::Value_Item & item_n )
{
	_type = item_n.type();
	if ( type() != ::dpe2::Value_Item::TYPE_INVALID ) {
		switch ( type() ) {
			case ::dpe2::Value_Item::TYPE_INT:
				_value.int_val = item_n.as_int();
				break;
			case ::dpe2::Value_Item::TYPE_UINT:
				_value.uint_val = item_n.as_uint();
				break;
			case ::dpe2::Value_Item::TYPE_DOUBLE:
				_value.uint_val = item_n.as_double();
				break;
			case ::dpe2::Value_Item::TYPE_USER:
				_value.user_val = item_n.as_user()->duplicate();
				break;
			default:
				break;
		}
	};
}

bool
Value_Item::operator== (
	const ::dpe2::Value_Item & item_n ) const
{
	bool res ( false );
	if ( item_n.type() == type() ) {
		switch ( type() ) {
			case ::dpe2::Value_Item::TYPE_INT:
				res = ( as_int() == item_n.as_int() );
				break;
			case ::dpe2::Value_Item::TYPE_UINT:
				res = ( as_uint() == item_n.as_uint() );
				break;
			case ::dpe2::Value_Item::TYPE_DOUBLE:
				res = ( as_double() == item_n.as_double() );
				break;
			case ::dpe2::Value_Item::TYPE_USER:
				res = ( *as_user() == *item_n.as_user() );
				break;
			default:
				break;
		}
	};
	return res;
}

bool
Value_Item::operator!= (
	const ::dpe2::Value_Item & item_n ) const
{
	bool res ( true );
	if ( item_n.type() == type() ) {
		switch ( type() ) {
			case ::dpe2::Value_Item::TYPE_INT:
				res = ( as_int() != item_n.as_int() );
				break;
			case ::dpe2::Value_Item::TYPE_UINT:
				res = ( as_uint() != item_n.as_uint() );
				break;
			case ::dpe2::Value_Item::TYPE_DOUBLE:
				res = ( as_double() != item_n.as_double() );
				break;
			case ::dpe2::Value_Item::TYPE_USER:
				res = ( *as_user() != *item_n.as_user() );
				break;
			default:
				break;
		}
	};
	return res;
}



Key_Values::Key_Values ( )
{

}

Key_Values::Key_Values (
	const ::dpe2::Key_Values & vset_n )
{
	assign ( vset_n );
}

Key_Values::~Key_Values ( )
{
	clear();
}

void
Key_Values::clear ( )
{
	_entries.clear();
}

void
Key_Values::append_int (
	unsigned int key_n,
	int val_n )
{
	_entries.push_back ( Entry ( key_n, Value_Item() ) );
	_entries.back().second.set_int ( val_n );
}

void
Key_Values::append_uint (
	unsigned int key_n,
	unsigned int val_n )
{
	_entries.push_back ( Entry ( key_n, Value_Item() ) );
	_entries.back().second.set_uint ( val_n );
}

void
Key_Values::append_double (
	unsigned int key_n,
	double val_n )
{
	_entries.push_back ( Entry ( key_n, Value_Item() ) );
	_entries.back().second.set_double ( val_n );
}

void
Key_Values::append_user (
	unsigned int key_n,
	::dpe2::User_Value * val_n )
{
	_entries.push_back ( Entry ( key_n, Value_Item() ) );
	_entries.back().second.set_user ( val_n );
}

::dpe2::Value_Item *
Key_Values::value_for_key (
	unsigned int key_n )
{
	for ( unsigned int ii=0; ii < _entries.size(); ++ii ) {
		Entry & entry ( _entries[ii] );
		if ( entry.first == key_n ) {
			return &entry.second;
		}
	}
	return 0;
}

const ::dpe2::Value_Item *
Key_Values::value_for_key (
	unsigned int key_n ) const
{
	for ( unsigned int ii=0; ii < _entries.size(); ++ii ) {
		const Entry & entry ( _entries[ii] );
		if ( entry.first == key_n ) {
			return &entry.second;
		}
	}
	return 0;
}

bool
Key_Values::value_int (
	int & val_n,
	unsigned int key_n ) const
{
	const ::dpe2::Value_Item * val (
		value_for_key ( key_n ) );
	if ( val != 0 ) {
		if ( val->type() == ::dpe2::Value_Item::TYPE_INT ) {
			val_n = val->as_int();
		}
	}
	return false;
}

bool
Key_Values::value_uint (
	unsigned int & val_n,
	unsigned int key_n ) const
{
	const ::dpe2::Value_Item * val (
		value_for_key ( key_n ) );
	if ( val != 0 ) {
		if ( val->type() == ::dpe2::Value_Item::TYPE_UINT ) {
			val_n = val->as_uint();
		}
	}
	return false;
}

bool
Key_Values::value_double (
	double & val_n,
	unsigned int key_n ) const
{
	const ::dpe2::Value_Item * val (
		value_for_key ( key_n ) );
	if ( val != 0 ) {
		if ( val->type() == ::dpe2::Value_Item::TYPE_DOUBLE ) {
			val_n = val->as_double();
		}
	}
	return false;
}

bool
Key_Values::value_user (
	::dpe2::User_Value * * val_n,
	unsigned int key_n ) const
{
	const ::dpe2::Value_Item * val (
		value_for_key ( key_n ) );
	if ( val != 0 ) {
		if ( val->type() == ::dpe2::Value_Item::TYPE_USER ) {
			*val_n = val->as_user();
		}
	}
	return false;
}

void
Key_Values::assign (
	const ::dpe2::Key_Values & vset_n )
{
	_entries.resize ( vset_n.num_entries() );
	for ( unsigned int ii=0; ii < num_entries(); ++ii ) {
		_entries[ii] = vset_n[ii];
	}
}

bool
Key_Values::operator== (
	const ::dpe2::Key_Values & vset_n ) const
{
	for ( unsigned int ii=0; ii < num_entries(); ++ii ) {
		if ( _entries[ii] != vset_n[ii] ) {
			return false;
		}
	}
	return true;
}

bool
Key_Values::operator!= (
	const ::dpe2::Key_Values & vset_n ) const
{
	for ( unsigned int ii=0; ii < num_entries(); ++ii ) {
		if ( _entries[ii] != vset_n[ii] ) {
			return true;
		}
	}
	return false;
}


} // End of namespace
