//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_qsnd_control_def_arg_hpp__
#define __INC_qsnd_control_def_arg_hpp__

#include <QString>


namespace QSnd
{


/// @brief CTL_Def_Arg
///
class CTL_Def_Arg
{
	// Public methods
	public:

	CTL_Def_Arg ( );

	CTL_Def_Arg (
		const QString & name_n,
		const QString & type_n = QString ( "string" ) );

	void
	clear ( );


	// Public attributes
	public:

	QString arg_name;
	QString arg_type;
};


} // End of namespace


#endif
