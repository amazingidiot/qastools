//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_mwdg_ctl_arg_view_card_hpp__
#define __INC_mwdg_ctl_arg_view_card_hpp__

#include "mwdg/ctl_arg_view.hpp"

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

	QString
	arg_string ( ) const;

	void
	set_arg_string (
		const QString & str_n );


	// Protected methods
	protected:

	void
	ctl_db_changed ( );


	// Private attributes
	private:

	::QSnd::Cards_Model * _cards_model;
	::MWdg::Controls_View * _ctl_view;
};


} // End of namespace


#endif
