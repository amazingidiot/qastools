/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_control_format_hpp__
#define __INC_qsnd_control_format_hpp__

#include "qsnd/ctl_address.hpp"
#include "qsnd/ctl_format_argument.hpp"
#include <QString>
#include <QList>


namespace QSnd
{


/// @brief CTL_Format
///
class CTL_Format
{
	// Public methods
	public:

	CTL_Format (
		const char * ctl_str_n = 0 );

	CTL_Format (
		const QString & ctl_str_n );

	CTL_Format (
		const CTL_Format & ctl_format_n );

	~CTL_Format ( );


	// Clear

	void
	clear ( );

	bool
	is_valid ( ) const;


	// Control name

	const QString &
	ctl_name ( ) const;

	void
	set_ctl_name (
		const QString & name_n );


	// Control arguments

	unsigned int
	num_args ( ) const;

	const ::QSnd::CTL_Format_Argument &
	arg (
		unsigned int idx_n ) const;

	void
	append_arg (
		const ::QSnd::CTL_Format_Argument & arg_n );


	// Comparison

	bool
	match (
		const ::QSnd::CTL_Address & ctl_addr_n ) const;


	// Operators

	CTL_Format &
	operator = (
		const ::QSnd::CTL_Format & ctl_format_n );

	bool
	operator == (
		const ::QSnd::CTL_Format & ctl_format_n ) const;

	bool
	operator != (
		const ::QSnd::CTL_Format & ctl_format_n ) const;


	// Private methods
	private:

	void
	clone_def (
		const ::QSnd::CTL_Format & ctl_format_n );


	// Private attributes
	private:

	QString _ctl_name;
	QList < ::QSnd::CTL_Format_Argument > _args;
};


inline
bool
CTL_Format::is_valid ( ) const
{
	return !_ctl_name.isEmpty();
}


inline
const QString &
CTL_Format::ctl_name ( ) const
{
	return _ctl_name;
}


inline
unsigned int
CTL_Format::num_args ( ) const
{
	return _args.size();
}


inline
const ::QSnd::CTL_Format_Argument &
CTL_Format::arg (
	unsigned int idx_n ) const
{
	return _args[idx_n];
}


} // End of namespace


#endif
