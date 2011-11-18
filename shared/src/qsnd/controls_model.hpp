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

#ifndef __INC_qsnd_controls_model_hpp__
#define __INC_qsnd_controls_model_hpp__

#include <QStandardItemModel>


// Forward declaration
namespace QSnd {
	class CTL_Def;
	class Controls_DB;
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

	::QSnd::Controls_DB *
	controls_db ( ) const;

	void
	set_controls_db (
		::QSnd::Controls_DB * ctl_db_n );


	// Control definition access

	const ::QSnd::CTL_Def *
	ctl_def (
		const QModelIndex & idx_n ) const;

	QModelIndex
	ctl_def_index (
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

	::QSnd::Controls_DB * _ctl_db;
};


inline
::QSnd::Controls_DB *
Controls_Model::controls_db ( ) const
{
	return _ctl_db;
}


} // End of namespace


#endif
