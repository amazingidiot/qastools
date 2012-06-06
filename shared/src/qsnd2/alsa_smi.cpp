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
	return asmi_pgroup()->int_value ( _snd_channel_id );
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
	if ( has_feature ( ::QSnd2::FFLAG_CAPTURE ) ) {
		::snd_mixer_selem_get_capture_volume_range (
			_snd_mixer_elem, &range_n[0], &range_n[1] );
	} else {
		::snd_mixer_selem_get_playback_volume_range (
			_snd_mixer_elem, &range_n[0], &range_n[1] );
	}
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
		if ( has_feature ( ::QSnd2::FFLAG_CAPTURE ) ) {
			err = snd_mixer_selem_get_capture_dB_range (
				_snd_mixer_elem, &range_n[0], &range_n[1] );
		} else {
			err = snd_mixer_selem_get_playback_dB_range (
				_snd_mixer_elem, &range_n[0], &range_n[1] );
		}
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
	::snd_mixer_selem_channel_id_t channel_id_n,
	long val_n )
{
	if ( has_feature ( ::QSnd2::FFLAG_CAPTURE ) ) {
		int err = snd_mixer_selem_set_capture_volume (
			_snd_mixer_elem, channel_id_n, val_n );
		if ( err < 0 ) {
			::QSnd::print_alsa_error ( "snd_mixer_selem_set_capture_volume", err );
		}
	} else {
		int err = snd_mixer_selem_set_playback_volume (
			_snd_mixer_elem, channel_id_n, val_n );
		if ( err < 0 ) {
			::QSnd::print_alsa_error ( "snd_mixer_selem_set_playback_volume", err );
		}
	}
}



ASMI_Proxy_Switch::ASMI_Proxy_Switch (
	::snd_mixer_selem_channel_id_t channel_id_n ) :
_snd_channel_id ( channel_id_n )
{
}

bool
ASMI_Proxy_Switch::switch_state ( ) const
{
	return asmi_pgroup()->switch_state ( _snd_channel_id );
}

void
ASMI_Proxy_Switch::set_switch_state (
	bool state_n )
{
	asmi_pgroup()->set_switch_state ( _snd_channel_id, state_n );
}



ASMI_Proxies_Group1_Switch::ASMI_Proxies_Group1_Switch (
	::snd_mixer_elem_t * elem_n ) :
_snd_mixer_elem ( elem_n )
{
}

bool
ASMI_Proxies_Group1_Switch::switch_state (
	::snd_mixer_selem_channel_id_t channel_id_n ) const
{
	int val ( 0 );

	if ( has_feature ( ::QSnd2::FFLAG_CAPTURE ) ) {
		int err ( snd_mixer_selem_get_capture_switch (
			_snd_mixer_elem, channel_id_n, &val ) );
		if ( err < 0 ) {
			::QSnd::print_alsa_error ( "snd_mixer_selem_get_capture_switch", err );
		}
	} else {
		int err ( snd_mixer_selem_get_playback_switch (
			_snd_mixer_elem, channel_id_n, &val ) );
		if ( err < 0 ) {
			::QSnd::print_alsa_error ( "snd_mixer_selem_get_playback_switch", err );
		}
	}

	return ( val != 0 );
}

void
ASMI_Proxies_Group1_Switch::set_switch_state (
	::snd_mixer_selem_channel_id_t channel_id_n,
	bool state_n )
{
	if ( has_feature ( ::QSnd2::FFLAG_CAPTURE ) ) {
		int err = snd_mixer_selem_set_capture_switch (
			_snd_mixer_elem, channel_id_n, state_n );
		if ( err < 0 ) {
			::QSnd::print_alsa_error ( "snd_mixer_selem_set_capture_switch", err );
		}
	} else {
		int err = snd_mixer_selem_set_playback_switch (
			_snd_mixer_elem, channel_id_n, state_n );
		if ( err < 0 ) {
			::QSnd::print_alsa_error ( "snd_mixer_selem_set_playback_switch", err );
		}
	}
}



ASMI_Proxies_Group2::ASMI_Proxies_Group2 (
	::snd_mixer_elem_t * elem_n ) :
_snd_mixer_elem ( elem_n )
{
	_str_name = QString ( snd_mixer_selem_get_name ( _snd_mixer_elem ) );
	// TODO:
	_str_name_l10n = _str_name;
}

bool
ASMI_Proxies_Group2::string_val (
	QString & str_n,
	unsigned int key_n ) const
{
	bool res ( true );
	switch ( key_n ) {
		case ::QSnd2::SK_NAME:
			str_n = _str_name;
			break;
		case ::QSnd2::SK_NAME_L10N:
			str_n = _str_name_l10n;
			break;
		default:
			res = false;
	}

	return res;
}



ASMI_Proxies_Group3::ASMI_Proxies_Group3 (
	::snd_mixer_elem_t * elem_n ) :
