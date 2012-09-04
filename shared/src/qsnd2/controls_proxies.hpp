//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_qsnd_control_proxies_hpp__
#define __INC_qsnd_control_proxies_hpp__

#include "callbacks.hpp"
#include "flags.hpp"
#include <QList>
#include <QString>

namespace QSnd2
{


// Forward declaration
class Proxies_Group1;
class Proxies_Group1_Slider;
class Proxies_Group1_Switch;
class Proxies_Group1_Enum;
class Proxies_Group2;
class Proxies_Group3;
class Proxies_Group4;

enum Control_Elem_Type {
	ETYPE_NONE,
	ETYPE_SWITCH,
	ETYPE_SLIDER,
	ETYPE_ENUM
};

enum Control_Feature_Flag {
	FFLAG_PLAYBACK = ( 1 << 0 ),
	FFLAG_CAPTURE  = ( 1 << 1 ),
	FFLAG_DECIBEL  = ( 1 << 2 )
};

enum String_Key {
	SK_NAME,
	SK_NAME_L10N,
	SK_DISPLAY_NAME,
	SK_DISPLAY_NAME_L10N
};



/// @brief Deletes all entries in a pointer list and clears it afterwards
///
template < class T >
void
destroy_list_entries (
	T & list_n )
{
	if ( list_n.size() > 0 ) {
		typename T::iterator it_end ( list_n.end() );
		typename T::iterator it ( list_n.begin() );
		for ( ; it != it_end;  ++it ) {
			delete *it;
		}
		list_n.clear();
	}
}


/// @brief Simple two integer struct
///
class Integer_Pair
{
	// Public methods
	public:

	Integer_Pair ( );

	Integer_Pair (
		long val0_n,
		long val1_n );

	long &
	operator[] (
		unsigned int idx_n );

	const long &
	operator[] (
		unsigned int idx_n ) const;

	// Private attributes
	private:

	long _vals[2];
};

inline
Integer_Pair::Integer_Pair ( )
{
}

inline
Integer_Pair::Integer_Pair (
	long val0_n,
	long val1_n )
{
	_vals[0] = val0_n;
	_vals[1] = val1_n;
}

inline
long &
Integer_Pair::operator[] (
	unsigned int idx_n )
{
	return _vals[idx_n];
}

inline
const long &
Integer_Pair::operator[] (
	unsigned int idx_n ) const
{
	return _vals[idx_n];
}



/// @brief Base class for control proxies
///
class Proxy
{
	// Public methods
	public:

	Proxy (
		unsigned int control_type_n,
		::QSnd2::Proxies_Group1 * group_n = 0 );

	virtual
	~Proxy ( );


	/// @brief Control type (slider, switch, etc. )
	///
	unsigned int
	control_type ( ) const;

	/// @brief Access function for string values
	/// @arg str_n return string value
	/// @arg key_n string key (string selector)
	/// @return true if a string for the give key was available
	virtual
	bool
	string_val (
		QString & str_n,
		unsigned int key_n ) const;

	/// @brief Parent proxies group
	///
	::QSnd2::Proxies_Group1 *
	pgroup ( ) const;

	void
	set_pgroup (
		::QSnd2::Proxies_Group1 * group_n );


	/// @brief Value change callback
	///
	/// This one will be called if the value of
	/// the proxy object changed (e.g. slider volume changed)
	const ::Context_Callback &
	val_change_callback ( ) const;

	void
	set_val_change_callback (
		const ::Context_Callback & callback_n );


	///@brief calls the val_change_callback
	///
	void
	notify_value_changed ( ) const;


	// Private attributes
	private:

	const unsigned int _control_type;
	::Context_Callback _val_change_callback;
	::QSnd2::Proxies_Group1 * _pgroup;
};

inline
unsigned int
Proxy::control_type ( ) const
{
	return _control_type;
}

inline
const ::Context_Callback &
Proxy::val_change_callback ( ) const
{
	return _val_change_callback;
}



/// @brief Slider proxy
///
class Proxy_Slider :
	public ::QSnd2::Proxy
{
	// Public methods
	public:

	Proxy_Slider ( );

	~Proxy_Slider ( );

	::QSnd2::Proxies_Group1_Slider *
	slider_pgroup ( ) const;

	virtual
	long
	int_value ( ) const = 0;

	virtual
	void
	set_int_value (
		long value_n ) = 0;
};



/// @brief Switch proxy
///
class Proxy_Switch :
	public ::QSnd2::Proxy
{
	// Public methods
	public:

	Proxy_Switch ( );

	~Proxy_Switch ( );


	::QSnd2::Proxies_Group1_Switch *
	switch_pgroup ( ) const;

	// Generic interface

	virtual
	bool
	switch_state ( ) const = 0;

	virtual
	void
	set_switch_state (
		bool state_n ) = 0;

	virtual
	void
	toggle_switch_state ( );
};



/// @brief Proxy_Enum
///
class Proxy_Enum :
	public ::QSnd2::Proxy
{
	// Public methods
	public:

	Proxy_Enum ( );

	~Proxy_Enum ( );

	::QSnd2::Proxies_Group1_Enum *
	group ( ) const;
};



/// @brief Base class for the proxy tree
///
class Proxies_Group
{
	// Public methods
	public:

