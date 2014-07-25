//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_qsnd_controls_db_hpp__
#define __INC_qsnd_controls_db_hpp__

#include "qsnd/ctl_format.hpp"
#include "qsnd/card_info.hpp"
#include <QObject>
#include <QList>

namespace QSnd
{


/// @brief Controls_Database
///
class Controls_Database :
	public QObject
{
	Q_OBJECT;


	// Public methods
	public:

	Controls_Database ( );

	~Controls_Database ( );


	// Control plugins

	unsigned int
	num_controls ( ) const;

	const ::QSnd::CTL_Format &
	control_def (
		unsigned int index_n ) const;

	const ::QSnd::CTL_Format *
	find_control_def (
		const QString & ctl_name_n ) const;


	// Cards access

	unsigned int
	num_cards ( ) const;

	const ::QSnd::Card_Info &
	card_info (
		unsigned int index_n ) const;



	// Signals
	signals:

	void
	sig_change_comming ( );

	void
	sig_change_done ( );


	// Public slots
	public slots:

	void
	reload_silent ( );

	void
	reload ( );


	// Protected methods
	protected:

	void
	clear_data ( );

	void
	load_plugins ( );

	void
	load_cards ( );


	// Private attributes
	private:

	QList < ::QSnd::CTL_Format > _ctl_formats;
	QList < ::QSnd::Card_Info > _card_infos;
};


inline
unsigned int
Controls_Database::num_controls ( ) const
{
	return _ctl_formats.size();
}


inline
const ::QSnd::CTL_Format &
Controls_Database::control_def (
	unsigned int index_n ) const
{
	return _ctl_formats[index_n];
}


inline
unsigned int
Controls_Database::num_cards ( ) const
{
	return _card_infos.size();
}


inline
const ::QSnd::Card_Info &
Controls_Database::card_info (
	unsigned int index_n ) const
{
	return _card_infos[index_n];
}


} // End of namespace


#endif
