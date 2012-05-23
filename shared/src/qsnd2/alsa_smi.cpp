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


ASMI_Proxy_Slider::ASMI_Proxy_Slider (
	::snd_mixer_selem_channel_id_t channel_id_n ) :
_snd_channel_id ( channel_id_n )
{
}

long
ASMI_Proxy_Slider::int_value ( ) const
{
	asmi_pgroup()->int_value ( _snd_channel_id );
}

void
ASMI_Proxy_Slider::set_int_value (
	long val_n )
{
	asmi_pgroup()->set_int_value ( _snd_channel_id, val_n );
}



ASMI_Proxies_Group1_Slider::ASMI_Proxies_Group1_Slider (
	::snd_mixer_elem_t * elem_n ) :
_snd_mixer_elem ( elem_n )
{
}

void
ASMI_Proxies_Group1_Slider::ASMI_Proxies_Group1_Slider::int_range (
	::QSnd2::Integer_Pair & range_n ) const
{
	::snd_mixer_selem_get_playback_volume_range (
		_snd_mixer_elem, &range_n[0], &range_n[1] );
}

long
ASMI_Proxies_Group1_Slider::ASMI_Proxies_Group1_Slider::int_from_db (
	long dbval_n,
	int dir_n ) const
{
	long res ( 0 );
	if ( has_feature ( ::QSnd2::FFLAG_CAPTURE ) ) {
		snd_mixer_selem_ask_capture_dB_vol (
			_snd_mixer_elem, dbval_n, dir_n, &res );
	} else {
		snd_mixer_selem_ask_playback_dB_vol (
			_snd_mixer_elem, dbval_n, dir_n, &res );
	}
	return res;
}

void
ASMI_Proxies_Group1_Slider::ASMI_Proxies_Group1_Slider::db_range (
	::QSnd2::Integer_Pair & range_n ) const
{
	int err ( -1 );
	if ( has_feature ( ::QSnd2::FFLAG_DECIBEL ) ) {
		int err = snd_mixer_selem_get_playback_dB_range (
			_snd_mixer_elem, &range_n[0], &range_n[1] );
	}
	if ( err < 0 ) {
		range_n[0] = 0;
		range_n[1] = 0;
	}
}

long
ASMI_Proxies_Group1_Slider::ASMI_Proxies_Group1_Slider::db_from_int (
	long intval_n ) const
{
	long res ( 0 );
	if ( has_feature ( ::QSnd2::FFLAG_CAPTURE ) ) {
		snd_mixer_selem_ask_capture_vol_dB (
			_snd_mixer_elem, intval_n, &res );
	} else {
		snd_mixer_selem_ask_playback_vol_dB (
			_snd_mixer_elem, intval_n, &res );
	}
	return res;
}

long
ASMI_Proxies_Group1_Slider::int_value (
	::snd_mixer_selem_channel_id_t channel_id_n ) const
{
	long val ( 0 );

	if ( has_feature ( ::QSnd2::FFLAG_CAPTURE ) ) {
		int err ( snd_mixer_selem_get_capture_volume (
			_snd_mixer_elem, channel_id_n, &val ) );
		if ( err < 0 ) {
			::QSnd::print_alsa_error ( "snd_mixer_selem_get_capture_volume", err );
		}
	} else {
		int err ( snd_mixer_selem_get_playback_volume (
			_snd_mixer_elem, channel_id_n, &val ) );
		if ( err < 0 ) {
			::QSnd::print_alsa_error ( "snd_mixer_selem_get_playback_volume", err );
		}
	}

	return val;
}

void
ASMI_Proxies_Group1_Slider::set_int_value (
	::snd_mixer_selem_channel_id_t,
	long val_n )
{

}



ASMI_Proxy_Switch::ASMI_Proxy_Switch (
	::snd_mixer_selem_channel_id_t channel_id_n ) :
_snd_channel_id ( channel_id_n )
{
}


ASMI_Proxies_Group2::ASMI_Proxies_Group2 (
	::snd_mixer_elem_t * elem_n ) :
