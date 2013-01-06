//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_qsnd_control_address_arg_hpp__
#define __INC_qsnd_control_address_arg_hpp__

#include <QString>


namespace QSnd
{


/// @brief CTL_Address_Arg
///
class CTL_Address_Arg
{
	// Public methods
	public:

	CTL_Address_Arg ( );

	CTL_Address_Arg (
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
