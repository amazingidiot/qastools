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
#include <QScopedPointer>

namespace QSnd2
{

// Forward declaration
class Proxies_Group0;
class Proxies_Group1;
class Proxies_Group2;
class Slider_Proxies_Group;
class Switch_Proxies_Group;
class Enum_Proxies_Group;

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
		::QSnd2::Proxies_Group0 * group_n = 0 );

	virtual
	~Proxy ( );


	::QSnd2::Proxies_Group0 *
	pgroup ( ) const;

	void
	set_pgroup (
		::QSnd2::Proxies_Group0 * group_n );


	// Private attributes
	private:

	::QSnd2::Proxies_Group0 * _pgroup;
};

/// @brief Parent of Proxy instances - holds information shared by all child proxies
///
class Proxies_Group0
{
	// Public methods
	public:

	Proxies_Group0 ( );

	virtual
	~Proxies_Group0 ( );

	void
	clear ( );

	unsigned int
	num_proxies ( ) const;

	::QSnd2::Proxy *
	proxy (
		unsigned int idx_n ) const;


	// Private attributes
	private:

	QList < ::QSnd2::Proxy * > _proxies;
};

inline
::QSnd2::Proxies_Group0 *
Proxy::pgroup ( ) const
{
	return _pgroup;
}

inline
unsigned int
Proxies_Group0::num_proxies ( ) const
{
	return _proxies.size();
}

inline
::QSnd2::Proxy *
Proxies_Group0::proxy (
	unsigned int idx_n ) const
{
	return _proxies[idx_n];
}



/// @brief Slider_Proxy
///
class Slider_Proxy :
	public ::QSnd2::Proxy
{
	// Public methods
	public:

	Slider_Proxy ( );

	~Slider_Proxy ( );

	::QSnd2::Slider_Proxies_Group *
	slider_pgroup ( ) const;

	virtual
	long
	int_value ( ) const = 0;

	virtual
	void
	set_int_value (
		long value_n ) = 0;
};

/// @brief Slider_Proxies_Group
///
class Slider_Proxies_Group :
	public ::QSnd2::Proxies_Group0
{
	// Public methods
	public:

	Slider_Proxies_Group ( );

	~Slider_Proxies_Group ( );

	::QSnd2::Slider_Proxy *
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
		long intval_n ) const = 0;
};

inline
::QSnd2::Slider_Proxies_Group *
Slider_Proxy::slider_pgroup ( ) const
{
	return static_cast < ::QSnd2::Slider_Proxies_Group * > (
		::QSnd2::Proxy::pgroup() );
}

inline
::QSnd2::Slider_Proxy *
Slider_Proxies_Group::slider_proxy (
	unsigned int idx_n ) const
{
	return static_cast < ::QSnd2::Slider_Proxy * >  (
		::QSnd2::Proxies_Group0::proxy ( idx_n ) );
}



/// @brief Switch_Proxy
///
class Switch_Proxy :
	public ::QSnd2::Proxy
{
	// Public methods
	public:

	Switch_Proxy ( );

	~Switch_Proxy ( );

	::QSnd2::Switch_Proxies_Group *
	group ( ) const;
};

/// @brief Switch_Proxies_Group
///
class Switch_Proxies_Group :
	public ::QSnd2::Proxies_Group0
{
	// Public methods
	public:

	Switch_Proxies_Group ( );

	~Switch_Proxies_Group ( );

	::QSnd2::Switch_Proxy *
	switch_proxy (
		unsigned int idx_n ) const;
};

inline
::QSnd2::Switch_Proxies_Group *
Switch_Proxy::group ( ) const
{
	return static_cast < ::QSnd2::Switch_Proxies_Group * > (
		::QSnd2::Proxy::pgroup() );
}

inline
::QSnd2::Switch_Proxy *
Switch_Proxies_Group::switch_proxy (
	unsigned int idx_n ) const
{
	return static_cast < ::QSnd2::Switch_Proxy * >  (
		::QSnd2::Proxies_Group0::proxy ( idx_n ) );
}


