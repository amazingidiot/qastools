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

#ifndef __INC_mwdg_ctl_arg_view_string_hpp__
#define __INC_mwdg_ctl_arg_view_string_hpp__

#include <mwdg/ctl_arg_view.hpp>
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
