//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_qsnd_udev_device_lookout_hpp__
#define __INC_qsnd_udev_device_lookout_hpp__

#include <QObject>
#include <string>
#include <libudev.h>

namespace QSnd
{


/// @brief UDev_Device_Lookout
///
class UDev_Device_Lookout :
	public QObject
{
	Q_OBJECT

	// Public methods
	public:

	UDev_Device_Lookout (
		QObject * parent_n = 0 );

	~UDev_Device_Lookout ( );


	// Signals
	signals:

	void
	sig_change ( );


	// Protected slots
	protected slots:

	void
	udev_process ( );


	// Private methods
	private:

	void
	udev_init ( );

	void
	udev_close ( );


	// Private attributes
	private:

	// udev device detection
	::udev * _udev;
	::udev_monitor * _udev_mon;
};


} // End of namespace

#endif
