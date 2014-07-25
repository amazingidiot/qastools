//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
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
	_ledit_signal_block = false;

	QIntValidator * vali ( new QIntValidator ( _ledit ) );
	_ledit->setValidator ( vali );
	connect ( _ledit, SIGNAL ( editingFinished() ),
		this, SIGNAL ( sig_arg_changed() ) );

	lay_content()->addWidget ( _ledit );
}


void
CTL_Arg_View_Integer::set_arg_string (
	const QString & str_n )
{
	if ( set_arg_string_private ( str_n ) ) {
		{
			_ledit_signal_block = true;
			_ledit->setText ( str_n );
			_ledit_signal_block = false;
		}
		emit sig_arg_changed();
	}
}

void
CTL_Arg_View_Integer::input_string_changed ( )
{
	if ( !_ledit_signal_block ) {
		set_arg_string ( _ledit->text() );
	}
}


} // End of namespace
