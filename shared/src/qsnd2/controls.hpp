//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_qsnd_controls_hpp__
#define __INC_qsnd_controls_hpp__

#include "control_proxies.hpp"
#include <poll.h>
#include <QScopedArrayPointer>

namespace QSnd2
{


/// @brief Controls
///
class Controls
{
	// Public methods
	public:

	Controls ( );

	virtual
	~Controls ( );


	unsigned int
	num_groups ( ) const;

	::QSnd2::Proxies_Group4 *
	group (
		unsigned int idx_n ) const;


	unsigned int
	num_pollfds ( ) const;

	pollfd *
	pollfds ( );

	virtual
	void
	socket_data (
		unsigned int socket_id_n );


	// Protected methods
	protected:

	void
	clear_groups ( );

	void
	append_group (
		::QSnd2::Proxies_Group4 * grp_n );

	void
	clear_pollfds ( );

	void
	set_num_pollfds (
		unsigned int num_n );


	// Private attributes
	private:

	QList < ::QSnd2::Proxies_Group4 * > _groups;
	unsigned int _num_pollfds;
	QScopedArrayPointer < pollfd > _pollfds;
};

inline
unsigned int
Controls::num_groups ( ) const
{
	return _groups.size();
}

inline
::QSnd2::Proxies_Group4 *
Controls::group (
	unsigned int idx_n ) const
{
	return _groups[idx_n];
}

inline
unsigned int
Controls::num_pollfds ( ) const
{
	return _num_pollfds;
}

inline
pollfd *
Controls::pollfds ( )
{
	return _pollfds.data();
}


} // End of namespace

#endif
