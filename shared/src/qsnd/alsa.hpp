/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_alsa_hpp__
#define __INC_qsnd_alsa_hpp__

#include <alsa/asoundlib.h>
#include <string>
#include <QString>


namespace QSnd
{


void
print_alsa_error (
	const ::std::string & prefix,
	int err );


QString
snd_error_qstring (
	int err );


} // End of namespace


#endif
