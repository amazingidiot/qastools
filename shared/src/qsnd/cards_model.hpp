//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_qsnd_cards_model_hpp__
#define __INC_qsnd_cards_model_hpp__

#include "qsnd/card_info.hpp"
#include <QStandardItemModel>

namespace QSnd
{


/// @brief Cards_Model
///
class Cards_Model :
	public QStandardItemModel
{
	Q_OBJECT;

	// Public types
	public:

	typedef QList < ::QSnd::Card_Info > Card_Infos;


	// Public methods
	public:

	Cards_Model (
		QObject * parent_n = 0 );

	~Cards_Model ( );


	unsigned int
	num_cards ( ) const;

	const ::QSnd::Card_Info &
	card_info (
		unsigned int index_n ) const;

	const ::QSnd::Card_Info *
	card_info_by_card_id (
		unsigned int id_n );


	// Control definition access

	const ::QSnd::Card_Info *
	card_info_by_model_index (
		const QModelIndex & idx_n ) const;

	QModelIndex
	model_index_by_card_id (
		const QString & id_str_n ) const;


	// Public slots
	public slots:

	void
	reload ( );


	// Protected methods
	protected:

	void
	load_cards (
		Card_Infos & card_infos_n );


	// Private attributes
	private:

	Card_Infos _card_infos;
};


inline
unsigned int
Cards_Model::num_cards ( ) const
{
	return _card_infos.size();
}

inline
const ::QSnd::Card_Info &
Cards_Model::card_info (
	unsigned int index_n ) const
{
	return _card_infos[index_n];
}


} // End of namespace


#endif
