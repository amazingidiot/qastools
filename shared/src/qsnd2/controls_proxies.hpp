//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_qsnd_control_proxies_hpp__
#define __INC_qsnd_control_proxies_hpp__

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


/// @brief Context plus function callback reference class
///
struct Context_Callback
{
	typedef void (*Func)( void * context_n );

	Context_Callback (
		void * context_n = 0,
		Func func_n = 0 );

	void
	call ( );

	void * context;
	Func function;
};

inline
Context_Callback::Context_Callback (
	void * context_n,
	Func func_n ) :
context ( context_n ),
function ( func_n )
{
}

inline
void
Context_Callback::call ( )
{
	function ( context );
}


/// @brief Base class for the proxy tree
///
class Proxy_Object
{
	// Public methods
	public:

	Proxy_Object (
		unsigned int level_n );

	virtual
	~Proxy_Object ( );

	unsigned int
	group_level ( ) const;


	// Private attributes
	private:

	const unsigned int _group_level;
};

inline
unsigned int
Proxy_Object::group_level ( ) const
{
	return _group_level;
}


/// @brief Base class for control proxies
///
class Proxy :
	public ::QSnd2::Proxy_Object
{
	// Public methods
	public:

	Proxy (
		unsigned int control_type_n,
		::QSnd2::Proxies_Group1 * group_n = 0 );

	~Proxy ( );

	unsigned int
	control_type ( ) const;

	::QSnd2::Proxies_Group1 *
	pgroup ( ) const;

	void
	set_pgroup (
		::QSnd2::Proxies_Group1 * group_n );


	// Private attributes
	private:

	const unsigned int _control_type;
	::QSnd2::Proxies_Group1 * _pgroup;
};

inline
unsigned int
Proxy::control_type ( ) const
{
	return _control_type;
}



/// @brief Proxies_Group1
///
/// Specialized versions of this class hold
/// information common to all child proxy objects
///
class Proxies_Group1 :
	public ::QSnd2::Proxy_Object
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


	// Private attributes
	private:

	const unsigned int _control_type;
	unsigned int _feature_flags;
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
void
Proxies_Group1::set_feature (
	unsigned int feat_id_n,
	bool on_n )
{
	if ( on_n ) {
		_feature_flags |= feat_id_n;
	} else {
		_feature_flags &= ~feat_id_n;
	}
}

inline
bool
Proxies_Group1::has_feature (
	unsigned int feat_id_n ) const
{
	return ( _feature_flags & feat_id_n );
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



/// @brief Proxy_Slider
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



/// @brief Proxy_Switch
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



/// @brief Proxies_Group1_Switch
///
class Proxies_Group1_Switch :
	public ::QSnd2::Proxies_Group1
{
	// Public methods
	public:

	Proxies_Group1_Switch ( );

	~Proxies_Group1_Switch ( );

	virtual
	void
	set_all_switches (
		bool state_n );

	virtual
	void
	toggle_all_switches ( );

	::QSnd2::Proxy_Switch *
	switch_proxy (
		unsigned int idx_n ) const;
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
	public ::QSnd2::Proxy_Object
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
	public ::QSnd2::Proxy_Object
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
	public ::QSnd2::Proxy_Object
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
