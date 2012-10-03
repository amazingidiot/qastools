//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_qsnd2_control_proxies_hpp__
#define __INC_qsnd2_control_proxies_hpp__

#include "callbacks.hpp"
#include "flags.hpp"
#include <QList>
#include <QString>

// Forward declarations
namespace QSnd2
{
	class Proxy_Object;
	class Control_Proxy;
	class Proxies_Group;
	class Proxies_Group_Controls;
	class Proxies_Group_Sliders;
	class Proxies_Group_Switches;
	class Proxies_Group_Enums;
	class Proxies_Group2;
	class Proxies_Group3;
	class Proxies_Group4;
}

namespace QSnd2
{


enum Proxy_Type
{
	PTYPE_NONE = 0,
	PTYPE_CONTROL_SWITCH,
	PTYPE_CONTROL_SLIDER,
	PTYPE_CONTROL_ENUM,
	PTYPE_CGROUP_SLIDERS,
	PTYPE_CGROUP_SWITCHES,
	PTYPE_CGROUP_ENUMS,
	PTYPE_GROUP2,
	PTYPE_GROUP3,
	PTYPE_GROUP4
};

enum Control_Feature_Flag
{
	FFLAG_PLAYBACK = ( 1 << 0 ),
	FFLAG_CAPTURE  = ( 1 << 1 ),
	FFLAG_DECIBEL  = ( 1 << 2 )
};

enum String_Key
{
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


/// @brief Base class for all kinds of proxy objects
///
class Proxy_Object
{
	// Public methods
	public:

	Proxy_Object (
		unsigned int proxy_type_n );

	virtual
	~Proxy_Object ( );

	/// @brief The proxy type
	///
	/// @see ::QSnd2::Proxy_Type
	unsigned int
	proxy_type ( ) const;


	// Private attributes
	private:

