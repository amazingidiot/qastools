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

#ifndef __INC_qsnd_ctl_info_model_hpp__
#define __INC_qsnd_ctl_info_model_hpp__

#include <QAbstractItemModel>
#include <QStandardItemModel>

#include "ctl_info.hpp"
#include <QBrush>


namespace QSnd
{


/// @brief CTL_Info_Model
///
class CTL_Info_Model :
	public QStandardItemModel
{
	Q_OBJECT

	// Public methods
	public:

	CTL_Info_Model ( );

	~CTL_Info_Model ( );



	/// @brief PCM info class getter
	///
	const CTL_Info *
	pcm_info ( ) const;

	/// @brief PCM info class setter
	///
	void
	set_pcm_info (
		const CTL_Info * info_n );

	void
	set_snd_dir_foreground (
		unsigned int dir_n,
		const QBrush & brush_n );


	/// @brief Number of devices getter
	///
	int
	num_devices ( ) const;


	// Private methods
	private:

	void
	setup_nodes ( );

	void
	setup_device_nodes (
		QStandardItem * parent_n,
		unsigned int idx_n );

	void
	setup_subdevices_nodes (
		const PCM_Device_Info * dev_info_n,
		unsigned int snd_dir_n,
		QStandardItem * items_n[] );

	void
	update_colors ( );


	// Private attributes
	private:

	const CTL_Info * _pcm_info;

	QStandardItem * _item_ctl[2];
	QStandardItem * _item_card_info[2];
	QStandardItem * _item_devices[2];

	QString _str_card[8];

	QString _str_dev[2];
	QString _ttip_dev[2];

	QString _str_dev_name[2];
	QString _ttip_dev_name[2];

	QString _str_dev_subdevs[2];

	QString _str_sdevs_id[2];
	QString _str_sdevs_val[2];
	QString _ttip_sdevs_val[2];

	QBrush _brush_snd_dir[2];
};


inline
const CTL_Info *
CTL_Info_Model::pcm_info ( ) const
{
	return _pcm_info;
}


} // End of namespace


#endif
