//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "alsa_smi.hpp"
#include <iostream>

namespace QSnd2
{


Alsa_SMI_Controls::Alsa_SMI_Controls ( ) :
_is_open ( false )
{
}

Alsa_SMI_Controls::~Alsa_SMI_Controls ( )
{
	close();
}

void
Alsa_SMI_Controls::close ( )
{
	clear_pollfds();
	clear_groups();

	// TODO:
	//clear_mixer_elems();

	if ( _snd_mixer != 0 ) {
		::snd_mixer_set_callback ( _snd_mixer, 0 );
		::snd_mixer_set_callback_private ( _snd_mixer, 0 );

		if ( _snd_hctl != 0 ) {
			::snd_mixer_detach_hctl ( _snd_mixer, _snd_hctl );
		}

		::snd_mixer_close ( _snd_mixer );
		_snd_mixer = 0;
	}

	if ( _snd_hctl != 0 ) {
		::snd_hctl_close ( _snd_hctl );
		_snd_hctl = 0;
	}
}

bool
Alsa_SMI_Controls::open (
	const QString & device_n )
{
	//::std::cout << "Alsa_SMI_Controls::open " << device_n.toLocal8Bit().constData() << "\n";

	close();

	_err_func.clear();
	_err_message.clear();

	if ( device_n.isEmpty() ) {
		_err_func = "open";
		// TODO:
		//_err_message = tr ( "Empty device name" );
		return -1;
	}

	int err;
	err = ::snd_mixer_open ( &_snd_mixer, 0 );
	if ( err < 0 ) {
		_err_func = "snd_mixer_open";
		_err_message = ::QSnd::snd_error_qstring ( err );
		return err;
	}

	err = ::snd_mixer_selem_register ( _snd_mixer, NULL, NULL );
	if ( err < 0 ) {
		_err_func = "snd_mixer_selem_register";
		_err_message = ::QSnd::snd_error_qstring ( err );
		close();
		return err;
	}

	// Open control handle
	err = ::snd_hctl_open (
		&_snd_hctl,
		device_n.toLocal8Bit().constData(),
		SND_CTL_NONBLOCK );

	if ( err < 0 ) {
		_err_func = "snd_hctl_open";
		_err_message = ::QSnd::snd_error_qstring ( err );
		close();
		return err;
	}

	// Attach hctl handle to mixer
	err = ::snd_mixer_attach_hctl ( _snd_mixer, _snd_hctl );
	if ( err < 0 ) {
		_snd_hctl = 0;
		_err_func = "snd_mixer_attach_hctl";
		_err_message = ::QSnd::snd_error_qstring ( err );
		close();
		return err;
	}

	// Load mixer
	err = ::snd_mixer_load ( _snd_mixer );
	if ( err < 0 ) {
		_err_func = "snd_mixer_load";
		_err_message = ::QSnd::snd_error_qstring ( err );
		close();
		return err;
	}

	// Install alsa callback
	::snd_mixer_set_callback_private ( _snd_mixer, this );
	::snd_mixer_set_callback ( _snd_mixer,
		&::QSnd2::Alsa_SMI_Controls::alsa_callback );

	// Create mixer elements
	//err = create_mixer_elems();
	//if ( err < 0 ) {
	//	close();
	//	return err;
	//}

	// Create socket notifiers
	err = load_pollfds();
	if ( err < 0 ) {
		close();
		return err;
	}

	return 0;
}

int
Alsa_SMI_Controls::load_pollfds ( )
{
	//::std::cout << "Alsa_SMI_Controls::load_pollfds" << "\n";

	int res ( 0 );

	{
		const int num_fds ( ::snd_mixer_poll_descriptors_count ( _snd_mixer ) );
		if ( num_fds >= 0 ) {
			set_num_pollfds ( num_fds );
		} else {
			res = num_fds;
			_err_func = "snd_mixer_poll_descriptors_count";
			_err_message = ::QSnd::snd_error_qstring ( num_fds );
		}
	}

	if ( num_pollfds() > 0 ) {
		memset ( pollfds(), 0, num_pollfds()*sizeof(pollfd) );
		int err ( ::snd_mixer_poll_descriptors (
			_snd_mixer, pollfds(), num_pollfds() ) );
		if ( err < 0 ) {
			res = err;
			set_num_pollfds ( 0 );
			_err_func = "snd_mixer_poll_descriptors";
			_err_message = ::QSnd::snd_error_qstring ( err );
		}
	}

	return res;
}


// Socket callbacks

void
Alsa_SMI_Controls::socket_data (
	int socket_id_n )
{
	(void) socket_id_n;

	if ( _snd_mixer != 0 ) {
		const int num_ev ( ::snd_mixer_handle_events ( _snd_mixer ) );
		if ( num_ev < 0 ) {
			// TODO: Error. Disable socket notifiers
			//set_socked_notifiers_enabled ( false );
			//print_alsa_error ( "snd_mixer_handle_events ", num_ev );
		}
	}
}

int
Alsa_SMI_Controls::alsa_callback (
	snd_mixer_t * snd_mixer_n,
	unsigned int mask_n,
	snd_mixer_elem_t * )
{
	int res ( 0 );

	::QSnd2::Alsa_SMI_Controls * alsa_controls =
		reinterpret_cast < ::QSnd2::Alsa_SMI_Controls * > (
		::snd_mixer_get_callback_private ( snd_mixer_n ) );

	if ( alsa_controls != 0 ) {
		const unsigned int change_mask (
			SND_CTL_EVENT_MASK_VALUE |
			SND_CTL_EVENT_MASK_INFO |
			SND_CTL_EVENT_MASK_ADD |
			SND_CTL_EVENT_MASK_TLV );

		if ( ( mask_n == SND_CTL_EVENT_MASK_REMOVE ) ||
			( ( mask_n & change_mask ) != 0 ) )
		{
			// TODO:
			//qsnd_mixer->request_reload();
		} else {
			::std::cerr << "Alsa_SMI_Controls::alsa_callback_mixer: ";
			::std::cerr << "Unknown mask ( " << mask_n << " )\n";
			res = -1;
		}
	}

	return res;
}


} // End of namespace
