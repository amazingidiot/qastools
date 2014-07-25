//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_mwdg_ctl_arg_view_card_hpp__
#define __INC_mwdg_ctl_arg_view_card_hpp__

#include "mwdg/ctl_arg_view.hpp"
#include <QModelIndex>

// Forward declaration
namespace QSnd {
	class Cards_Model;
}
namespace MWdg {
	class Controls_View;
}


namespace MWdg
{


/// @brief CTL_Arg_View_Card
///
class CTL_Arg_View_Card :
	public ::MWdg::CTL_Arg_View
{
	Q_OBJECT


	// Public methods
	public:

	CTL_Arg_View_Card (
		QWidget * parent_n = 0 );

	void
	set_arg_string (
		const QString & str_n );


	// Protected methods
	protected:

	void
	ctl_db_changed ( );


	// Protected methods
	protected slots:

	void
	view_selection_changed (
		const QModelIndex & index_to_n,
		const QModelIndex & index_from_n );


	// Private attributes
	private:

	::QSnd::Cards_Model * _cards_model;
	::MWdg::Controls_View * _ctl_view;
};


} // End of namespace


#endif