	Proxies_Group (
		unsigned int level_n );

	virtual
	~Proxies_Group ( );

	/// @brief Group tree depth 0 is at the broad end
	///
	unsigned int
	group_level ( ) const;

	/// @brief Access function for string values
	/// @arg str_n return string value
	/// @arg key_n string key (string selector)
	/// @return true if a string for the give key was available
	virtual
	bool
	string_val (
		QString & str_n,
		unsigned int key_n ) const;


	// Private attributes
	private:

	const unsigned int _group_level;
};

inline
unsigned int
Proxies_Group::group_level ( ) const
{
	return _group_level;
}



/// @brief Proxies_Group1
///
/// Specialized versions of this class hold
/// information common to all child proxy objects
/// (e.g. switches, sliders, enums )
///
class Proxies_Group1 :
	public ::QSnd2::Proxies_Group
{
	// Public methods
	public:

	Proxies_Group1 (
		unsigned int control_type_n );

	~Proxies_Group1 ( );


	unsigned int
	control_type ( ) const;


	void
	set_feature (
		unsigned int feat_id_n,
		bool on_n = true );

	bool
	has_feature (
		unsigned int feat_id_n ) const;


	void
	clear_children ( );

	unsigned int
	num_proxies ( ) const;

	::QSnd2::Proxy *
	proxy (
		unsigned int idx_n ) const;

	void
	append_proxy (
		::QSnd2::Proxy * proxy_n );


	/// @brief Value change callback
	///
	/// This one will be called if the value of
	/// the proxy object changed (e.g. slider volume changed)
	const ::Context_Callback &
	val_change_callback ( ) const;

	void
	set_val_change_callback (
		const ::Context_Callback & callback_n );


	void
	notify_proxies_value_changed ( );


	// Private attributes
	private:

	const unsigned int _control_type;
	::Flags _feature_flags;
	::Context_Callback _val_change_callback;
	QList < ::QSnd2::Proxy * > _proxies;
};

inline
::QSnd2::Proxies_Group1 *
Proxy::pgroup ( ) const
{
	return _pgroup;
}

inline
unsigned int
Proxies_Group1::control_type ( ) const
{
	return _control_type;
}

inline
const ::Context_Callback &
Proxies_Group1::val_change_callback ( ) const
{
	return _val_change_callback;
}

inline
void
Proxies_Group1::set_feature (
	unsigned int feat_id_n,
	bool on_n )
{
	_feature_flags.set ( feat_id_n, on_n );
}

inline
bool
Proxies_Group1::has_feature (
	unsigned int feat_id_n ) const
{
	return ( _feature_flags.has_any ( feat_id_n ) );
}

inline
unsigned int
Proxies_Group1::num_proxies ( ) const
{
	return _proxies.size();
}

inline
::QSnd2::Proxy *
Proxies_Group1::proxy (
	unsigned int idx_n ) const
{
	return _proxies[idx_n];
}


/// @brief Proxies_Group1_Slider
///
class Proxies_Group1_Slider :
	public ::QSnd2::Proxies_Group1
{
	// Public methods
	public:

	Proxies_Group1_Slider ( );

	~Proxies_Group1_Slider ( );

	::QSnd2::Proxy_Slider *
	slider_proxy (
		unsigned int idx_n ) const;


	virtual
	void
	int_range (
		::QSnd2::Integer_Pair & range_n ) const = 0;

	virtual
	void
	db_range (
		::QSnd2::Integer_Pair & range_n ) const = 0;

	virtual
	long
	db_from_int (
		long intval_n ) const = 0;

	virtual
	long
	int_from_db (
		long dbval_n,
		int dir_n ) const = 0;


	virtual
	long
	int_value_joined ( );

	virtual
	void
	set_int_value_joined (
		long value_n );


	virtual
	bool
	values_equal ( ) const;
};

inline
::QSnd2::Proxies_Group1_Slider *
Proxy_Slider::slider_pgroup ( ) const
{
	return static_cast < ::QSnd2::Proxies_Group1_Slider * > (
		::QSnd2::Proxy::pgroup() );
}

inline
::QSnd2::Proxy_Slider *
Proxies_Group1_Slider::slider_proxy (
	unsigned int idx_n ) const
{
	return static_cast < ::QSnd2::Proxy_Slider * >  (
		::QSnd2::Proxies_Group1::proxy ( idx_n ) );
}



/// @brief Proxies_Group1_Switch
///
class Proxies_Group1_Switch :
	public ::QSnd2::Proxies_Group1
{
	// Public methods
	public:

	Proxies_Group1_Switch ( );

	~Proxies_Group1_Switch ( );


	::QSnd2::Proxy_Switch *
	switch_proxy (
		unsigned int idx_n ) const;


	virtual
	bool
	switches_state_joined ( );

	virtual
	void
	set_switches_state_joined (
		bool state_n );

	virtual
	void
	toggle_switches ( );


	virtual
	bool
	values_equal ( ) const;
};

inline
::QSnd2::Proxies_Group1_Switch *
Proxy_Switch::switch_pgroup ( ) const
{
	return static_cast < ::QSnd2::Proxies_Group1_Switch * > (
		::QSnd2::Proxy::pgroup() );
}

inline
::QSnd2::Proxy_Switch *
Proxies_Group1_Switch::switch_proxy (
	unsigned int idx_n ) const
{
	return static_cast < ::QSnd2::Proxy_Switch * >  (
		::QSnd2::Proxies_Group1::proxy ( idx_n ) );
}


class Enum_Proxy_Item
{
	// Public methods
	public:

