//
// C++ Implementation:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "ctl_info.hpp"

#include <qsnd/event_types.hpp>

#include <QCoreApplication>
#include <iostream>


namespace QSnd
{


CTL_Info::CTL_Info ( ) :
_snd_hctl_handle ( 0 ),
_reload_requested ( false )
{
}


CTL_Info::~CTL_Info ( )
{
	close();
}


void
CTL_Info::close ( )
{
	_ctl_name.clear();

	_card_info.clear();
	for ( int ii=0; ii < _devices_info.size(); ++ii ) {
		delete _devices_info[ii];
	}
	_devices_info.clear();

	if ( _snd_hctl_handle != 0 ) {
		snd_hctl_set_callback ( _snd_hctl_handle, 0 );
		snd_hctl_set_callback_private ( _snd_hctl_handle, 0 );

		//::std::cout << "Closing hctl handle\n";
		snd_hctl_close ( _snd_hctl_handle );
		_snd_hctl_handle = 0;
	}
}


int
CTL_Info::open (
	const QString & dev_str_n )
{
	close();

	_err_func.clear();
	_err_message.clear();

	_ctl_name = dev_str_n;

	int err;

	// Open snd hctl handle
	{
		err = snd_hctl_open (
			&_snd_hctl_handle,
			dev_str_n.toLocal8Bit().constData(),
			SND_CTL_NONBLOCK );

		if ( err < 0 ) {
			_snd_hctl_handle = 0;
			_err_func = "snd_hctl_open";
			_err_message = snd_error_qstring ( err );
			close();
			return err;
		}

		err = snd_hctl_load ( _snd_hctl_handle );
		if ( err < 0 ) {
			_err_func = "snd_hctl_load";
			_err_message = snd_error_qstring ( err );
			close();
			return err;
		}
	}

	snd_hctl_set_callback ( _snd_hctl_handle, &CTL_Info::alsa_callback_hctl_info );
	snd_hctl_set_callback_private ( _snd_hctl_handle, this );

	_card_info.acquire_info ( _snd_hctl_handle );
	err = acquire_devices_info();

	return err;
}


int
CTL_Info::acquire_devices_info ( )
{
	int err ( 0 );
	int dev_idx ( -1 );

	snd_ctl_t * snd_ctl ( snd_hctl_ctl ( _snd_hctl_handle ) );

	while ( true ) {
		err = snd_ctl_pcm_next_device ( snd_ctl, &dev_idx );
		if ( err  < 0 ) {
			break;
		}
		if ( dev_idx < 0 ) {
			break;
		}

		PCM_Device_Info * dev_info ( new PCM_Device_Info );
		err = dev_info->acquire_device_info ( snd_ctl, dev_idx );
		if ( err >= 0 ) {
			_devices_info.append ( dev_info );
		} else {
			delete dev_info;
		}
	}

	return err;
}


void
CTL_Info::request_reload ( )
{
	if ( !_reload_requested ) {
		_reload_requested = true;

		QCoreApplication::postEvent (
			this, new QEvent ( ::QSnd::evt_reload_request ) );
	}
}


bool
CTL_Info::event (
	QEvent * event_n )
{
	bool res ( true );

	if ( event_n->type() == ::QSnd::evt_reload_request ) {
		_reload_requested = false;
		emit sig_reload_request();
	} else {
		res = QObject::event ( event_n );
	}

	return res;
}


// Alsa callbacks

int
CTL_Info::alsa_callback_hctl_info (
	snd_hctl_t * snd_hctl_n,
	unsigned int mask_n,
	snd_hctl_elem_t * )
{
	int res ( 0 );

	CTL_Info * snd_info;
	{
		void * priv ( snd_hctl_get_callback_private ( snd_hctl_n ) );
		snd_info = reinterpret_cast < CTL_Info * > ( priv );
	}

	if ( snd_info != 0 ) {
		const unsigned int change_mask (
			SND_CTL_EVENT_MASK_VALUE |
			SND_CTL_EVENT_MASK_INFO |
			SND_CTL_EVENT_MASK_ADD |
			SND_CTL_EVENT_MASK_TLV );

		if ( ( mask_n == SND_CTL_EVENT_MASK_REMOVE ) ||
			( ( mask_n & change_mask ) != 0 ) )
		{
			snd_info->request_reload();
		} else {
			::std::cerr << "CTL_Info::alsa_callback_hctl_info: ";
			::std::cerr << "Unknown mask ( " << mask_n << " )\n";
			res = -1;
		}
	}

	return res;
}


} // End of namespace
