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

#ifndef __INC_qsnd_cards_model_hpp__
#define __INC_qsnd_cards_model_hpp__

#include <QStandardItemModel>


// Forward declaration
namespace QSnd {
	class Card_Info;
	class Controls_DB;
}


namespace QSnd
{


/// @brief Cards_Model
///
class Cards_Model :
	public QStandardItemModel
{
	Q_OBJECT;


	// Public methods
	public:

	Cards_Model (
		QObject * parent_n = 0 );

	~Cards_Model ( );


	// Controls database

	const ::QSnd::Controls_DB *
	controls_db ( ) const;

	void
	set_controls_db (
		const ::QSnd::Controls_DB * ctl_db_n );


	// Control definition access

	const ::QSnd::Card_Info *
	card_info (
		const QModelIndex & idx_n ) const;

	QModelIndex
	card_info_index (
		const QString & str_n ) const;


	// Public slots
	public slots:

	void
	reload ( );


	// Protected slots
	protected slots:

	void
	reload_begin ( );

	void
	reload_finish ( );


	// Protected methods
	protected:

	void
	load_data ( );


	// Private attributes
	private:

	const ::QSnd::Controls_DB * _ctl_db;
};


inline
const ::QSnd::Controls_DB *
Cards_Model::controls_db ( ) const
{
	return _ctl_db;
}


} // End of namespace


#endif
