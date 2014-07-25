//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "ctl_format_argument.hpp"


namespace QSnd
{


CTL_Format_Argument::CTL_Format_Argument ( ) :
arg_type ( "string" )
{
}

CTL_Format_Argument::CTL_Format_Argument (
	const QString & name_n,
	const QString & type_n ) :
arg_name ( name_n ),
arg_type ( type_n )
{
}

void
CTL_Format_Argument::clear ( )
{
	arg_name.clear();
	arg_type.clear();
}

bool
CTL_Format_Argument::is_valid ( ) const
{
	bool res ( true );
	if ( arg_name.isEmpty() ) {
		res = false;
	} else if ( arg_type.isEmpty() ) {
		res = false;
	}
	return res;
}

bool
CTL_Format_Argument::operator == (
	const ::QSnd::CTL_Format_Argument & ctl_arg_n ) const
{
	return (
		( arg_name == ctl_arg_n.arg_name ) &&
		( arg_type == ctl_arg_n.arg_type ) );
}

bool
CTL_Format_Argument::operator != (
	const ::QSnd::CTL_Format_Argument & ctl_arg_n ) const
{
	return (
		( arg_name != ctl_arg_n.arg_name ) ||
		( arg_type != ctl_arg_n.arg_type ) );
}


} // End of namespace
