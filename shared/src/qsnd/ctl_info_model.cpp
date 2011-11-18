//
// C++ Implementation:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "ctl_info_model.hpp"

#include <QFont>
#include <QFontMetrics>

#include <iostream>


namespace QSnd
{


CTL_Info_Model::CTL_Info_Model ( ) :
_pcm_info ( 0 )
{
	QFont fnt_emph;
	fnt_emph.setBold ( true );
	QFontMetrics fmet_emph ( fnt_emph );

	_str_card[0] = tr ( "Index" );
	_str_card[1] = tr ( "Id" );
	_str_card[2] = tr ( "Driver" );
	_str_card[3] = tr ( "Name" );
	_str_card[4] = tr ( "Long name" );
	_str_card[5] = tr ( "Mixer name" );
	_str_card[6] = tr ( "Components" );

	_str_dev[0] = "%1";
	_ttip_dev[0] = tr ( "Device index" );

	_str_dev[1] = "%1";
	_ttip_dev[1] = tr ( "Device id" );

	_str_dev_name[0] = tr ( "Id / Name" );
	_ttip_dev_name[1] = tr ( "" );

	_str_dev_name[1] = tr ( "%1 / %2" );
	_ttip_dev_name[1] = _str_dev_name[1]
		.arg ( tr ( "Device id" ) )
		.arg ( tr ( "Device name" ) );

	_str_dev_subdevs[0] = tr ( "Subdevices" );
	_str_dev_subdevs[1] = tr ( "" );

	_str_sdevs_id[0] = tr ( "Playback" );
	_str_sdevs_id[1] = tr ( "Capture" );

	_str_sdevs_val[0] = tr ( "%1 : %2 available" );
	_str_sdevs_val[1] = tr ( "%1 : %2 available" );

	_ttip_sdevs_val[0] = tr ( "Existing : Available" );
	_ttip_sdevs_val[1] = tr ( "Existing : Available" );


	// Init header / columns

	setColumnCount ( 2 );
	setHorizontalHeaderItem ( 0, new QStandardItem ( tr ( "Key" ) ) );
	setHorizontalHeaderItem ( 1, new QStandardItem ( tr ( "Value" ) ) );

	// Init base nodes

	_item_ctl[0] = new QStandardItem;
	_item_ctl[1] = new QStandardItem;

	_item_ctl[0]->setText ( tr ( "Control" ) );
	_item_ctl[0]->setFont ( fnt_emph );
	_item_ctl[0]->setColumnCount ( 2 );
	_item_ctl[0]->setEditable ( false );
	_item_ctl[0]->setSelectable ( true );

	_item_ctl[1]->setToolTip ( tr ( "Control plugin" ) );
	_item_ctl[1]->setEditable ( false );
	_item_ctl[1]->setSelectable ( true );


	// Card info item

	_item_card_info[0] = new QStandardItem;
	_item_card_info[1] = new QStandardItem;

	{
		QString txt (  tr ( "Card" ) );
		_item_card_info[0]->setFont ( fnt_emph );
		_item_card_info[0]->setText ( txt );
	}

	_item_card_info[0]->setColumnCount ( 2 );
	_item_card_info[0]->setEditable ( false );
	_item_card_info[0]->setSelectable ( true );

	_item_card_info[1]->setEditable ( false );
	_item_card_info[1]->setSelectable ( true );


	// Devices item

	_item_devices[0] = new QStandardItem;
	_item_devices[1] = new QStandardItem;

	{
		QString txt (  tr ( "Devices" ) );
		_item_devices[0]->setFont ( fnt_emph );
		_item_devices[0]->setText ( txt );
	}

	_item_devices[0]->setColumnCount ( 2 );
	_item_devices[0]->setEditable ( false );
	_item_devices[0]->setSelectable ( true );

	_item_devices[1]->setToolTip ( tr ( "Number of devices" ) );
	_item_devices[1]->setEditable ( false );
	_item_devices[1]->setSelectable ( true );

	_item_ctl[0]->setChild ( 0, 0, _item_card_info[0] );
	_item_ctl[0]->setChild ( 0, 1, _item_card_info[1] );
	_item_ctl[0]->setChild ( 1, 0, _item_devices[0] );
	_item_ctl[0]->setChild ( 1, 1, _item_devices[1] );

	QStandardItem * item_root ( invisibleRootItem() );
	item_root->setChild ( 0, 0, _item_ctl[0] );
	item_root->setChild ( 0, 1, _item_ctl[1] );
}


CTL_Info_Model::~CTL_Info_Model ( )
{
}


void
CTL_Info_Model::set_pcm_info (
	const ::QSnd::CTL_Info * info_n )
{
	_pcm_info = info_n;

	setup_nodes();
}


void
CTL_Info_Model::setup_nodes ( )
{
	// Clear first
	_item_card_info[0]->removeRows ( 0, _item_card_info[0]->rowCount() );
	_item_card_info[1]->setText ( "" );

	_item_devices[0]->removeRows ( 0, _item_devices[0]->rowCount() );
	_item_devices[1]->setText ( "0" );

	_item_ctl[1]->setText ( "" );

	const unsigned int num_dev ( num_devices() );
	if ( ( num_dev == 0 ) || ( pcm_info() == 0 ) ) {
		return;
	}

	_item_ctl[1]->setText ( pcm_info()->ctl_name() );

	const Card_Info * card_info ( pcm_info()->card_info() );
	if ( card_info != 0 ) {
		const unsigned int num_str ( 7 );
		QString values[num_str];

		values[0] = QString ( "%1" ).arg ( card_info->card_index() );
		values[1] = card_info->card_id();
		values[2] = card_info->card_driver();
		values[3] = card_info->card_name();
		values[4] = card_info->card_long_name();
		values[5] = card_info->card_mixer_name();
		values[6] = card_info->card_components();

		_item_card_info[0]->setRowCount ( num_str );

		for ( unsigned int ii=0; ii < num_str; ++ii ) {
			QStandardItem * items[2] = {
				new QStandardItem, new QStandardItem };
			items[0]->setText ( _str_card[ii] );
			items[1]->setText ( values[ii] );
			for ( unsigned int jj=0; jj < 2; ++jj ) {
				items[jj]->setEditable ( false );
				items[jj]->setSelectable ( true );
				_item_card_info[0]->setChild ( ii, jj, items[jj] );
			}
		}
	}

	if ( pcm_info() != 0 ) {
		unsigned int num_dev ( pcm_info()->num_devices() );

		_item_devices[0]->setRowCount ( num_dev );
		_item_devices[1]->setText ( QString ( "%1" ).arg ( num_dev ) );

		for ( unsigned int ii=0; ii < num_dev; ++ii ) {
			setup_device_nodes ( _item_devices[0], ii );
		}
	}

}


void
CTL_Info_Model::setup_device_nodes (
	QStandardItem * parent_n,
	unsigned int idx_n )
{
	const PCM_Device_Info * dev_info (
		pcm_info()->device_info ( idx_n ) );

	const unsigned int num_items ( 5*2 );
	QStandardItem * item[num_items];
	for ( unsigned int ii=0; ii < num_items; ++ii ) {
		item[ii] = new QStandardItem;
		item[ii]->setEditable ( false );
		item[ii]->setSelectable ( true );
	}

	int idx = 0;

	// Device
	item[idx]->setText ( _str_dev[0].arg ( dev_info->dev_index() ) );
	item[idx]->setToolTip ( _ttip_dev[0] );

	++idx;
	//item[idx]->setText ( _str_dev[1].arg ( dev_info->dev_id() ) );
	//item[idx]->setToolTip ( _ttip_dev[1] );


	// Device name
	++idx;
	const unsigned int idx_dev_name ( idx );
	item[idx]->setText ( _str_dev_name[0] );
	item[idx]->setToolTip ( _ttip_dev_name[0] );

	++idx;
	item[idx]->setText ( _str_dev_name[1]
		.arg ( dev_info->dev_id() )
		.arg ( dev_info->dev_name() ) );
	item[idx]->setToolTip ( _ttip_dev_name[1] );


	// Subdevices
	++idx;
	const unsigned int idx_sdevs ( idx );
	item[idx]->setText ( _str_dev_subdevs[0] );

	++idx;
	item[idx]->setText ( _str_dev_subdevs[1] );

	// Playback subdevices
	++idx;
	const unsigned int idx_sdevs_play ( idx );
	setup_subdevices_nodes ( dev_info, 0, &item[idx_sdevs_play] );

	// Capture subdevices
	idx += 2;
	const unsigned int idx_sdevs_cap ( idx );
	setup_subdevices_nodes ( dev_info, 1, &item[idx_sdevs_cap] );

	item[idx_sdevs]->setChild ( 0, 0, item[idx_sdevs_play] );
	item[idx_sdevs]->setChild ( 0, 1, item[idx_sdevs_play+1] );
	item[idx_sdevs]->setChild ( 1, 0, item[idx_sdevs_cap] );
	item[idx_sdevs]->setChild ( 1, 1, item[idx_sdevs_cap+1] );


	item[0]->setChild ( 0, 0, item[idx_dev_name] );
	item[0]->setChild ( 0, 1, item[idx_dev_name+1] );
	item[0]->setChild ( 1, 0, item[idx_sdevs] );
	item[0]->setChild ( 1, 1, item[idx_sdevs+1] );


	parent_n->setChild ( idx_n, 0, item[0] );
	parent_n->setChild ( idx_n, 1, item[1] );
}


void
CTL_Info_Model::setup_subdevices_nodes (
	const PCM_Device_Info * dev_info_n,
	unsigned int snd_dir_n,
	QStandardItem * items_n[] )
{
	const PCM_Subdevices_Info & sdevs_info (
		dev_info_n->subdevices_info ( snd_dir_n ) );

	unsigned int num_devs ( sdevs_info.num_subdevices() );

	{
		items_n[0]->setText ( _str_sdevs_id[snd_dir_n] );

		QString val ( _str_sdevs_val[snd_dir_n] );
		if ( sdevs_info.num_subdevices() > 0 ) {
			val = val.arg ( sdevs_info.num_subdevices() );
			val = val.arg ( sdevs_info.num_subdevices_avail() );
		} else {
			val = val.arg ( 0 );
			val = val.arg ( 0 );
		}


		items_n[1]->setText ( val );
		items_n[1]->setToolTip ( _ttip_sdevs_val[snd_dir_n] );

		if ( num_devs > 0 ) {
			items_n[0]->setForeground ( _brush_snd_dir[snd_dir_n] );
			items_n[1]->setForeground ( _brush_snd_dir[snd_dir_n] );
		} else {
			items_n[0]->setEnabled ( false );
			items_n[1]->setEnabled ( false );
		}
	}

	for ( unsigned int ii=0; ii < num_devs; ++ii ) {

		QStandardItem * item_sdev[2] =
			{ new QStandardItem, new QStandardItem };

		const PCM_Subdevice_Info * sdev_info (
			sdevs_info.subdevice_info ( ii ) );

		item_sdev[0]->setText ( QString ( "%1" ).arg ( sdev_info->dev_idx() ) );
		item_sdev[0]->setEditable ( false );
		item_sdev[0]->setSelectable ( false );
		item_sdev[0]->setForeground ( _brush_snd_dir[snd_dir_n] );

		item_sdev[1]->setText ( sdev_info->dev_name() );
		item_sdev[1]->setEditable ( false );
		item_sdev[1]->setSelectable ( false );
		item_sdev[1]->setForeground ( _brush_snd_dir[snd_dir_n] );

		items_n[0]->setChild ( ii, 0, item_sdev[0] );
		items_n[0]->setChild ( ii, 1, item_sdev[1] );
	}
}


void
CTL_Info_Model::set_snd_dir_foreground (
	unsigned int dir_n,
	const QBrush & brush_n )
{
	_brush_snd_dir[dir_n] = brush_n;
}


int
CTL_Info_Model::num_devices ( ) const
{
	int res ( 0 );
	if ( pcm_info() != 0 ) {
		res = pcm_info()->num_devices();
	}
	return res;
}


} // End of namespace
