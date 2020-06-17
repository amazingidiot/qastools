/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "alsa.hpp"
#include <QString>
#include <iostream>
#include <sstream>

namespace QSnd
{

void
print_alsa_error ( const std::string & prefix, int err_n )
{
  ::std::stringstream msg;
  msg << "[EE] ";
  msg << prefix << ": ";
  msg << snd_strerror ( err_n ) << ::std::endl;
  ::std::cerr << msg.str ();
}

QString
snd_error_qstring ( int err_n )
{
  return QString::fromLocal8Bit ( snd_strerror ( err_n ) );
}

} // namespace QSnd