	const unsigned int _proxy_type;
};

inline
unsigned int
Proxy_Object::proxy_type ( ) const
{
	return _proxy_type;
}


/// @brief Base class for control proxies
///
class Control_Proxy :
	public ::QSnd2::Proxy_Object
{
	// Public methods
	public:

	Control_Proxy (
		unsigned int proxy_type_n,
		::QSnd2::Proxies_Group_Controls * group_n = 0 );

	~Control_Proxy ( );


	/// @brief Parent controls group
	///
	::QSnd2::Proxies_Group_Controls *
	controls_group ( ) const;

	void
	set_controls_group (
		::QSnd2::Proxies_Group_Controls * group_n );


	/// @brief Access function for string values
	/// @arg str_n return string value
	/// @arg key_n string key (string selector)
	/// @return true if a string for the give key was available
	virtual
	bool
	string_val (
		QString & str_n,
		unsigned int key_n ) const;


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
	clear_val_change_callback ( );

	///@brief calls the val_change_callback
	///
	void
	call_val_change_callback ( ) const;


	// Private attributes
	private:

	::QSnd2::Proxies_Group_Controls * _controls_group;
	::Context_Callback _val_change_callback;
};

inline
::QSnd2::Proxies_Group_Controls *
Control_Proxy::controls_group ( ) const
{
	return _controls_group;
}

inline
const ::Context_Callback &
Control_Proxy::val_change_callback ( ) const
{
	return _val_change_callback;
}



/// @brief Slider proxy
///
class Proxy_Slider :
	public ::QSnd2::Control_Proxy
{
	// Public methods
	public:

	Proxy_Slider ( );

	~Proxy_Slider ( );


	/// @brief Parent sliders group
	/// @return The parent sliders group
	::QSnd2::Proxies_Group_Sliders *
	sliders_group ( ) const;


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
	public ::QSnd2::Control_Proxy
{
	// Public methods
	public:

	Proxy_Switch ( );

	~Proxy_Switch ( );


	/// @brief Parent switches group
	/// @return The parent switches group
	::QSnd2::Proxies_Group_Switches *
	switches_group ( ) const;


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
	public ::QSnd2::Control_Proxy
{
	// Public methods
	public:

	Proxy_Enum ( );

	~Proxy_Enum ( );


	::QSnd2::Proxies_Group_Enums *
	enums_group ( ) const;
};



/// @brief Base class for the proxy tree
///
class Proxies_Group :
	public ::QSnd2::Proxy_Object
{
	// Public methods
	public:

	Proxies_Group (
		unsigned int proxy_type_n );

	~Proxies_Group ( );


	void
	clear_children ( );

	unsigned int
	num_children ( ) const;

	::QSnd2::Proxy_Object *
	child_proxy (
		unsigned int idx_n ) const;

	void
	append_child_proxy (
		::QSnd2::Proxy_Object * proxy_n );


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

	QList < ::QSnd2::Proxy_Object * > _proxies;
};

inline
unsigned int
Proxies_Group::num_children ( ) const
{
	return _proxies.size();
}

inline
::QSnd2::Proxy_Object *
Proxies_Group::child_proxy (
	unsigned int idx_n ) const
{
	return _proxies[idx_n];
}


/// @brief Proxies_Group_Controls
///
/// Specialized versions of this class hold
/// information common to all child proxy objects
/// (e.g. switches, sliders, enums )
///
class Proxies_Group_Controls :
	public ::QSnd2::Proxies_Group
{
	// Public methods
	public:

	Proxies_Group_Controls (
		unsigned int proxy_type_n );

	~Proxies_Group_Controls ( );


	void
	set_feature (
		unsigned int feat_id_n,
		bool on_n = true );

	bool
	has_feature (
		unsigned int feat_id_n ) const;


	::QSnd2::Control_Proxy *
	control_proxy (
		unsigned int idx_n ) const;

	void
	append_control_proxy (
		::QSnd2::Control_Proxy * proxy_n );


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
	clear_val_change_callback ( );

	void
	call_val_changed_callbacks ( );


	// Private attributes
	private:

	::Flags _feature_flags;
	::Context_Callback _val_change_callback;
};

inline
const ::Context_Callback &
Proxies_Group_Controls::val_change_callback ( ) const
{
	return _val_change_callback;
}

inline
void
Proxies_Group_Controls::set_feature (
	unsigned int feat_id_n,
	bool on_n )
{
	_feature_flags.set ( feat_id_n, on_n );
}

inline
bool
Proxies_Group_Controls::has_feature (
	unsigned int feat_id_n ) const
{
	return ( _feature_flags.has_any ( feat_id_n ) );
}

inline
::QSnd2::Control_Proxy *
Proxies_Group_Controls::control_proxy (
	unsigned int idx_n ) const
{
	return static_cast < ::QSnd2::Control_Proxy * > ( child_proxy ( idx_n ) );
}


/// @brief Proxies_Group_Sliders
///
class Proxies_Group_Sliders :
	public ::QSnd2::Proxies_Group_Controls
{
	// Public methods
	public:

	Proxies_Group_Sliders ( );

	~Proxies_Group_Sliders ( );


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
::QSnd2::Proxies_Group_Sliders *
Proxy_Slider::sliders_group ( ) const
{
	return static_cast < ::QSnd2::Proxies_Group_Sliders * > (
		::QSnd2::Control_Proxy::controls_group() );
}

inline
::QSnd2::Proxy_Slider *
Proxies_Group_Sliders::slider_proxy (
	unsigned int idx_n ) const
{
	return static_cast < ::QSnd2::Proxy_Slider * >  (
		::QSnd2::Proxies_Group_Controls::child_proxy ( idx_n ) );
}



/// @brief Proxies_Group_Switches
///
class Proxies_Group_Switches :
	public ::QSnd2::Proxies_Group_Controls
{
	// Public methods
	public:

	Proxies_Group_Switches ( );

	~Proxies_Group_Switches ( );


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
::QSnd2::Proxies_Group_Switches *
Proxy_Switch::switches_group ( ) const
{
	return static_cast < ::QSnd2::Proxies_Group_Switches * > (
		::QSnd2::Control_Proxy::controls_group() );
}

inline
::QSnd2::Proxy_Switch *
Proxies_Group_Switches::switch_proxy (
	unsigned int idx_n ) const
{
	return static_cast < ::QSnd2::Proxy_Switch * >  (
		::QSnd2::Proxies_Group_Controls::child_proxy ( idx_n ) );
}


class Enum_Proxy_Item
{
	// Public methods
	public:

	QString name;
	QString name_l10n;
};

/// @brief Proxies_Group_Enums
///
class Proxies_Group_Enums :
	public ::QSnd2::Proxies_Group_Controls
{
	// Public methods
	public:

	Proxies_Group_Enums ( );

	~Proxies_Group_Enums ( );


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
::QSnd2::Proxies_Group_Enums *
Proxy_Enum::enums_group ( ) const
{
	return static_cast < ::QSnd2::Proxies_Group_Enums * > (
		::QSnd2::Control_Proxy::controls_group() );
}

inline
::QSnd2::Proxy_Enum *
Proxies_Group_Enums::enum_proxy (
	unsigned int idx_n ) const
{
	return static_cast < ::QSnd2::Proxy_Enum * >  (
		::QSnd2::Proxies_Group::child_proxy ( idx_n ) );
}

inline
unsigned int
Proxies_Group_Enums::num_items ( ) const
{
	return _items.size();
}

inline
const ::QSnd2::Enum_Proxy_Item &
Proxies_Group_Enums::item (
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

	~Proxies_Group2 ( );


	::QSnd2::Proxies_Group_Controls *
	controls_group (
		unsigned int idx_n );

	void
	append_controls_group (
		::QSnd2::Proxies_Group_Controls * group_n );


	/// @brief Finds respective sliders, switches, enums, etc. group
	::QSnd2::Proxies_Group_Controls *
	find_group_type (
		unsigned int proxy_type_n ) const;

	::QSnd2::Proxies_Group_Sliders *
	sliders ( ) const;

	::QSnd2::Proxies_Group_Switches *
	switches ( ) const;

	::QSnd2::Proxies_Group_Enums *
	enums ( ) const;


	void
	call_val_changed_callbacks ( );
};

inline
::QSnd2::Proxies_Group_Controls *
Proxies_Group2::controls_group (
	unsigned int idx_n )
{
	return static_cast < ::QSnd2::Proxies_Group_Controls * > (
		::QSnd2::Proxies_Group::child_proxy ( idx_n ) );
}


/// @brief Proxies_Group3
///
class Proxies_Group3 :
	public ::QSnd2::Proxies_Group
{
	// Public methods
	public:

	Proxies_Group3 ( );

	~Proxies_Group3 ( );


	::QSnd2::Proxies_Group2 *
	child_group (
		unsigned int idx_n );

	void
	append_child_group (
		::QSnd2::Proxies_Group2 * group_n );


	void
	call_val_changed_callbacks ( );
};

inline
::QSnd2::Proxies_Group2 *
Proxies_Group3::child_group (
	unsigned int idx_n )
{
	return static_cast < ::QSnd2::Proxies_Group2 * > (
		::QSnd2::Proxies_Group::child_proxy ( idx_n ) );
}


/// @brief Proxies_Group4
///
class Proxies_Group4 :
	public ::QSnd2::Proxies_Group
{
	// Public methods
	public:

	Proxies_Group4 ( );

	~Proxies_Group4 ( );


	::QSnd2::Proxies_Group3 *
	child_group (
		unsigned int idx_n );

	void
	append_child_group (
		::QSnd2::Proxies_Group3 * group_n );
};

inline
::QSnd2::Proxies_Group3 *
Proxies_Group4::child_group (
	unsigned int idx_n )
{
	return static_cast < ::QSnd2::Proxies_Group3 * > (
		::QSnd2::Proxies_Group::child_proxy ( idx_n ) );
}


} // End of namespace

#endif
