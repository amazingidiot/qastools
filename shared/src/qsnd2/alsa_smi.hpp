//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_qsnd_mixer_hpp__
#define __INC_qsnd_mixer_hpp__

#include "qsnd/alsa.hpp"
#include "controls.hpp"
#include <vector>

namespace QSnd2
{


// Forward declaration
class ASMI_Proxies_Group1_Slider;
class ASMI_Proxies_Group1_Switch;
class ASMI_Proxies_Group1_Enum;


class ASMI_Proxy_Slider :
	public ::QSnd2::Proxy_Slider
{
	// Public methods
	public:

	ASMI_Proxy_Slider (
		::snd_mixer_selem_channel_id_t channel_id_n );

	long
	int_value ( ) const;

	void
	set_int_value (
		long val_n );

	::QSnd2::ASMI_Proxies_Group1_Slider *
	asmi_pgroup ( ) const;


	// Private attributes
	private:

	const ::snd_mixer_selem_channel_id_t _snd_channel_id;
};

class ASMI_Proxies_Group1_Slider :
	public ::QSnd2::Proxies_Group1_Slider
{
	// Public methods
	public:

	ASMI_Proxies_Group1_Slider (
		::snd_mixer_elem_t * elem_n );


	// Generic interface

	void
	int_range (
		::QSnd2::Integer_Pair & range_n ) const;

	void
	db_range (
		::QSnd2::Integer_Pair & range_n ) const;

	long
	db_from_int (
		long intval_n ) const;

	long
	int_from_db (
		long dbval_n,
		int dir_n ) const;


	// Specific interface

	long
	int_value (
		::snd_mixer_selem_channel_id_t channel_id_n ) const;

	void
	set_int_value (
		::snd_mixer_selem_channel_id_t channel_id_n,
		long val_n );


	// Private attributes
	private:

	::snd_mixer_elem_t * _snd_mixer_elem;
};

inline
::QSnd2::ASMI_Proxies_Group1_Slider *
ASMI_Proxy_Slider::asmi_pgroup ( ) const
{
	return static_cast < ::QSnd2::ASMI_Proxies_Group1_Slider * > ( pgroup() );
}


class ASMI_Proxy_Switch :
	public ::QSnd2::Proxy_Switch
{
	// Public methods
	public:

	ASMI_Proxy_Switch (
		::snd_mixer_selem_channel_id_t channel_id_n );

	::QSnd2::ASMI_Proxies_Group1_Switch *
	asmi_pgroup ( ) const;


	// Generic interface

	bool
	switch_state ( ) const;

	void
	set_switch_state (
		bool state_n );


	// Private attributes
	private:

	const ::snd_mixer_selem_channel_id_t _snd_channel_id;
};


class ASMI_Proxies_Group1_Switch :
	public ::QSnd2::Proxies_Group1_Switch
{
	// Public methods
	public:

	ASMI_Proxies_Group1_Switch (
		::snd_mixer_elem_t * elem_n );


	// Specific interface

	bool
	switch_state (
		::snd_mixer_selem_channel_id_t channel_id_n ) const;

	void
	set_switch_state (
		::snd_mixer_selem_channel_id_t channel_id_n,
		bool state_n );


	// Private attributes
	private:

	::snd_mixer_elem_t * _snd_mixer_elem;
};

inline
::QSnd2::ASMI_Proxies_Group1_Switch *
ASMI_Proxy_Switch::asmi_pgroup ( ) const
{
	return static_cast < ::QSnd2::ASMI_Proxies_Group1_Switch * > ( pgroup() );
}



class ASMI_Proxies_Group3 :
	public ::QSnd2::Proxies_Group3
{
	// Public methods
	public:

	ASMI_Proxies_Group3 (
		::snd_mixer_elem_t * elem_n );

	~ASMI_Proxies_Group3 ( );


	// Alsa callbacks

	static
	int
	alsa_callback_mixer_elem (
		::snd_mixer_elem_t * elem_n,
		unsigned int mask_n );


	// Private methods
	private:

	void
	create_playback_group ( );

	void
	create_capture_group ( );


	// Private attributes
	private:

	::snd_mixer_elem_t * _snd_mixer_elem;
};


/// @brief ALSA simple mixer interface mixer controls
///
class ASMI_Controls :
	public ::QSnd2::Controls
{
	// Public methods
	public:

	ASMI_Controls ( );

	~ASMI_Controls ( );

	bool
	open (
		const QString & device_n );

	void
	close ( );

	bool
	is_open ( ) const;

	// Error strings / codes

	const QString &
	err_func ( ) const;

	const QString &
	err_message ( ) const;

	// Alsa structs

	// TODO: remove
	snd_hctl_t *
	snd_hctl ( );

	// TODO: remove
	snd_mixer_t *
	snd_mixer ( );


	// Socket reading

	void
	socket_data (
		int socket_id );

	// Alsa callback

	static
	int
	alsa_callback (
		::snd_mixer_t * snd_mixer_n,
		unsigned int mask_n,
		::snd_mixer_elem_t * );


	// Private methods
	private:

	int
	create_control_groups ( );

	int
	load_pollfds ( );


	// Private attributes
	private:

	::snd_hctl_t * _snd_hctl;
	::snd_mixer_t * _snd_mixer;

	QString _err_func;
	QString _err_message;

	bool _is_open;
};

inline
bool
ASMI_Controls::is_open ( ) const
{
	return _is_open;
}

inline
const QString &
ASMI_Controls::err_func ( ) const
{
	return _err_func;
}

inline
const QString &
ASMI_Controls::err_message ( ) const
{
	return _err_message;
}

inline
snd_hctl_t *
ASMI_Controls::snd_hctl ( )
{
	return _snd_hctl;
}

inline
snd_mixer_t *
ASMI_Controls::snd_mixer ( )
{
	return _snd_mixer;
}

} // End of namespace

#endif
