//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_dpe2_values_set_hpp__
#define __INC_dpe2_values_set_hpp__

#include <vector>
#include <QString>

namespace dpe2
{


/// @brief User_Value
///
class User_Value
{
	// Public methods
	public:

	virtual
	~User_Value ( );

	virtual
	::dpe2::User_Value *
	duplicate ( ) const = 0;

	virtual
	bool
	operator== (
		const ::dpe2::User_Value & val_n ) = 0;

	virtual
	bool
	operator!= (
		const ::dpe2::User_Value & val_n );
};


/// @brief User_Value_String
///
class User_Value_String :
	public ::dpe2::User_Value
{
	// Public methods
	public:

	User_Value_String ( );

	User_Value_String (
		const QString & str_n );

	QString &
	str ( );

	const QString &
	str ( ) const;

	::dpe2::User_Value *
	duplicate ( ) const;

	bool
	operator== (
		const ::dpe2::User_Value & val_n );

	bool
	operator!= (
		const ::dpe2::User_Value & val_n );


	// Private attributes
	private:

	QString _str;
};

inline
QString &
User_Value_String::str ( )
{
	return _str;
}

inline
const QString &
User_Value_String::str ( ) const
{
	return _str;
}


/// @brief Value_Item
///
class Value_Item
{
	// Public typedefs
	public:

	enum Types {
		TYPE_INVALID = 0,
		TYPE_INT,
		TYPE_UINT,
		TYPE_DOUBLE,
		TYPE_USER
	};


	// Public methods
	public:

	Value_Item ( );

	Value_Item (
		const ::dpe2::Value_Item & item_n );

	~Value_Item ( );

	void
	reset ( );

	unsigned int
	type ( ) const;

	bool
	is_valid ( ) const;


	int
	as_int ( ) const;

	unsigned int
	as_uint ( ) const;

	double
	as_double ( ) const;

	::dpe2::User_Value *
	as_user ( ) const;


	void
	set_int (
		int val_n );

	void
	set_uint (
		unsigned int val_n );

	void
	set_double (
		double val_n );

	void
	set_user (
		::dpe2::User_Value * val_n );


	::dpe2::Value_Item &
	operator= (
		const ::dpe2::Value_Item & item_n );

	bool
	operator== (
		const ::dpe2::Value_Item & item_n ) const;

	bool
	operator!= (
		const ::dpe2::Value_Item & item_n ) const;


	// Private methods
	private:

	void
	assign (
		const ::dpe2::Value_Item & item_n );

	// Private attributes
	private:

	unsigned int _type;
	union Value_Types_Union {
		int int_val;
		unsigned int uint_val;
		double double_val;
		::dpe2::User_Value * user_val;
	} _value;

};

inline
Value_Item::Value_Item ( ) :
_type ( ::dpe2::Value_Item::TYPE_INVALID )
{
}

inline
unsigned int
Value_Item::type ( ) const
{
	return _type;
}

inline
bool
Value_Item::is_valid ( ) const
{
	return ( _type != ::dpe2::Value_Item::TYPE_INVALID );
}

inline
int
Value_Item::as_int ( ) const
{
	return _value.int_val;
}

inline
unsigned int
Value_Item::as_uint ( ) const
{
	return _value.uint_val;
}

inline
double
Value_Item::as_double ( ) const
{
	return _value.double_val;
}

inline
::dpe2::User_Value *
Value_Item::as_user ( ) const
{
	return _value.user_val;
}

inline
void
Value_Item::set_int (
	int val_n )
{
	reset();
	_type = ::dpe2::Value_Item::TYPE_INT;
	_value.int_val = val_n;
}

inline
void
Value_Item::set_uint (
	unsigned int val_n )
{
	reset();
	_type = ::dpe2::Value_Item::TYPE_UINT;
	_value.uint_val = val_n;
}

inline
void
Value_Item::set_double (
	double val_n )
{
	reset();
	_type = ::dpe2::Value_Item::TYPE_DOUBLE;
	_value.double_val = val_n;
}

inline
void
Value_Item::set_user (
	::dpe2::User_Value * val_n )
{
	reset();
	if ( val_n != 0 ) {
		_type = ::dpe2::Value_Item::TYPE_USER;
		_value.user_val = val_n;
	}
}

inline
::dpe2::Value_Item &
Value_Item::operator= (
	const ::dpe2::Value_Item & item_n )
{
	reset();
	assign ( item_n );
	return *this;
}



/// @brief Values_Set
///
class Values_Set
{
	// Public typedefs
	public:

	typedef ::std::pair < unsigned int, ::dpe2::Value_Item > Entry;


	// Public methods
	public:

	Values_Set ( );

	Values_Set (
		const ::dpe2::Values_Set & vset_n );

	~Values_Set ( );

	void
	clear ( );

	bool
	is_clear ( ) const;

	unsigned int
	num_entries ( ) const;


	void
	append_int (
		unsigned int key_n,
		int val_n );

	void
	append_uint (
		unsigned int key_n,
		unsigned int val_n );

	void
	append_double (
		unsigned int key_n,
		double val_n );

	void
	append_user (
		unsigned int key_n,
		::dpe2::User_Value * val_n );


	::dpe2::Value_Item *
	value_for_key (
		unsigned int key_n );

	const ::dpe2::Value_Item *
	value_for_key (
		unsigned int key_n ) const;


	bool
	value_int (
		int & val_n,
		unsigned int key_n ) const;

	bool
	value_uint (
		unsigned int & val_n,
		unsigned int key_n ) const;

	bool
	value_double (
		double & val_n,
		unsigned int key_n ) const;

	bool
	value_user (
		::dpe2::User_Value * * val_n,
		unsigned int key_n ) const;


	::dpe2::Values_Set::Entry &
	operator[] (
		unsigned int idx_n );

	const ::dpe2::Values_Set::Entry &
	operator[] (
		unsigned int idx_n ) const;


	::dpe2::Values_Set &
	operator= (
		const ::dpe2::Values_Set & vset_n );


	bool
	operator== (
		const ::dpe2::Values_Set & vset_n ) const;

	bool
	operator!= (
		const ::dpe2::Values_Set & vset_n ) const;


	// Private methods
	private:

	void
	assign (
		const ::dpe2::Values_Set & vset_n );


	// Private attributes
	private:

	::std::vector < ::dpe2::Values_Set::Entry > _entries;
};

inline
bool
Values_Set::is_clear ( ) const
{
	return ( num_entries() == 0 );
}

inline
unsigned int
Values_Set::num_entries ( ) const
{
	return _entries.size();
}

inline
::dpe2::Values_Set::Entry &
Values_Set::operator[] (
	unsigned int idx_n )
{
	return _entries[idx_n];
}

inline
const ::dpe2::Values_Set::Entry &
Values_Set::operator[] (
	unsigned int idx_n ) const
{
	return _entries[idx_n];
}

inline
::dpe2::Values_Set &
Values_Set::operator= (
	const ::dpe2::Values_Set & vset_n )
{
	clear();
	assign ( vset_n );
	return *this;
}


} // End of namespace

#endif
