/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "controls_model.hpp"

#include "qsnd/alsa.hpp"
#include "qsnd/model_keys.hpp"
#include "qsnd/ctl_format.hpp"
#include "qsnd/controls_database.hpp"
#include <QFont>
#include <QCoreApplication>
#include <iostream>


namespace QSnd
{


Controls_Model::Controls_Model (
	QObject * parent_n ) :
QStandardItemModel ( parent_n ),
_ctl_db ( 0 )
{
}


Controls_Model::~Controls_Model ( )
{
}


void
Controls_Model::set_controls_db (
	::QSnd::Controls_Database * ctl_db_n )
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
		connect ( _ctl_db, SIGNAL ( sig_change_coming() ), this, SLOT ( reload_begin() ) );
		connect ( _ctl_db, SIGNAL ( sig_change_done() ), this, SLOT ( reload_finish() ) );
	}
	reload_finish();
}


const ::QSnd::CTL_Format *
Controls_Model::ctl_format (
	const QModelIndex & idx_n ) const
{
	const ::QSnd::CTL_Format * res ( 0 );
	if ( ( _ctl_db != 0 ) && ( idx_n.isValid() ) ) {
		const QVariant & idata ( data ( idx_n, MKEY_DB_INDEX ) );
		if ( idata.type() == QVariant::UInt ) {
			const unsigned int ctl_idx ( idata.toUInt() );
			if ( ctl_idx < _ctl_db->num_controls() ) {
				res = &_ctl_db->control_format ( ctl_idx );
			}
		}
	}
	return res;
}

void
Controls_Model::ctl_format (
	::QSnd::CTL_Format & ctl_format_n,
	const QModelIndex & idx_n ) const
{
	const ::QSnd::CTL_Format * format (
		ctl_format ( idx_n ) );
	if ( format != 0 ) {
		ctl_format_n = *format;
	} else {
		ctl_format_n.clear();
	}
}

QModelIndex
Controls_Model::ctl_format_index (
	const QString & ctl_addr_n ) const
{
	QModelIndex res;

	if ( _ctl_db != 0 ) {
		const int num_rows ( rowCount() );
		for ( int row=0; row < num_rows; ++row ) {
			QModelIndex midx ( index ( row, 0 ) );
			const QVariant & idata ( data ( midx, MKEY_DB_INDEX ) );
			const unsigned int ctl_idx ( idata.toUInt() );
			if ( ctl_idx < _ctl_db->num_controls() ) {
				if ( _ctl_db->control_format ( ctl_idx ).match ( ctl_addr_n ) ) {
					res = midx;
					break;
				}
			}
		}
	}

	return res;
}


void
Controls_Model::reload ( )
{
	reload_begin();
	reload_finish();
}


void
Controls_Model::reload_begin ( )
{
	beginResetModel();
	removeRows ( 0, rowCount() );
}


void
Controls_Model::reload_finish ( )
{
	load_data();
	endResetModel();
}


void
Controls_Model::load_data ( )
{
	if ( _ctl_db == 0 ) {
		return;
	}
	if ( _ctl_db->num_controls() == 0 ) {
		return;
	}

	for ( unsigned int ii=0; ii != _ctl_db->num_controls(); ++ii ) {
		const ::QSnd::CTL_Format & ctl_format (
			_ctl_db->control_format ( ii ) );
		// Create standard item and append
		QStandardItem * sitem ( new QStandardItem );
		sitem->setText ( ctl_format.ctl_name() );
		sitem->setEditable ( false );
		sitem->setSelectable ( true );
		sitem->setData ( QVariant ( ii ), MKEY_DB_INDEX );
		QString ttip ( ctl_format.ctl_name() );
		QStringList args_l10n;
		if ( ctl_format.num_args() != 0 ) {
			ttip.append ( ":" );
			for ( unsigned int jj=0; jj != ctl_format.num_args(); ++jj ) {
				const ::QSnd::CTL_Format_Argument & arg ( ctl_format.arg ( jj ) );
				if ( jj > 0 ) {
					ttip.append ( "," );
				}
				ttip.append ( arg.arg_name );
				QString str_l10n ( QCoreApplication::translate (
					"ALSA::CTL_Arg_Name", arg.arg_name.toUtf8().constData() ) );
				args_l10n.append ( str_l10n );
			}
		}
		sitem->setData ( QVariant ( args_l10n ), MKEY_L10N_ARGS );
		sitem->setToolTip ( ttip );
		appendRow ( sitem );
	}

	// Sort items
	sort ( 0 );
}


} // End of namespace