class Enum_Proxy_Item
{
	// Public methods
	public:

	QString name;
	QString name_l10n;
};

/// @brief Enum_Proxy
///
class Enum_Proxy :
	public ::QSnd2::Proxy
{
	// Public methods
	public:

	Enum_Proxy ( );

	~Enum_Proxy ( );

	::QSnd2::Enum_Proxies_Group *
	group ( ) const;
};

/// @brief Enum_Proxies_Group
///
class Enum_Proxies_Group :
	public ::QSnd2::Proxies_Group0
{
	// Public methods
	public:

	Enum_Proxies_Group ( );

	~Enum_Proxies_Group ( );

	::QSnd2::Enum_Proxy *
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
::QSnd2::Enum_Proxies_Group *
Enum_Proxy::group ( ) const
{
	return static_cast < ::QSnd2::Enum_Proxies_Group * > (
		::QSnd2::Proxy::pgroup() );
}

inline
::QSnd2::Enum_Proxy *
Enum_Proxies_Group::enum_proxy (
	unsigned int idx_n ) const
{
	return static_cast < ::QSnd2::Enum_Proxy * >  (
		::QSnd2::Proxies_Group0::proxy ( idx_n ) );
}

inline
unsigned int
Enum_Proxies_Group::num_items ( ) const
{
	return _items.size();
}

inline
const ::QSnd2::Enum_Proxy_Item &
Enum_Proxies_Group::item (
	unsigned int idx_n )
{
	return _items[idx_n];
}


/// @brief Group level 1 - holds a proxies group0 for every type
///
class Proxies_Group1
{
	// Public methods
	public:

	Proxies_Group1 ( );

	virtual
	~Proxies_Group1 ( );

	void
	clear_groups ( );

	::QSnd2::Slider_Proxies_Group *
	sliders ( ) const;

	void
	set_sliders (
		::QSnd2::Slider_Proxies_Group * grp_n );


	::QSnd2::Switch_Proxies_Group *
	switches ( ) const;

	void
	set_switches (
		::QSnd2::Switch_Proxies_Group * grp_n );


	::QSnd2::Enum_Proxies_Group *
	enums ( ) const;

	void
	set_enums (
		::QSnd2::Enum_Proxies_Group * grp_n );


	// Private attributes
	private:

	QScopedPointer < ::QSnd2::Slider_Proxies_Group > _sliders;
	QScopedPointer < ::QSnd2::Switch_Proxies_Group > _switches;
	QScopedPointer < ::QSnd2::Enum_Proxies_Group > _enums;
};

inline
::QSnd2::Slider_Proxies_Group *
Proxies_Group1::sliders ( ) const
{
	return _sliders.data();
}

inline
::QSnd2::Switch_Proxies_Group *
Proxies_Group1::switches ( ) const
{
	return _switches.data();
}

inline
::QSnd2::Enum_Proxies_Group *
Proxies_Group1::enums ( ) const
{
	return _enums.data();
}


/// @brief Group level 2 - holds groups of level 1
///
class Proxies_Group2
{
	// Public methods
	public:

	Proxies_Group2 ( );

	virtual
	~Proxies_Group2 ( );


	unsigned int
	num_groups ( ) const;

	::QSnd2::Proxies_Group1 *
	group (
		unsigned int idx_n );

	void
	clear_groups ( );

	void
	append_group (
		::QSnd2::Proxies_Group1 * grp_n );


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


/// @brief Group level 3 - holds groups of level 2
///
class Proxies_Group3
{
	// Public methods
	public:

	Proxies_Group3 ( );

	virtual
	~Proxies_Group3 ( );


	unsigned int
	num_groups ( ) const;

	::QSnd2::Proxies_Group1 *
	group (
		unsigned int idx_n );

	void
	clear_groups ( );

	void
	append_group (
		::QSnd2::Proxies_Group1 * grp_n );


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
::QSnd2::Proxies_Group1 *
Proxies_Group3::group (
	unsigned int idx_n )
{
	return _groups[idx_n];
}


} // End of namespace

#endif
