/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "ctl_address_argument.hpp"

namespace QSnd
{

CTL_Address_Argument::CTL_Address_Argument () = default;

CTL_Address_Argument::CTL_Address_Argument ( const QString & name_n,
                                             const QString & value_n )
: arg_name ( name_n )
, arg_value ( value_n )
{
}

void
CTL_Address_Argument::clear ()
{
  arg_name.clear ();
  arg_value.clear ();
}

} // namespace QSnd
