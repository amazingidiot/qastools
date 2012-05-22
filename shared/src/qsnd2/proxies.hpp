//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_qsnd_proxies_hpp__
#define __INC_qsnd_proxies_hpp__

#include <QList>
#include <QString>
#include <QScopedPointer>

namespace QSnd2
{

// Forward declaration
class Proxies_Group;


/// @brief Proxy
///
class Proxy
{
	// Public methods
	public:

	Proxy (
		::QSnd2::Proxies_Group * group_n = 0 );

	virtual
	~Proxy ( );

	::QSnd2::Proxies_Group *
	group ( ) const;

	void
	set_group (
		::QSnd2::Proxies_Group * group_n );


	// Private attributes
	private:

	::QSnd2::Proxies_Group * _group;
};

inline
::QSnd2::Proxies_Group *
Proxy::group ( ) const
{
	return _group;
}


/// @brief Proxies_Group
///
class Proxies_Group
{
	// Public methods
	public:

	Proxies_Group ( );

	virtual
	~Proxies_Group ( );

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
unsigned int
Proxies_Group::num_proxies ( ) const
{
	return _proxies.size();
}

inline
::QSnd2::Proxy *
Proxies_Group::proxy (
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
};

/// @brief Slider_Proxies_Group
///
class Slider_Proxies_Group :
	public ::QSnd2::Proxies_Group
{
	// Public methods
	public:

	Slider_Proxies_Group ( );

	~Slider_Proxies_Group ( );

	::QSnd2::Slider_Proxy *
	proxy (
		unsigned int idx_n ) const;
};

inline
::QSnd2::Slider_Proxy *
Slider_Proxies_Group::proxy (
	unsigned int idx_n ) const
{
	return static_cast < ::QSnd2::Slider_Proxy * >  (
		::QSnd2::Proxies_Group::proxy ( idx_n ) );
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

};

/// @brief Switch_Proxies_Group
///
class Switch_Proxies_Group :
	public ::QSnd2::Proxies_Group
{
	// Public methods
	public:

	Switch_Proxies_Group ( );

	~Switch_Proxies_Group ( );

	::QSnd2::Switch_Proxy *
	proxy (
		unsigned int idx_n ) const;
};

inline
::QSnd2::Switch_Proxy *
Switch_Proxies_Group::proxy (
	unsigned int idx_n ) const
{
	return static_cast < ::QSnd2::Switch_Proxy * >  (
		::QSnd2::Proxies_Group::proxy ( idx_n ) );
}


/// @brief List_Proxy
///
class List_Proxy :
	public ::QSnd2::Proxy
{
	// Public methods
	public:

	List_Proxy ( );

	~List_Proxy ( );
};

class List_Proxy_Item
{
	// Public methods
	public:

	QString name;
	QString name_l10n;
};

/// @brief List_Proxies_Group
///
class List_Proxies_Group :
	public ::QSnd2::Proxies_Group
{
	// Public methods
	public:

	List_Proxies_Group ( );

	~List_Proxies_Group ( );

	unsigned int
	num_items ( ) const;

	::QSnd2::List_Proxy *
	proxy (
		unsigned int idx_n ) const;
};

inline
::QSnd2::List_Proxy *
List_Proxies_Group::proxy (
	unsigned int idx_n ) const
{
	return static_cast < ::QSnd2::List_Proxy * >  (
		::QSnd2::Proxies_Group::proxy ( idx_n ) );
}


/// @brief Group level 1 - holds a proxies list for every type
///
class Mixer_Group1
{
	// Public methods
	public:

	Mixer_Group1 ( );

	virtual
	~Mixer_Group1 ( );


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


	::QSnd2::List_Proxies_Group *
	lists ( ) const;

	void
	set_lists (
		::QSnd2::List_Proxies_Group * grp_n );


	// Private attributes
	private:

	QScopedPointer < ::QSnd2::Slider_Proxies_Group > _sliders;
	QScopedPointer < ::QSnd2::Switch_Proxies_Group > _switches;
	QScopedPointer < ::QSnd2::List_Proxies_Group > _lists;
};

inline
::QSnd2::Slider_Proxies_Group *
Mixer_Group1::sliders ( ) const
{
	return _sliders.data();
}

inline
::QSnd2::Switch_Proxies_Group *
Mixer_Group1::switches ( ) const
{
	return _switches.data();
}

inline
::QSnd2::List_Proxies_Group *
Mixer_Group1::lists ( ) const
{
	return _lists.data();
}


/// @brief Group level 2 - holds groups of level 1
///
class Mixer_Group2
{
	// Public methods
	public:

	Mixer_Group2 ( );

	virtual
	~Mixer_Group2 ( );


	unsigned int
	num_groups ( ) const;

	::QSnd2::Mixer_Group1 *
	group (
		unsigned int idx_n );


	// Private attributes
	private:

	QList < ::QSnd2::Mixer_Group1 * > _groups;
};

inline
unsigned int
Mixer_Group2::num_groups ( ) const
{
	return _groups.size();
}

inline
::QSnd2::Mixer_Group1 *
Mixer_Group2::group (
	unsigned int idx_n )
{
	return _groups[idx_n];
}


} // End of namespace

#endif
