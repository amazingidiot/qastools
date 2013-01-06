//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#include "ctl_def.hpp"


namespace QSnd
{


CTL_Def::CTL_Def (
	const char * addr_str_n )
{
	if ( addr_str_n != 0 ) {
		_ctl_name = addr_str_n;
	}
}


CTL_Def::CTL_Def (
	const QString & addr_str_n ) :
_ctl_name ( addr_str_n )
{
}


CTL_Def::CTL_Def (
	const CTL_Def & ctl_def_n )
{
	clone_def ( ctl_def_n );
}


CTL_Def::~CTL_Def ( )
{
	clear();
}


void
CTL_Def::clear ( )
{
	_ctl_name.clear();
	if ( _args.size() > 0 ) {
		for ( int ii=0; ii < _args.size(); ++ii ) {
			delete _args[ii];
		}
		_args.clear();
	}
}


void
CTL_Def::set_ctl_name (
	const QString & name_n )
{
	_ctl_name = name_n;
}


void
CTL_Def::append_arg (
	const ::QSnd::CTL_Def_Arg * arg_n )
{
	if ( arg_n != 0 ) {
		_args.append ( arg_n );
	}
}


bool
CTL_Def::match (
	const ::QSnd::CTL_Address & ctl_addr_n ) const
{
	bool res ( true );
	if ( ctl_addr_n.ctl_name().isEmpty() ) {
		res = false;
	}
	if ( res && ( ctl_addr_n.ctl_name() != ctl_name() ) ) {
		res = false;
	}
	if ( res && ( ctl_addr_n.num_args() > num_args() ) ) {
		res = false;
	}
	if ( res ) {
		for ( unsigned int ii=0; ii < ctl_addr_n.num_args(); ++ii ) {
			const ::QSnd::CTL_Address_Arg * arga ( ctl_addr_n.arg ( ii ) );
			if ( !arga->arg_name.isEmpty() ) {
				const ::QSnd::CTL_Def_Arg * argd ( arg ( ii ) );
				if ( arga->arg_name != argd->arg_name ) {
					res = false;
					break;
				}
			}
		}
	}
	return res;
}


CTL_Def &
CTL_Def::operator = (
	const ::QSnd::CTL_Def & ctl_def_n )
{
	clear();
	clone_def ( ctl_def_n );
	return *this;
}


bool
CTL_Def::operator == (
	const ::QSnd::CTL_Def & ctl_def_n ) const
{
	bool res;
	res = ( _ctl_name == ctl_def_n.ctl_name() );
	res = res && ( num_args() == ctl_def_n.num_args() );
	if ( res ) {
		for ( unsigned int ii=0; ii < ctl_def_n.num_args(); ++ii ) {
			const ::QSnd::CTL_Def_Arg * arg1 ( arg ( ii ) );
			const ::QSnd::CTL_Def_Arg * arg2 ( ctl_def_n.arg ( ii ) );
			if (
				( arg1->arg_name != arg2->arg_name ) ||
				( arg1->arg_type != arg2->arg_type ) )
			{
				res = false;
				break;
			}
		}
	}
	return res;
}


bool
CTL_Def::operator != (
	const ::QSnd::CTL_Def & ctl_def_n ) const
{
	return !operator== ( ctl_def_n );
}


void
CTL_Def::clone_def (
	const ::QSnd::CTL_Def & ctl_def_n )
{
	_ctl_name = ctl_def_n.ctl_name();
	for ( unsigned int ii=0; ii < ctl_def_n.num_args(); ++ii ) {
		_args.append ( new ::QSnd::CTL_Def_Arg ( *ctl_def_n.arg ( ii ) ) );
	}
}


} // End of namespace

