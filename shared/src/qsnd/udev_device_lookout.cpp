//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "udev_device_lookout.hpp"
#include <QSocketNotifier>
#include <iostream>


namespace QSnd
{


UDev_Device_Lookout::UDev_Device_Lookout (
	QObject * parent_n ) :
QObject ( parent_n ),
_udev ( 0 ),
_udev_mon ( 0 )
{
	udev_init();
}

UDev_Device_Lookout::~UDev_Device_Lookout ( )
{
	udev_close();
}

void
UDev_Device_Lookout::udev_init ( )
{
	// Init udev
	if ( _udev == 0 ) {
		_udev = ::udev_new();
		if ( _udev == 0 ) {
			::std::cout << "Could not create udev object\n";
		} else {
			_udev_mon = ::udev_monitor_new_from_netlink ( _udev, "udev" );
			if ( _udev_mon == 0 ) {
				::std::cout << "Could not create udev monitor object\n";
				udev_close();
			} else {
				::udev_monitor_filter_add_match_subsystem_devtype (
					_udev_mon,
					"sound",
					0 );
				::udev_monitor_enable_receiving ( _udev_mon );

				// Socket notifier
				{
					QSocketNotifier * socknot (
						new QSocketNotifier (
							::udev_monitor_get_fd ( _udev_mon ),
							QSocketNotifier::Read,
							this ) );
					connect (
						socknot, SIGNAL ( activated ( int ) ),
						this, SLOT ( udev_process() ) );
				}
			}
		}
	}
}

void
UDev_Device_Lookout::udev_close ( )
{
	if ( _udev != 0 ) {
		if ( _udev_mon != 0 ) {
			::udev_monitor_unref ( _udev_mon );
			_udev_mon = 0;
		}
		::udev_unref ( _udev );
		_udev = 0;
	}
}

void
UDev_Device_Lookout::udev_process ( )
{
	bool any_change ( false );
	while ( true ) {
		// Read all device device changes from udev
		::udev_device * dev ( ::udev_monitor_receive_device ( _udev_mon ) );
		if ( dev == 0 ) {
			break;
		} else {
#ifndef NDEBUG
	{
		const char * devnode ( ::udev_device_get_devnode ( dev ) );
		const char * subsystem ( ::udev_device_get_subsystem ( dev ) );
		const char * devtype ( ::udev_device_get_devtype ( dev ) );
		const char * action ( ::udev_device_get_action ( dev ) );
		::std::cout << "UDev action" << "\n";
		::std::cout << "  Node:      " << ( devnode ? devnode : "(null)" ) << "\n";
		::std::cout << "  Subsystem: " << ( subsystem ? subsystem : "(null)" ) << "\n";
		::std::cout << "  Devtype:   " << ( devtype ? devtype : "(null)" ) << "\n";
		::std::cout << "  Action:    " << ( action ? action : "(null)" ) << "\n";
	}
#endif // NDEBUG
			any_change = true;
			::udev_device_unref ( dev );
		}
	}

	if ( any_change ) {
		emit sig_change();
	}
}



} // End of namespace
