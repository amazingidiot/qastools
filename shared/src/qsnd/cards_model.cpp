//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "cards_model.hpp"

#include "qsnd/alsa.hpp"
#include "qsnd/model_keys.hpp"
#include "qsnd/card_info.hpp"
#include "qsnd/controls_db.hpp"
#include <QFont>
#include <iostream>


namespace QSnd
{


Cards_Model::Cards_Model (
	QObject * parent_n ) :
QStandardItemModel ( parent_n ),
_ctl_db ( 0 )
{
}


Cards_Model::~Cards_Model ( )
{
}


void
Cards_Model::set_controls_db (
	const ::QSnd::Controls_DB * ctl_db_n )
{
	if ( _ctl_db == ctl_db_n ) {
		return;
	}

	reload_begin();
	if ( _ctl_db != 0 ) {
		disconnect ( _ctl_db, 0, this, 0 );
	}
	_ctl_db = ctl_db_n;
	if ( _ctl_db != 0 ) {
		connect ( _ctl_db, SIGNAL ( sig_change_comming() ), this, SLOT ( reload_begin() ) );
		connect ( _ctl_db, SIGNAL ( sig_change_done() ), this, SLOT ( reload_finish() ) );
	}
	reload_finish();
}


const ::QSnd::Card_Info *
Cards_Model::card_info (
	const QModelIndex & idx_n ) const
{
	const ::QSnd::Card_Info * res ( 0 );

	if ( ( _ctl_db != 0 ) && ( idx_n.isValid() ) ) {
		const QVariant & idata ( data ( idx_n, MKEY_DB_INDEX ) );
		if ( idata.type() == QVariant::UInt ) {
			const unsigned int ctl_idx ( idata.toUInt() );
			if ( ctl_idx < _ctl_db->num_cards() ) {
				res = _ctl_db->card_info ( ctl_idx );
			}
		}
	}

	return res;
}


QModelIndex
Cards_Model::card_info_index (
	const QString & str_n ) const
{
	QModelIndex res;

	bool is_int ( false );
	int int_val ( str_n.toInt ( &is_int) );
	for ( int ii=0; ii < rowCount(); ++ii ) {
		QModelIndex midx ( index ( ii, 0 ) );
		const ::QSnd::Card_Info * cinfo ( card_info ( index ( ii, 0 ) ) );
		if ( is_int ) {
			if ( cinfo->card_index() == int_val ) {
				res = midx;
				break;
			}
		} else {
			if ( cinfo->card_id() == str_n ) {
				res = midx;
				break;
			}
		}
	}

	return res;
}


void
Cards_Model::reload ( )
{
	reload_begin();
	reload_finish();
}


void
Cards_Model::reload_begin ( )
{
	beginResetModel();
	removeRows ( 0, rowCount() );
}


void
Cards_Model::reload_finish ( )
{
	load_data();
	endResetModel();
}


void
Cards_Model::load_data ( )
{
	if ( _ctl_db == 0 ) {
		return;
	}
	if ( _ctl_db->num_cards() == 0 ) {
		return;
	}

	for ( unsigned int ii=0; ii < _ctl_db->num_cards(); ++ii ) {
		const ::QSnd::Card_Info * cinfo ( _ctl_db->card_info ( ii ) );
		// Create standard item and append
		QStandardItem * sitem ( new QStandardItem );
		sitem->setText ( cinfo->card_name() );
		sitem->setEditable ( false );
		sitem->setSelectable ( true );
		sitem->setData ( QVariant ( ii ), MKEY_DB_INDEX );
		sitem->setData ( QVariant ( cinfo->card_index() ), MKEY_CARD_INDEX );
		sitem->setData ( QVariant ( cinfo->card_name() ), MKEY_CARD_NAME );
		sitem->setData ( QVariant ( cinfo->card_mixer_name() ), MKEY_CARD_MIXER_NAME );
		appendRow ( sitem );
	}
}


} // End of namespace
