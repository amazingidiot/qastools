//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_views_device_selection_view_hpp__
#define __INC_views_device_selection_view_hpp__

#include "qsnd/ctl_address.hpp"
#include "qsnd/controls_db.hpp"
#include "mwdg/ctl_arg_view.hpp"

#include <QWidget>
#include <QVBoxLayout>


// Forward declaration
class QModelIndex;
namespace QSnd {
	class Controls_Model;
}
namespace MWdg {
	class Controls_View;
}
namespace Views {
	class Device_Selection_View_Setup;
}


namespace Views
{


/// @brief Device_Selection_View
///
/// Mixer device selection view
class Device_Selection_View :
	public QWidget
{
	Q_OBJECT


	// Public methods
	public:

	Device_Selection_View (
		QWidget * parent_n = 0 );

	~Device_Selection_View ( );


	void
	set_view_setup (
		::Views::Device_Selection_View_Setup * setup_n );


	const ::QSnd::CTL_Address &
	default_ctl ( ) const;

	const ::QSnd::CTL_Address &
	selected_ctl ( ) const;

	void
	silent_select_ctl (
		const ::QSnd::CTL_Address & ctl_addr_n );


	// Signals
	signals:

	void
	sig_close ( );

	void
	sig_control_changed ( );


	// Public slots
	public slots:

	void
	reload_database ( );


	// Protected methods
	protected:

	void
	contextMenuEvent (
		QContextMenuEvent * event_n );


	// Private slots
	private slots:

	void
	control_changed (
		const QModelIndex & cur_idx_n,
		const QModelIndex & prev_idx_n );

	void
	control_changed (
		const QModelIndex & idx_n );

	void
	control_arg_changed ( );


	// Private methods
	private:

	void
	clear_arg_views ( );

	void
	create_arg_views ( );

	void
	restore_arg_views ( );

	void
	compile_ctl_addr (
		::QSnd::CTL_Address & ctl_addr_n );

	bool
	update_selected_ctl ( );


	const ::QSnd::CTL_Address *
	sel_db_find (
		const QString & ctl_name_n ) const;

	void
	sel_db_commit (
		const ::QSnd::CTL_Address * ctl_addr_n );

	/// @brief Removes non existing CTL addresses
	///
	void
	sel_db_clean ( );


	// Private attributes
	private:

	::Views::Device_Selection_View_Setup * _view_setup;

	::QSnd::Controls_DB _controls_db;
	::QSnd::Controls_Model * _controls_model;
	::MWdg::Controls_View * _controls_view;
	QList < ::MWdg::CTL_Arg_View * > _arg_views;
	QVBoxLayout * _lay_arg_views;

	// Selection state
	const ::QSnd::CTL_Def * _current_ctl_def;
	::QSnd::CTL_Address _selected_ctl;
	bool _silent_ctl_change;
	bool _silent_arg_change;

	// Strings
	QString _str_type_card;
	QString _str_type_string;
	QString _str_type_integer;

	// Context menu
	QAction * _act_close;
};


inline
const ::QSnd::CTL_Address &
Device_Selection_View::selected_ctl ( ) const
{
	return _selected_ctl;
}


} // End of namespace


#endif