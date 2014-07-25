//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_qsnd_control_address_argument_hpp__
#define __INC_qsnd_control_address_argument_hpp__

#include <QString>


namespace QSnd
{


/// @brief CTL_Address_Argument
///
class CTL_Address_Argument
{
	// Public methods
	public:

	CTL_Address_Argument ( );

	CTL_Address_Argument (
		const QString & name_n,
		const QString & value_n = QString() );

	void
	clear ( );


	// Public attributes

	QString arg_name;
	QString arg_value;
};


} // End of namespace


#endif