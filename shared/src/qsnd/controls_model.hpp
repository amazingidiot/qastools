//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_qsnd_controls_model_hpp__
#define __INC_qsnd_controls_model_hpp__

#include <QStandardItemModel>


// Forward declaration
namespace QSnd {
	class CTL_Format;
	class Controls_Database;
}


namespace QSnd
{


/// @brief Controls_Model
///
class Controls_Model :
	public QStandardItemModel
{
	Q_OBJECT;


	// Public methods
	public:

	Controls_Model (
		QObject * parent_n = 0 );

	~Controls_Model ( );


	// Controls database

	::QSnd::Controls_Database *
	controls_db ( ) const;

	void
	set_controls_db (
		::QSnd::Controls_Database * ctl_db_n );


	// Control definition access

	const ::QSnd::CTL_Format *
	ctl_format (
		const QModelIndex & idx_n ) const;

	QModelIndex
	ctl_format_index (
		const QString & ctl_addr_n ) const;


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

	::QSnd::Controls_Database * _ctl_db;
};


inline
::QSnd::Controls_Database *
Controls_Model::controls_db ( ) const
{
	return _ctl_db;
}


} // End of namespace


#endif
