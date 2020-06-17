/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "ctl_format.hpp"

namespace QSnd
{

CTL_Format::CTL_Format ( const char * addr_str_n )
{
  if ( addr_str_n != nullptr ) {
    _ctl_name = addr_str_n;
  }
}

CTL_Format::CTL_Format ( const QString & addr_str_n )
: _ctl_name ( addr_str_n )
{
}

CTL_Format::CTL_Format ( const CTL_Format & ctl_format_n ) = default;

CTL_Format::CTL_Format ( CTL_Format && ctl_format_n ) = default;

CTL_Format::~CTL_Format () = default;

void
CTL_Format::clear ()
{
  _ctl_name.clear ();
  _args.clear ();
}

void
CTL_Format::set_ctl_name ( const QString & name_n )
{
  _ctl_name = name_n;
}

void
CTL_Format::append_arg ( const ::QSnd::CTL_Format_Argument & arg_n )
{
  _args.push_back ( arg_n );
}

void
CTL_Format::append_arg ( ::QSnd::CTL_Format_Argument && arg_n )
{
  _args.push_back ( std::move ( arg_n ) );
}

bool
CTL_Format::match ( const ::QSnd::CTL_Address & ctl_addr_n ) const
{
  if ( ctl_addr_n.ctl_name ().isEmpty () ) {
    return false;
  }
  if ( ctl_addr_n.ctl_name () != ctl_name () ) {
    return false;
  }
  if ( ctl_addr_n.num_args () > num_args () ) {
    return false;
  }

  for ( std::size_t ii = 0; ii != ctl_addr_n.num_args (); ++ii ) {
    const auto & arga = ctl_addr_n.arg ( ii );
    if ( arga.arg_name.isEmpty () ) {
      continue;
    }
    const auto & argd = arg ( ii );
    if ( arga.arg_name != argd.arg_name ) {
      return false;
    }
  }

  return true;
}

CTL_Format &
CTL_Format::operator= ( const ::QSnd::CTL_Format & ctl_format_n ) = default;

CTL_Format &
CTL_Format::operator= ( ::QSnd::CTL_Format && ctl_format_n ) = default;

bool
CTL_Format::operator== ( const ::QSnd::CTL_Format & ctl_format_n ) const
{
  return ( _ctl_name == ctl_format_n._ctl_name ) &&
         ( _args == ctl_format_n._args );
}

bool
CTL_Format::operator!= ( const ::QSnd::CTL_Format & ctl_format_n ) const
{
  return !operator== ( ctl_format_n );
}

} // namespace QSnd
