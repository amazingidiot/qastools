/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_control_format_argument_hpp__
#define __INC_qsnd_control_format_argument_hpp__

#include <QString>


namespace QSnd
{


/// @brief CTL_Format_Argument
///
class CTL_Format_Argument
{
	// Public methods
	public:

	CTL_Format_Argument ( );

	CTL_Format_Argument (
		const QString & name_n,
		const QString & type_n = QString ( "string" ) );

	void
	clear ( );

	bool
	is_valid ( ) const;


	bool
	operator == (
		const ::QSnd::CTL_Format_Argument & ctl_arg_n ) const;

	bool
	operator != (
		const ::QSnd::CTL_Format_Argument & ctl_arg_n ) const;


	// Public attributes
	public:

	QString arg_name;
	QString arg_type;
};


} // End of namespace


#endif
