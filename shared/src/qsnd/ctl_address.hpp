/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_control_address_hpp__
#define __INC_qsnd_control_address_hpp__

#include "qsnd/ctl_address_argument.hpp"
#include <QString>
#include <QList>


namespace QSnd
{


/// @brief CTL_Address
///
class CTL_Address
{
	// Public methods
	public:

	CTL_Address (
		const char * ctl_str_n = 0 );

	CTL_Address (
		const QString & ctl_str_n );

	CTL_Address (
		const CTL_Address & ctl_addr_n );

	~CTL_Address ( );


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


	// Address string

	const QString &
	addr_str ( ) const;

	void
	set_addr_str (
		const QString & id_n );


	// Control arguments

	unsigned int
	num_args ( ) const;

	const ::QSnd::CTL_Address_Argument &
	arg (
		unsigned int idx_n ) const;

	void
	append_arg (
		const ::QSnd::CTL_Address_Argument & arg_n );


	// Comparison

	bool
	match (
		const ::QSnd::CTL_Address & ctl_addr_n ) const;


	// Operators

	::QSnd::CTL_Address &
	operator = (
		const ::QSnd::CTL_Address & ctl_addr_n );


	// Private methods
	private:

	void
	clone_addr (
		const ::QSnd::CTL_Address & ctl_addr_n );

	void
	update_addr_str ( );


	// Private attributes
	private:

	QString _addr_str;
	QString _ctl_name;
	QList < ::QSnd::CTL_Address_Argument > _args;
};


inline
bool
CTL_Address::is_valid ( ) const
{
	return !_ctl_name.isEmpty();
}


inline
const QString &
CTL_Address::ctl_name ( ) const
{
	return _ctl_name;
}


inline
unsigned int
CTL_Address::num_args ( ) const
{
	return _args.size();
}


inline
const ::QSnd::CTL_Address_Argument &
CTL_Address::arg (
	unsigned int idx_n ) const
{
	return _args[idx_n];
}


inline
const QString &
CTL_Address::addr_str ( ) const
{
	return _addr_str;
}


} // End of namespace


#endif
