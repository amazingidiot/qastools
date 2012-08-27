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



UValue_String::UValue_String ( )
{
}

UValue_String::UValue_String (
	const QString & str_n ) :
_str ( str_n )
{
}

::dpe2::User_Value *
UValue_String::duplicate ( ) const
{
	::dpe2::UValue_String * res (
		new ::dpe2::UValue_String ( str() ) );
	return res;
}

bool
UValue_String::operator== (
	const ::dpe2::User_Value & val_n )
{
	const ::dpe2::UValue_String * uval (
		dynamic_cast < const ::dpe2::UValue_String * > ( &val_n ) );
	if ( uval != 0 ) {
		return ( str() == uval->str() );
	}
	return false;
}

bool
UValue_String::operator!= (
	const ::dpe2::User_Value & val_n )
{
	const ::dpe2::UValue_String * uval (
		dynamic_cast < const ::dpe2::UValue_String * > ( &val_n ) );
	if ( uval != 0 ) {
		return ( str() != uval->str() );
	}
	return true;
}




UValue_Font::UValue_Font ( )
{
}

UValue_Font::UValue_Font (
	const QFont & font_n ) :
_font ( font_n )
{
}

::dpe2::User_Value *
UValue_Font::duplicate ( ) const
{
	::dpe2::UValue_Font * res (
		new ::dpe2::UValue_Font ( font() ) );
	return res;
}

bool
UValue_Font::operator== (
	const ::dpe2::User_Value & val_n )
{
	const ::dpe2::UValue_Font * uval (
		dynamic_cast < const ::dpe2::UValue_Font * > ( &val_n ) );
	if ( uval != 0 ) {
		return ( font() == uval->font() );
	}
	return false;
}

bool
UValue_Font::operator!= (
	const ::dpe2::User_Value & val_n )
{
	const ::dpe2::UValue_Font * uval (
		dynamic_cast < const ::dpe2::UValue_Font * > ( &val_n ) );
	if ( uval != 0 ) {
		return ( font() != uval->font() );
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

int
Key_Values::val_int (
	unsigned int key_n,
	bool * good_n ) const
{
	const ::dpe2::Value_Item * val ( value_for_key ( key_n ) );
	if ( val != 0 ) {
		if ( val->type() == ::dpe2::Value_Item::TYPE_INT ) {
			return val->as_int();
		}
	}
	if ( good_n != 0 ) {
		*good_n = false;
	}
	return 0;
}

unsigned int
Key_Values::val_uint (
	unsigned int key_n,
	bool * good_n ) const
{
	const ::dpe2::Value_Item * val ( value_for_key ( key_n ) );
	if ( val != 0 ) {
		if ( val->type() == ::dpe2::Value_Item::TYPE_UINT ) {
			return val->as_uint();
		}
	}
	if ( good_n != 0 ) {
		*good_n = false;
	}
	return 0;
}

double
Key_Values::val_double (
	unsigned int key_n,
	bool * good_n ) const
{
	const ::dpe2::Value_Item * val ( value_for_key ( key_n ) );
	if ( val != 0 ) {
		if ( val->type() == ::dpe2::Value_Item::TYPE_DOUBLE ) {
			return val->as_double();
		}
	}
	if ( good_n != 0 ) {
		*good_n = false;
	}
	return 0;
}

::dpe2::User_Value *
Key_Values::val_user (
	unsigned int key_n,
	bool * good_n ) const
{
	const ::dpe2::Value_Item * val ( value_for_key ( key_n ) );
	if ( val != 0 ) {
		if ( val->type() == ::dpe2::Value_Item::TYPE_USER ) {
			return val->as_user();
		}
	}
	if ( good_n != 0 ) {
		*good_n = false;
	}
	return 0;
}

inline
::dpe2::Value_Item *
Key_Values::ensured_value_for_key (
	unsigned int key_n )
{
	::dpe2::Value_Item * vitem ( value_for_key ( key_n ) );
	if ( vitem == 0 ) {
		_entries.push_back ( Entry ( key_n, Value_Item() ) );
		vitem = &_entries.back().second;
	}
	return vitem;
}

void
Key_Values::set_int (
	unsigned int key_n,
	int val_n )
{
	ensured_value_for_key ( key_n )->set_int ( val_n );
}

void
Key_Values::set_uint (
	unsigned int key_n,
	unsigned int val_n )
{
	ensured_value_for_key ( key_n )->set_uint ( val_n );
}

void
Key_Values::set_double (
	unsigned int key_n,
	double val_n )
{
	ensured_value_for_key ( key_n )->set_double ( val_n );
}

void
Key_Values::set_user (
	unsigned int key_n,
	::dpe2::User_Value * val_n )
{
	ensured_value_for_key ( key_n )->set_user ( val_n );
}

void
Key_Values::set_value (
	unsigned int key_n,
	const ::dpe2::Value_Item & val_n )
{
	*ensured_value_for_key ( key_n ) = val_n;
}

void
Key_Values::set_from (
	const ::dpe2::Key_Values & vset_n )
{
	for ( unsigned int ii=0; ii < vset_n.num_entries(); ++ii ) {
		const ::dpe2::Key_Values::Entry & entry ( vset_n.entry ( ii ) );
		set_value ( entry.first, entry.second );
	}
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
