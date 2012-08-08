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
#include <QFont>

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


/// @brief UValue_String
///
class UValue_String :
	public ::dpe2::User_Value
{
	// Public methods
	public:

	UValue_String ( );

	UValue_String (
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
UValue_String::str ( )
{
	return _str;
}

inline
const QString &
UValue_String::str ( ) const
{
	return _str;
}


/// @brief UValue_Font
///
class UValue_Font :
	public ::dpe2::User_Value
{
	// Public methods
	public:

	UValue_Font ( );

	UValue_Font (
		const QFont & font_n );

	QFont &
	font ( );

	const QFont &
	font ( ) const;

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

	QFont _font;
};

inline
QFont &
UValue_Font::font ( )
{
	return _font;
}

inline
const QFont &
UValue_Font::font ( ) const
{
	return _font;
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



/// @brief Key_Values
///
class Key_Values
{
	// Public typedefs
	public:

	typedef ::std::pair < unsigned int, ::dpe2::Value_Item > Entry;


	// Public methods
	public:

	Key_Values ( );

	Key_Values (
		const ::dpe2::Key_Values & vset_n );

	~Key_Values ( );

	void
	clear ( );

	bool
	is_clear ( ) const;

	unsigned int
	num_entries ( ) const;


	::dpe2::Value_Item *
	value_for_key (
		unsigned int key_n );

	const ::dpe2::Value_Item *
	value_for_key (
		unsigned int key_n ) const;


	int
	val_int (
		unsigned int key_n,
		bool * good_n = 0 ) const;

	unsigned int
	val_uint (
		unsigned int key_n,
		bool * good_n = 0 ) const;

	double
	val_double (
		unsigned int key_n,
		bool * good_n = 0 ) const;

	::dpe2::User_Value *
	val_user (
		unsigned int key_n,
		bool * good_n = 0 ) const;



	void
	set_int (
		unsigned int key_n,
		int val_n );

	void
	set_uint (
		unsigned int key_n,
		unsigned int val_n );

	void
	set_double (
		unsigned int key_n,
		double val_n );

	void
	set_user (
		unsigned int key_n,
		::dpe2::User_Value * val_n );


	::dpe2::Key_Values::Entry &
	operator[] (
		unsigned int idx_n );

	const ::dpe2::Key_Values::Entry &
	operator[] (
		unsigned int idx_n ) const;


	::dpe2::Key_Values &
	operator= (
		const ::dpe2::Key_Values & vset_n );


	bool
	operator== (
		const ::dpe2::Key_Values & vset_n ) const;

	bool
	operator!= (
		const ::dpe2::Key_Values & vset_n ) const;


	// Private methods
	private:

	void
	assign (
		const ::dpe2::Key_Values & vset_n );


	// Private attributes
	private:

	::std::vector < ::dpe2::Key_Values::Entry > _entries;
};

inline
bool
Key_Values::is_clear ( ) const
{
	return ( num_entries() == 0 );
}

inline
unsigned int
Key_Values::num_entries ( ) const
{
	return _entries.size();
}

inline
::dpe2::Key_Values::Entry &
Key_Values::operator[] (
	unsigned int idx_n )
{
	return _entries[idx_n];
}

inline
const ::dpe2::Key_Values::Entry &
Key_Values::operator[] (
	unsigned int idx_n ) const
{
	return _entries[idx_n];
}

inline
::dpe2::Key_Values &
Key_Values::operator= (
	const ::dpe2::Key_Values & vset_n )
{
	clear();
	assign ( vset_n );
	return *this;
}


} // End of namespace

#endif
