//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "alsa.hpp"

#include <iostream>
#include <QString>


namespace QSnd
{


void
print_alsa_error (
	const std::string & prefix,
	int err_n )
{
	::std::cerr << "[EE] ";
	::std::cerr << prefix << ": ";
	::std::cerr << snd_strerror ( err_n ) << "\n";
}


QString
snd_error_qstring (
	int err_n )
{
	return QString::fromLocal8Bit ( snd_strerror ( err_n ) );
}


} // End of namespace

