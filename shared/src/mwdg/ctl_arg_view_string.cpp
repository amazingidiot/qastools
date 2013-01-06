//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "ctl_arg_view_string.hpp"

#include <QLayout>


namespace MWdg
{


CTL_Arg_View_String::CTL_Arg_View_String (
	QWidget * parent_n ) :
::MWdg::CTL_Arg_View ( parent_n )
{
	_ledit = new QLineEdit;
	connect ( _ledit, SIGNAL ( editingFinished() ),
		this, SIGNAL ( sig_arg_changed() ) );
	lay_content()->addWidget ( _ledit );
}


QString
CTL_Arg_View_String::arg_string ( ) const
{
	return _ledit->text();
}


void
CTL_Arg_View_String::set_arg_string (
	const QString & str_n )
{
	_ledit->setText ( str_n );
}


} // End of namespace
