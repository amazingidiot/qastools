//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_qsnd_controls_db_hpp__
#define __INC_qsnd_controls_db_hpp__

#include <qsnd/ctl_format.hpp>
#include <QObject>
#include <QList>


// Forward declaration
namespace QSnd {
	class Card_Info;
}


namespace QSnd
{


/// @brief Controls_DB
///
class Controls_DB :
	public QObject
{
	Q_OBJECT;


	// Public methods
	public:

	Controls_DB ( );

	~Controls_DB ( );


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

	const ::QSnd::Card_Info *
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
	QList < ::QSnd::Card_Info * > _card_infos;
};


inline
unsigned int
Controls_DB::num_controls ( ) const
{
	return _ctl_formats.size();
}


inline
const ::QSnd::CTL_Format &
Controls_DB::control_def (
	unsigned int index_n ) const
{
	return _ctl_formats[index_n];
}


inline
unsigned int
Controls_DB::num_cards ( ) const
{
	return _card_infos.size();
}


inline
const ::QSnd::Card_Info *
Controls_DB::card_info (
	unsigned int index_n ) const
{
	return _card_infos[index_n];
}


} // End of namespace


#endif
