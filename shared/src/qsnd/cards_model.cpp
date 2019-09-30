/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "cards_model.hpp"

#include "qsnd/alsa.hpp"
#include "qsnd/udev_device_lookout.hpp"
#include "qsnd/model_keys.hpp"
#include <iostream>


namespace QSnd
{


Cards_Model::Cards_Model (
	QObject * parent_n ) :
QStandardItemModel ( parent_n )
{
	reload();

	// Device lookout
	{
		::QSnd::UDev_Device_Lookout * lookout (
			new ::QSnd::UDev_Device_Lookout ( this ) );
		connect ( lookout, SIGNAL ( sig_change() ),
			this, SLOT ( reload() ) );
	}
}

Cards_Model::~Cards_Model ( )
{
}

const ::QSnd::Card_Info *
Cards_Model::card_info_by_card_id (
	unsigned int id_n )
{
	const ::QSnd::Card_Info * res ( 0 );
	{
		const unsigned int num ( _card_infos.size() );
		for ( unsigned int ii=0; ii != num; ++ii ) {
			const ::QSnd::Card_Info & cinfo ( _card_infos[ii] );
			if ( (unsigned int)cinfo.card_index() == id_n ) {
				res = &cinfo;
				break;
			}
		}
	}
	return res;
}

const ::QSnd::Card_Info *
Cards_Model::card_info_by_model_index (
	const QModelIndex & idx_n ) const
{
	const ::QSnd::Card_Info * res ( 0 );
	if ( idx_n.isValid() && !idx_n.parent().isValid() ) {
		if ( ( idx_n.column() == 0 ) &&
			( idx_n.row() < _card_infos.size() ) )
		{
			res = &_card_infos[idx_n.row()];
		}
	}
	return res;
}

QModelIndex
Cards_Model::model_index_by_card_id (
	const QString & id_str_n ) const
{
	QModelIndex res;

	bool found ( false );
	bool is_int ( false );
	int int_val ( id_str_n.toInt ( &is_int) );
	for ( int ii=0; ii != _card_infos.size(); ++ii ) {
		const ::QSnd::Card_Info & cinfo ( _card_infos[ii] );
		if ( is_int ) {
			if ( cinfo.card_index() == int_val ) {
				found = true;
			}
		} else {
			if ( cinfo.card_id() == id_str_n ) {
				found = true;
			}
		}
		if ( found ) {
			res = QModelIndex ( index ( ii, 0 ) );
			break;
		}
	}

	return res;
}

void
Cards_Model::reload ( )
{
	Card_Infos cards;
	load_cards ( cards );

	unsigned int index_src ( 0 );
	unsigned int index_dst ( 0 );
	while ( ( index_src != (unsigned int)cards.size() ) ||
		( index_dst != (unsigned int)_card_infos.size() ) )
	{
		bool dst_item_remove ( false );
		bool src_item_insert ( false );
		bool src_dst_matching ( false );
		if ( index_src == (unsigned int)cards.size() ) {
			dst_item_remove = true;
		} else {
			if ( index_dst == (unsigned int)_card_infos.size() ) {
				src_item_insert = true;
			} else {
				const ::QSnd::Card_Info & cinfo_src ( cards[index_src] );
				const ::QSnd::Card_Info & cinfo_dst ( _card_infos[index_dst] );
				if ( cinfo_src.card_index() == cinfo_dst.card_index() ) {
					if ( cinfo_src == cinfo_dst ) {
						src_dst_matching = true;
					} else {
						dst_item_remove = true;
						src_item_insert = true;
					}
				} else {
					if ( cinfo_src.card_index() > cinfo_dst.card_index() ) {
						dst_item_remove = true;
					} else {
						src_item_insert = true;
					}
				}
			}
		}
		if ( src_dst_matching ) {
			++index_src;
			++index_dst;
		} else {
			if ( dst_item_remove ) {
				_card_infos.removeAt ( index_dst );
				QList < QStandardItem * > lst ( takeRow ( index_dst ) );
				for ( int ii=0; ii != lst.size(); ++ii ) {
					delete lst[ii];
				}
			}
			if ( src_item_insert ) {
				const ::QSnd::Card_Info & cinfo ( cards[index_src] );
				_card_infos.insert ( index_dst, cinfo );
				{
					// Create standard item and append
					QStandardItem * sitem ( new QStandardItem );
					sitem->setText ( cinfo.card_name() );
					sitem->setEditable ( false );
					sitem->setSelectable ( true );
					sitem->setData ( QVariant ( cinfo.card_index() ), MKEY_CARD_INDEX );
					sitem->setData ( QVariant ( cinfo.card_name() ), MKEY_CARD_NAME );
					sitem->setData ( QVariant ( cinfo.card_mixer_name() ), MKEY_CARD_MIXER_NAME );
					insertRow ( index_dst, sitem );
				}
				++index_src;
				++index_dst;
			}
		}
	}
}

void
Cards_Model::load_cards (
	Card_Infos & card_infos_n )
{
	card_infos_n.clear();

	int card_idx = -1;
	while (	true ) {
		if ( ::snd_card_next ( &card_idx ) != 0 ) {
			break;
		}
		if ( card_idx < 0 ) {
			break;
		}
		card_infos_n.append ( ::QSnd::Card_Info ( card_idx ) );
	}
}


} // End of namespace
