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


class ASMI_Proxies_Group1_Slider :
	public ::QSnd2::Proxies_Group1_Slider
{
	// Public methods
	public:

};

class ASMI_Proxies_Group2 :
	public ::QSnd2::Proxies_Group2
{
	// Public methods
	public:

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
		snd_mixer_t * snd_mixer_n,
		unsigned int mask_n,
		snd_mixer_elem_t * );


	// Private methods
	private:

	void
	destroy_control_groups ( );

	int
	create_control_groups ( );

	::QSnd2::ASMI_Proxies_Group2 *
	create_control_group (
		::snd_mixer_elem_t * snd_elem_n );

	int
	load_pollfds ( );


	// Private attributes
	private:

	::snd_hctl_t * _snd_hctl;
	::snd_mixer_t * _snd_mixer;

	QList < ::QSnd2::ASMI_Proxies_Group2 * > _cp_groups;

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