_snd_mixer_elem ( elem_n )
{
	QScopedPointer < ::QSnd2::ASMI_Proxies_Group1_Slider > grp_play_slider (
		new ::QSnd2::ASMI_Proxies_Group1_Slider ( elem_n ) );
	QScopedPointer < ::QSnd2::ASMI_Proxies_Group1_Slider > grp_cap_slider (
		new ::QSnd2::ASMI_Proxies_Group1_Slider ( elem_n ) );

	grp_play_slider->set_feature ( ::QSnd2::FFLAG_PLAYBACK );
	grp_cap_slider->set_feature ( ::QSnd2::FFLAG_CAPTURE );

	// Playback sliders
	if ( ::snd_mixer_selem_has_playback_volume ( _snd_mixer_elem ) > 0 ) {
		// Detect available channels
		for ( int ii=0; ii <= SND_MIXER_SCHN_LAST; ++ii ) {
			::snd_mixer_selem_channel_id_t channel_id (
				static_cast < ::snd_mixer_selem_channel_id_t > ( ii ) );
			if ( ::snd_mixer_selem_has_playback_channel ( _snd_mixer_elem, channel_id ) ) {
				::QSnd2::ASMI_Proxy_Slider * slider_proxy (
					new ::QSnd2::ASMI_Proxy_Slider ( channel_id ) );
				grp_play_slider->append_proxy ( slider_proxy );
			}
		}

		// Detect decibel availability
		{
			::QSnd2::Integer_Pair vals ( 0, 0 );
			int err ( ::snd_mixer_selem_get_playback_dB_range (
				_snd_mixer_elem, &vals[0], &vals[1] ) );
			if ( ( err >= 0 ) && ( vals[0] != vals[1] ) ) {
				grp_play_slider->set_feature ( ::QSnd2::FFLAG_DECIBEL );
			}
		}
	}

	// Capture sliders
	if ( ::snd_mixer_selem_has_capture_volume ( _snd_mixer_elem ) > 0 ) {
		// Detect available channels
		for ( int ii=0; ii <= SND_MIXER_SCHN_LAST; ++ii ) {
			::snd_mixer_selem_channel_id_t channel_id (
				static_cast < ::snd_mixer_selem_channel_id_t > ( ii ) );
			if ( ::snd_mixer_selem_has_capture_channel ( _snd_mixer_elem, channel_id ) ) {
				::QSnd2::ASMI_Proxy_Slider * slider_proxy (
					new ::QSnd2::ASMI_Proxy_Slider ( channel_id ) );
				grp_cap_slider->append_proxy ( slider_proxy );
			}
		}

		// Detect decibel availability
		{
			::QSnd2::Integer_Pair vals ( 0, 0 );
			int err ( ::snd_mixer_selem_get_capture_dB_range (
				_snd_mixer_elem, &vals[0], &vals[1] ) );
			if ( ( err >= 0 ) && ( vals[0] != vals[1] ) ) {
				grp_cap_slider->set_feature ( ::QSnd2::FFLAG_DECIBEL );
			}
		}
	}

	// Install Alsa callbacks
	snd_mixer_elem_set_callback_private ( _snd_mixer_elem, this );
	snd_mixer_elem_set_callback ( _snd_mixer_elem,
		&::QSnd2::ASMI_Proxies_Group2::alsa_callback_mixer_elem );
}

ASMI_Proxies_Group2::~ASMI_Proxies_Group2 ( )
{
}

int
ASMI_Proxies_Group2::alsa_callback_mixer_elem (
	::snd_mixer_elem_t * elem_n,
	unsigned int mask_n )
{
	int res ( 0 );

	::QSnd2::ASMI_Proxies_Group2 * pgrp (
		reinterpret_cast < ::QSnd2::ASMI_Proxies_Group2 * > (
		snd_mixer_elem_get_callback_private ( elem_n ) ) );

	{
		const unsigned int change_mask (
			SND_CTL_EVENT_MASK_INFO |
			SND_CTL_EVENT_MASK_ADD |
			SND_CTL_EVENT_MASK_TLV );

		if ( ( mask_n == SND_CTL_EVENT_MASK_REMOVE ) ||
			( ( mask_n & change_mask ) != 0 ) )
		{
			// TODO:
			//pgrp->signalize_element_changed();
		} else if ( ( mask_n & SND_CTL_EVENT_MASK_VALUE ) != 0 ) {
			//pgrp->update_values_mark();
		} else {
			// Unusual mask
			::std::cerr << "Mixer_Simple_Elem::alsa_callback_mixer_elem: ";
			::std::cerr << "Unknown mask ( " << mask_n << " )\n";
			res = -1;
		}
	}

	return res;
}



ASMI_Controls::ASMI_Controls ( ) :
_is_open ( false )
{
}

ASMI_Controls::~ASMI_Controls ( )
{
	close();
}

