//
// C++ Interface:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INC_qsnd_ctl_info_hpp__
#define __INC_qsnd_ctl_info_hpp__

#include <qsnd/alsa.hpp>
#include <qsnd/pcm_device_info.hpp>
#include <qsnd/card_info.hpp>

#include <QObject>
#include <QString>
#include <QList>


namespace QSnd
{


/// @brief CTL_Info
///
class CTL_Info :
	public QObject
{
	Q_OBJECT

	// Public methods
	public:

	CTL_Info ( );

	~CTL_Info ( );


	// Open / close

	int
	open (
		const QString & plug_str_n );

	bool
	is_open ( ) const;

	void
	close ( );

	const QString &
	ctl_name ( ) const;


	// Error strings / codes

	const QString &
	err_func ( ) const;

	const QString &
	err_message ( ) const;


	// Card info

	const ::QSnd::Card_Info *
	card_info ( ) const;


	// Device info

	unsigned int
	num_devices ( ) const;

	const ::QSnd::PCM_Device_Info *
	device_info (
		unsigned int idx_n ) const;


	void
	request_reload ( );


	// Event handling

	bool
	event (
		QEvent * event_n );


	// Alsa callbacks

	static
	int
	alsa_callback_hctl_info (
		snd_hctl_t * snd_hctl_n,
		unsigned int mask_n,
		snd_hctl_elem_t * snd_hctl_elem_n );


	// Signals
	signals:

	void
	sig_reload_request ( );


	// Protected methods
	protected:

	int
	acquire_devices_info ( );


	// Private attributes
	private:

	QString _ctl_name;
	::QSnd::Card_Info _card_info;
	QList < ::QSnd::PCM_Device_Info * > _devices_info;
	snd_hctl_t * _snd_hctl_handle;
	bool _reload_requested;

	// Strings
	QString _err_func;
	QString _err_message;
};


inline
const QString &
CTL_Info::ctl_name ( ) const
{
	return _ctl_name;
}


inline
const ::QSnd::Card_Info *
CTL_Info::card_info ( ) const
{
	return &_card_info;
}


inline
bool
CTL_Info::is_open ( ) const
{
	return ( _snd_hctl_handle != 0 );
}


inline
const QString &
CTL_Info::err_func ( ) const
{
	return _err_func;
}


inline
const QString &
CTL_Info::err_message ( ) const
{
	return _err_message;
}


inline
unsigned int
CTL_Info::num_devices ( ) const
{
	return _devices_info.size();
}


inline
const ::QSnd::PCM_Device_Info *
CTL_Info::device_info (
	unsigned int idx_n ) const
{
	return _devices_info[idx_n];
}


} // End of namespace

#endif