_snd_mixer_elem ( elem_n )
{
	create_playback_group();
	create_capture_group();

	// Install Alsa callbacks
	snd_mixer_elem_set_callback_private ( _snd_mixer_elem, this );
	snd_mixer_elem_set_callback ( _snd_mixer_elem,
		&::QSnd2::ASMI_Proxies_Group3::alsa_callback_mixer_elem );
}

ASMI_Proxies_Group3::~ASMI_Proxies_Group3 ( )
{
	// Remove Alsa callbacks
	snd_mixer_elem_set_callback ( _snd_mixer_elem, 0 );
	snd_mixer_elem_set_callback_private ( _snd_mixer_elem, 0 );
}

void
ASMI_Proxies_Group3::create_playback_group ( )
{
	QScopedPointer < ::QSnd2::ASMI_Proxies_Group2 > grp2 (
		new ::QSnd2::ASMI_Proxies_Group2 ( _snd_mixer_elem ) );

	// Playback sliders
	{
		QScopedPointer < ::QSnd2::ASMI_Proxies_Group1_Slider > grp_slider (
			new ::QSnd2::ASMI_Proxies_Group1_Slider ( _snd_mixer_elem ) );
		grp_slider->set_feature ( ::QSnd2::FFLAG_PLAYBACK );
		if ( ::snd_mixer_selem_has_playback_volume ( _snd_mixer_elem ) > 0 ) {
			// Detect available channels
			if ( ::snd_mixer_selem_has_playback_volume_joined ( _snd_mixer_elem ) > 0 ) {
				::QSnd2::ASMI_Proxy_Slider * slider_proxy (
					new ::QSnd2::ASMI_Proxy_Slider ( SND_MIXER_SCHN_MONO ) );
				grp_slider->append_proxy ( slider_proxy );
			} else {
				for ( int ii=0; ii <= SND_MIXER_SCHN_LAST; ++ii ) {
					::snd_mixer_selem_channel_id_t channel_id (
						static_cast < ::snd_mixer_selem_channel_id_t > ( ii ) );
					if ( ::snd_mixer_selem_has_playback_channel ( _snd_mixer_elem, channel_id ) > 0 ) {
						::QSnd2::ASMI_Proxy_Slider * slider_proxy (
							new ::QSnd2::ASMI_Proxy_Slider ( channel_id ) );
						grp_slider->append_proxy ( slider_proxy );
					}
				}
			}

			// Detect decibel availability
			{
				::QSnd2::Integer_Pair vals ( 0, 0 );
				int err ( ::snd_mixer_selem_get_playback_dB_range (
					_snd_mixer_elem, &vals[0], &vals[1] ) );
				if ( ( err >= 0 ) && ( vals[0] != vals[1] ) ) {
					grp_slider->set_feature ( ::QSnd2::FFLAG_DECIBEL );
				}
			}
		}
		if ( grp_slider->num_proxies() != 0 ) {
			grp2->append_group ( grp_slider.take() );
		}
	}

	{
		// Playback switches
		QScopedPointer < ::QSnd2::ASMI_Proxies_Group1_Switch > grp_switch (
			new ::QSnd2::ASMI_Proxies_Group1_Switch ( _snd_mixer_elem ) );
		grp_switch->set_feature ( ::QSnd2::FFLAG_PLAYBACK );
		if ( ::snd_mixer_selem_has_playback_switch ( _snd_mixer_elem ) > 0 ) {
			// Detect available channels
			if ( ::snd_mixer_selem_has_playback_switch_joined ( _snd_mixer_elem ) > 0 ) {
				// Single joined switch for all channels
				::QSnd2::ASMI_Proxy_Switch * switch_proxy (
					new ::QSnd2::ASMI_Proxy_Switch ( SND_MIXER_SCHN_MONO ) );
				grp_switch->append_proxy ( switch_proxy );
			} else {
				for ( int ii=0; ii <= SND_MIXER_SCHN_LAST; ++ii ) {
					::snd_mixer_selem_channel_id_t channel_id (
						static_cast < ::snd_mixer_selem_channel_id_t > ( ii ) );
					if ( ::snd_mixer_selem_has_playback_channel ( _snd_mixer_elem, channel_id ) > 0 ) {
						::QSnd2::ASMI_Proxy_Switch * switch_proxy (
							new ::QSnd2::ASMI_Proxy_Switch ( channel_id ) );
						grp_switch->append_proxy ( switch_proxy );
					}
				}
			}
		}
		if ( grp_switch->num_proxies() != 0 ) {
			grp2->append_group ( grp_switch.take() );
		}
	}

	if ( grp2->num_groups() != 0 ) {
		append_group ( grp2.take() );
	}
}

