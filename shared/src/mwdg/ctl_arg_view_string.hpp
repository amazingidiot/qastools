//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_mwdg_ctl_arg_view_string_hpp__
#define __INC_mwdg_ctl_arg_view_string_hpp__

#include "mwdg/ctl_arg_view.hpp"
#include <QLineEdit>


namespace MWdg
{


/// @brief CTL_Arg_View_String
///
class CTL_Arg_View_String :
	public ::MWdg::CTL_Arg_View
{
	Q_OBJECT


	// Public methods
	public:

	CTL_Arg_View_String (
		QWidget * parent_n = 0 );


	void
	set_arg_string (
		const QString & str_n );


	// Protected slots
	protected slots:

	void
	input_string_changed ( );


	// Private attributes
	private:

	QLineEdit * _ledit;
	bool _ledit_signal_block;
};


} // End of namespace


#endif
