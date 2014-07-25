//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "ctl_address_argument.hpp"


namespace QSnd
{


CTL_Address_Argument::CTL_Address_Argument ( )
{
}


CTL_Address_Argument::CTL_Address_Argument (
	const QString & name_n,
	const QString & value_n ) :
arg_name ( name_n ),
arg_value ( value_n )
{
}


void
CTL_Address_Argument::clear ( )
{
	arg_name.clear();
	arg_value.clear();
}


} // End of namespace