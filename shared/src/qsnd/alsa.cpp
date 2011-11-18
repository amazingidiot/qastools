//
// C++ Implementation:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
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

