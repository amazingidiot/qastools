//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "ctl_def_arg.hpp"


namespace QSnd
{


CTL_Def_Arg::CTL_Def_Arg ( ) :
arg_type ( "string" )
{
}


CTL_Def_Arg::CTL_Def_Arg (
	const QString & name_n,
	const QString & type_n ) :
arg_name ( name_n ),
arg_type ( type_n )
{
}


void
CTL_Def_Arg::clear ( )
{
	arg_name.clear();
	arg_type.clear();
}


} // End of namespace
