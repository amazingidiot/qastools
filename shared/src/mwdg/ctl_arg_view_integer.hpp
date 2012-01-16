//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_mwdg_ctl_arg_view_integer_hpp__
#define __INC_mwdg_ctl_arg_view_integer_hpp__

#include "mwdg/ctl_arg_view.hpp"
#include <QLineEdit>


namespace MWdg
{


/// @brief CTL_Arg_View_Integer
///
class CTL_Arg_View_Integer :
	public ::MWdg::CTL_Arg_View
{
	Q_OBJECT


	// Public methods
	public:

	CTL_Arg_View_Integer (
		QWidget * parent_n = 0 );

	QString
	arg_string ( ) const;

	void
	set_arg_string (
		const QString & str_n );


	// Private attributes
	private:

	QLineEdit * _ledit;
};


} // End of namespace


#endif