void
ASMI_Proxies_Group3::create_capture_group ( )
{
	QScopedPointer < ::QSnd2::ASMI_Proxies_Group2 > grp2 (
		new ::QSnd2::ASMI_Proxies_Group2 ( _snd_mixer_elem ) );

	// Capture sliders
	{
		QScopedPointer < ::QSnd2::ASMI_Proxies_Group1_Slider > grp_slider (
			new ::QSnd2::ASMI_Proxies_Group1_Slider ( _snd_mixer_elem ) );
		grp_slider->set_feature ( ::QSnd2::FFLAG_CAPTURE );
		if ( ::snd_mixer_selem_has_capture_volume ( _snd_mixer_elem ) > 0 ) {
			// Detect available channels
			if ( ::snd_mixer_selem_has_capture_volume_joined ( _snd_mixer_elem ) > 0 ) {
				::QSnd2::ASMI_Proxy_Slider * slider_proxy (
					new ::QSnd2::ASMI_Proxy_Slider ( SND_MIXER_SCHN_MONO ) );
				grp_slider->append_proxy ( slider_proxy );
			} else {
				for ( int ii=0; ii <= SND_MIXER_SCHN_LAST; ++ii ) {
					::snd_mixer_selem_channel_id_t channel_id (
						static_cast < ::snd_mixer_selem_channel_id_t > ( ii ) );
					if ( ::snd_mixer_selem_has_capture_channel ( _snd_mixer_elem, channel_id ) > 0 ) {
						::QSnd2::ASMI_Proxy_Slider * slider_proxy (
							new ::QSnd2::ASMI_Proxy_Slider ( channel_id ) );
						grp_slider->append_proxy ( slider_proxy );
					}
				}
			}

			// Detect decibel availability
			{
				::QSnd2::Integer_Pair vals ( 0, 0 );
				int err ( ::snd_mixer_selem_get_capture_dB_range (
					_snd_mixer_elem, &vals[0], &vals[1] ) );
				if ( ( err >= 0 ) && ( vals[0] != vals[1] ) ) {
					grp_slider->set_feature ( ::QSnd2::FFLAG_DECIBEL );
				}
			}
		}
		if ( grp_slider->num_proxies() != 0 ) {
			grp2->append_group ( grp_slider.take() );
		}
	}

	// Capture switches
	{
		QScopedPointer < ::QSnd2::ASMI_Proxies_Group1_Switch > grp_switch (
			new ::QSnd2::ASMI_Proxies_Group1_Switch ( _snd_mixer_elem ) );
		grp_switch->set_feature ( ::QSnd2::FFLAG_CAPTURE );
		if ( ::snd_mixer_selem_has_capture_switch ( _snd_mixer_elem ) > 0 ) {
			// Detect available channels
			if ( ::snd_mixer_selem_has_capture_switch_joined ( _snd_mixer_elem ) > 0 ) {
				// Single joined switch for all channels
				::QSnd2::ASMI_Proxy_Switch * switch_proxy (
					new ::QSnd2::ASMI_Proxy_Switch ( SND_MIXER_SCHN_MONO ) );
				grp_switch->append_proxy ( switch_proxy );
			} else {
				for ( int ii=0; ii <= SND_MIXER_SCHN_LAST; ++ii ) {
					::snd_mixer_selem_channel_id_t channel_id (
						static_cast < ::snd_mixer_selem_channel_id_t > ( ii ) );
					if ( ::snd_mixer_selem_has_capture_channel ( _snd_mixer_elem, channel_id ) > 0 ) {
						::QSnd2::ASMI_Proxy_Switch * switch_proxy (
							new ::QSnd2::ASMI_Proxy_Switch ( channel_id ) );
						grp_switch->append_proxy ( switch_proxy );
					}
				}
			}
		}
		if ( grp_switch->num_proxies() != 0 ) {
			grp2->append_group ( grp_switch.take() );
		}
	}

	if ( grp2->num_groups() != 0 ) {
		append_group ( grp2.take() );
	}
}

int
ASMI_Proxies_Group3::alsa_callback_mixer_elem (
	::snd_mixer_elem_t * elem_n,
	unsigned int mask_n )
{
	int res ( 0 );

	::QSnd2::ASMI_Proxies_Group3 * pgrp (
		reinterpret_cast < ::QSnd2::ASMI_Proxies_Group3 * > (
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
			pgrp->notify_proxies_value_changed();
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
_snd_hctl ( 0 ),
_snd_mixer ( 0 )
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

int
ASMI_Controls::create_control_groups ( )
{
	int res ( 0 );

	QScopedPointer < ::QSnd2::Proxies_Group4 > grp4 (
		new ::QSnd2::Proxies_Group4 );

	// Create and Mixer_Simple_Elem objects
	{
		::snd_mixer_elem_t * snd_elem;
		snd_elem = ::snd_mixer_first_elem ( _snd_mixer );
		while ( snd_elem != 0 )	{
			QScopedPointer < ::QSnd2::ASMI_Proxies_Group3 > grp3 (
				new ::QSnd2::ASMI_Proxies_Group3 ( snd_elem ) );
			if ( grp3->num_groups() != 0 ) {
				grp4->append_group ( grp3.take() );
			}
			snd_elem = ::snd_mixer_elem_next ( snd_elem );
		}
	}

	if ( grp4->num_groups() != 0 ) {
		append_group ( grp4.take() );
	} else {
		grp4.reset();
		_err_func = "create_control_groups";
		// TODO:
		//_err_message = tr ( "Mixer device has no controls" );
		res = -1;
	}

	return res;
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
