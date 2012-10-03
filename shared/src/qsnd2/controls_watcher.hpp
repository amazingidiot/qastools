//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_qsnd2_controls_watcher_hpp__
#define __INC_qsnd2_controls_watcher_hpp__

#include "controls.hpp"
#include <QList>
#include <QSocketNotifier>

namespace QSnd2
{


/// @brief Controls_Watcher - Listens to sockets for control changes
///
class Controls_Watcher :
	public QObject
{
	Q_OBJECT

	// Public methods
	public:

	Controls_Watcher (
		::QSnd2::Controls * ctl_n = 0 );

	~Controls_Watcher ( );


	::QSnd2::Controls *
	controls ( ) const;

	void
	set_controls (
		::QSnd2::Controls * ctl_n );


	// Private slots
	private slots:

	void
	socket_data (
		int socket_id_n );


	// Private methods
	private:

	void
	clear_socket_notifiers ( );

	void
	create_socket_nofifiers ( );


	// Private attributes
	private:

	::QSnd2::Controls * _controls;
	QList < QSocketNotifier * > _socket_notifiers;
};

inline
::QSnd2::Controls *
Controls_Watcher::controls ( ) const
{
	return _controls;
}


} // End of namespace

#endif
