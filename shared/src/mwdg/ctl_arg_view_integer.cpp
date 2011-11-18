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

#include "ctl_arg_view_integer.hpp"

#include <QLayout>
#include <QIntValidator>


namespace MWdg
{


CTL_Arg_View_Integer::CTL_Arg_View_Integer (
	QWidget * parent_n ) :
::MWdg::CTL_Arg_View ( parent_n )
{
	_ledit = new QLineEdit;
	QIntValidator * vali ( new QIntValidator ( _ledit ) );
	_ledit->setValidator ( vali );
	connect ( _ledit, SIGNAL ( editingFinished() ),
		this, SIGNAL ( sig_arg_changed() ) );

	lay_content()->addWidget ( _ledit );
}


QString
CTL_Arg_View_Integer::arg_string ( ) const
{
	return _ledit->text();
}


void
CTL_Arg_View_Integer::set_arg_string (
	const QString & str_n )
{
	_ledit->setText ( str_n );
}


} // End of namespace