	QString name;
	QString name_l10n;
};

/// @brief Proxies_Group1_Enum
///
class Proxies_Group1_Enum :
	public ::QSnd2::Proxies_Group1
{
	// Public methods
	public:

	Proxies_Group1_Enum ( );

	~Proxies_Group1_Enum ( );

	::QSnd2::Proxy_Enum *
	enum_proxy (
		unsigned int idx_n ) const;


	unsigned int
	num_items ( ) const;

	const ::QSnd2::Enum_Proxy_Item &
	item (
		unsigned int idx_n );


	// Private attributes
	private:

	QList < ::QSnd2::Enum_Proxy_Item > _items;
};

inline
::QSnd2::Proxies_Group1_Enum *
Proxy_Enum::group ( ) const
{
	return static_cast < ::QSnd2::Proxies_Group1_Enum * > (
		::QSnd2::Proxy::pgroup() );
}

inline
::QSnd2::Proxy_Enum *
Proxies_Group1_Enum::enum_proxy (
	unsigned int idx_n ) const
{
	return static_cast < ::QSnd2::Proxy_Enum * >  (
		::QSnd2::Proxies_Group1::proxy ( idx_n ) );
}

inline
unsigned int
Proxies_Group1_Enum::num_items ( ) const
{
	return _items.size();
}

inline
const ::QSnd2::Enum_Proxy_Item &
Proxies_Group1_Enum::item (
	unsigned int idx_n )
{
	return _items[idx_n];
}



/// @brief Proxies_Group2
///
class Proxies_Group2 :
	public ::QSnd2::Proxies_Group
{
	// Public methods
	public:

	Proxies_Group2 ( );

	virtual
	~Proxies_Group2 ( );

	void
	clear_children ( );

	unsigned int
	num_groups ( ) const;

	::QSnd2::Proxies_Group1 *
	group (
		unsigned int idx_n );

	void
	append_group (
		::QSnd2::Proxies_Group1 * grp_n );


	::QSnd2::Proxies_Group1 *
	find_group_type (
		unsigned int elem_type_n ) const;

	::QSnd2::Proxies_Group1_Slider *
	sliders ( ) const;

	::QSnd2::Proxies_Group1_Switch *
	switches ( ) const;

	::QSnd2::Proxies_Group1_Enum *
	enums ( ) const;


	void
	notify_proxies_value_changed ( );


	// Private attributes
	private:

	QList < ::QSnd2::Proxies_Group1 * > _groups;
};

inline
unsigned int
Proxies_Group2::num_groups ( ) const
{
	return _groups.size();
}

inline
::QSnd2::Proxies_Group1 *
Proxies_Group2::group (
	unsigned int idx_n )
{
	return _groups[idx_n];
}


/// @brief Proxies_Group3
///
class Proxies_Group3 :
	public ::QSnd2::Proxies_Group
{
	// Public methods
	public:

	Proxies_Group3 ( );

	virtual
	~Proxies_Group3 ( );

	void
	clear_children ( );

	unsigned int
	num_groups ( ) const;

	::QSnd2::Proxies_Group2 *
	group (
		unsigned int idx_n );

	void
	append_group (
		::QSnd2::Proxies_Group2 * grp_n );


	void
	notify_proxies_value_changed ( );


	// Private attributes
	private:

	QList < ::QSnd2::Proxies_Group2 * > _groups;
};

inline
unsigned int
Proxies_Group3::num_groups ( ) const
{
	return _groups.size();
}

inline
::QSnd2::Proxies_Group2 *
Proxies_Group3::group (
	unsigned int idx_n )
{
	return _groups[idx_n];
}


/// @brief Proxies_Group4
///
class Proxies_Group4 :
	public ::QSnd2::Proxies_Group
{
	// Public methods
	public:

	Proxies_Group4 ( );

	virtual
	~Proxies_Group4 ( );

	void
	clear_children ( );

	unsigned int
	num_groups ( ) const;

	::QSnd2::Proxies_Group3 *
	group (
		unsigned int idx_n );

	void
	append_group (
		::QSnd2::Proxies_Group3 * grp_n );


	// Private attributes
	private:

	QList < ::QSnd2::Proxies_Group3 * > _groups;
};

inline
unsigned int
Proxies_Group4::num_groups ( ) const
{
	return _groups.size();
}

inline
::QSnd2::Proxies_Group3 *
Proxies_Group4::group (
	unsigned int idx_n )
{
	return _groups[idx_n];
}


} // End of namespace

#endif
