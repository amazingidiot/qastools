//
// C++ Interface:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INC_qsnd_control_def_hpp__
#define __INC_qsnd_control_def_hpp__

#include <qsnd/ctl_address.hpp>
#include <qsnd/ctl_def_arg.hpp>
#include <QString>
#include <QList>


namespace QSnd
{


/// @brief CTL_Def
///
class CTL_Def
{
	// Public methods
	public:

	CTL_Def (
		const char * ctl_str_n = 0 );

	CTL_Def (
		const QString & ctl_str_n );

	CTL_Def (
		const CTL_Def & ctl_def_n );

	~CTL_Def ( );


	// Clear

	void
	clear ( );

	bool
	is_clear ( ) const;


	// Control name

	const QString &
	ctl_name ( ) const;

	void
	set_ctl_name (
		const QString & name_n );


	// Control arguments

	unsigned int
	num_args ( ) const;

	const ::QSnd::CTL_Def_Arg *
	arg (
		unsigned int idx_n ) const;

	void
	append_arg (
		const ::QSnd::CTL_Def_Arg * arg_n );


	// Comparison

	bool
	match (
		const ::QSnd::CTL_Address & ctl_addr_n ) const;


	// Operators

	CTL_Def &
	operator = (
		const ::QSnd::CTL_Def & ctl_def_n );

	bool
	operator == (
		const ::QSnd::CTL_Def & ctl_def_n ) const;

	bool
	operator != (
		const ::QSnd::CTL_Def & ctl_def_n ) const;


	// Private methods
	private:

	void
	clone_def (
		const ::QSnd::CTL_Def & ctl_def_n );


	// Private attributes
	private:

	QString _ctl_name;
	QList < const ::QSnd::CTL_Def_Arg * > _args;
};


inline
bool
CTL_Def::is_clear ( ) const
{
	return _ctl_name.isEmpty();
}


inline
const QString &
CTL_Def::ctl_name ( ) const
{
	return _ctl_name;
}


inline
unsigned int
CTL_Def::num_args ( ) const
{
	return _args.size();
}


inline
const ::QSnd::CTL_Def_Arg *
CTL_Def::arg (
	unsigned int idx_n ) const
{
	return _args[idx_n];
}


} // End of namespace


#endif