void
ASMI_Controls::close ( )
{
	clear_pollfds();
	clear_groups();
	destroy_control_groups();

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
ASMI_Controls::open (
	const QString & device_n )
{
	//::std::cout << "ASMI_Controls::open " << device_n.toLocal8Bit().constData() << "\n";

	close();

	_err_func.clear();
	_err_message.clear();

	int err ( 0 );

	if ( device_n.isEmpty() ) {
		_err_func = "open";
		// TODO:
		//_err_message = tr ( "Empty device name" );
		err = -1;
	}

	if ( err == 0 ) {
		err = ::snd_mixer_open ( &_snd_mixer, 0 );
		if ( err < 0 ) {
			_err_func = "snd_mixer_open";
			_err_message = ::QSnd::snd_error_qstring ( err );
		}
	}

	if ( err == 0 ) {
		err = ::snd_mixer_selem_register ( _snd_mixer, NULL, NULL );
		if ( err < 0 ) {
			_err_func = "snd_mixer_selem_register";
			_err_message = ::QSnd::snd_error_qstring ( err );
			close();
		}
	}

	// Open control handle
	if ( err == 0 ) {
		err = ::snd_hctl_open (
			&_snd_hctl,
			device_n.toLocal8Bit().constData(),
			SND_CTL_NONBLOCK );
		if ( err < 0 ) {
			_err_func = "snd_hctl_open";
			_err_message = ::QSnd::snd_error_qstring ( err );
			close();
		}
	}

	// Attach hctl handle to mixer
	if ( err == 0 ) {
		err = ::snd_mixer_attach_hctl ( _snd_mixer, _snd_hctl );
		if ( err < 0 ) {
			_err_func = "snd_mixer_attach_hctl";
			_err_message = ::QSnd::snd_error_qstring ( err );
			close();
		}
	}

	// Load mixer
	if ( err == 0 ) {
		err = ::snd_mixer_load ( _snd_mixer );
		if ( err < 0 ) {
			_err_func = "snd_mixer_load";
			_err_message = ::QSnd::snd_error_qstring ( err );
			close();
		}
	}

	// Install alsa callback
	if ( err == 0 ) {
		::snd_mixer_set_callback_private ( _snd_mixer, this );
		::snd_mixer_set_callback ( _snd_mixer,
			&::QSnd2::ASMI_Controls::alsa_callback );

		// Create mixer elements
		err = create_control_groups();
		if ( err < 0 ) {
			close();
		}
	}

	// Create socket notifiers
	if ( err == 0 ) {
		err = load_pollfds();
		if ( err < 0 ) {
			close();
		}
	}

	return ( err != 0 );
}

void
ASMI_Controls::destroy_control_groups ( )
{
	if ( _cp_groups.size() > 0 ) {
		for ( int ii=0; ii < _cp_groups.size(); ++ii ) {
			delete _cp_groups[ii];
		}
		_cp_groups.clear();
	}
}

int
ASMI_Controls::create_control_groups ( )
{
	int res ( 0 );

	// Create and Mixer_Simple_Elem objects
	{
		::snd_mixer_elem_t * snd_elem;
		snd_elem = ::snd_mixer_first_elem ( _snd_mixer );
		while ( snd_elem != 0 )	{
			_cp_groups.append ( create_control_group ( snd_elem ) );
			snd_elem = ::snd_mixer_elem_next ( snd_elem );
		}
	}

	return res;
}

::QSnd2::ASMI_Proxies_Group2 *
ASMI_Controls::create_control_group (
	::snd_mixer_elem_t * snd_elem_n )
{
	::QSnd2::ASMI_Proxies_Group2 * grp (
		new ::QSnd2::ASMI_Proxies_Group2 ( snd_elem_n ) );

	return grp;
}

int
ASMI_Controls::load_pollfds ( )
{
	//::std::cout << "ASMI_Controls::load_pollfds" << "\n";

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
ASMI_Controls::socket_data (
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
ASMI_Controls::alsa_callback (
	snd_mixer_t * snd_mixer_n,
	unsigned int mask_n,
	snd_mixer_elem_t * )
{
	int res ( 0 );

	::QSnd2::ASMI_Controls * alsa_controls =
		reinterpret_cast < ::QSnd2::ASMI_Controls * > (
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
			::std::cerr << "ASMI_Controls::alsa_callback_mixer: ";
			::std::cerr << "Unknown mask ( " << mask_n << " )\n";
			res = -1;
		}
	}

	return res;
}


} // End of namespace
