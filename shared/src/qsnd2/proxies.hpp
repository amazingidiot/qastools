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

namespace QSnd2
{


/// @brief Proxy
///
class Proxy
{
	// Public methods
	public:

	Proxy ( );

	virtual
	~Proxy ( );
};

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
	public:

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

/// @brief List_Proxies_Group
///
class List_Proxies_Group :
	public ::QSnd2::Proxies_Group
{
	// Public methods
	public:

	List_Proxies_Group ( );

	~List_Proxies_Group ( );

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


} // End of namespace

#endif
