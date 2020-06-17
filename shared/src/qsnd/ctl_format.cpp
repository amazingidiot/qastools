/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "ctl_format.hpp"

namespace QSnd
{

CTL_Format::CTL_Format ( const char * addr_str_n )
{
  if ( addr_str_n != 0 ) {
    _ctl_name = addr_str_n;
  }
}

CTL_Format::CTL_Format ( const QString & addr_str_n )
: _ctl_name ( addr_str_n )
{
}

CTL_Format::CTL_Format ( const CTL_Format & ctl_format_n )
{
  clone_def ( ctl_format_n );
}

CTL_Format::~CTL_Format ()
{
  clear ();
}

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
  _args.append ( arg_n );
}

bool
CTL_Format::match ( const ::QSnd::CTL_Address & ctl_addr_n ) const
{
  bool res ( true );
  if ( ctl_addr_n.ctl_name ().isEmpty () ) {
    res = false;
  }
  if ( res && ( ctl_addr_n.ctl_name () != ctl_name () ) ) {
    res = false;
  }
  if ( res && ( ctl_addr_n.num_args () > num_args () ) ) {
    res = false;
  }
  if ( res ) {
    for ( unsigned int ii = 0; ii < ctl_addr_n.num_args (); ++ii ) {
      const ::QSnd::CTL_Address_Argument & arga ( ctl_addr_n.arg ( ii ) );
      if ( !arga.arg_name.isEmpty () ) {
        const ::QSnd::CTL_Format_Argument & argd ( arg ( ii ) );
        if ( arga.arg_name != argd.arg_name ) {
          res = false;
          break;
        }
      }
    }
  }
  return res;
}

CTL_Format &
CTL_Format::operator= ( const ::QSnd::CTL_Format & ctl_format_n )
{
  clear ();
  clone_def ( ctl_format_n );
  return *this;
}

bool
CTL_Format::operator== ( const ::QSnd::CTL_Format & ctl_format_n ) const
{
  bool res;
  res = ( _ctl_name == ctl_format_n.ctl_name () );
  res = res && ( num_args () == ctl_format_n.num_args () );
  if ( res ) {
    for ( unsigned int ii = 0; ii < ctl_format_n.num_args (); ++ii ) {
      const ::QSnd::CTL_Format_Argument & arg1 ( arg ( ii ) );
      const ::QSnd::CTL_Format_Argument & arg2 ( ctl_format_n.arg ( ii ) );
      if ( ( arg1.arg_name != arg2.arg_name ) ||
           ( arg1.arg_type != arg2.arg_type ) ) {
        res = false;
        break;
      }
    }
  }
  return res;
}

bool
CTL_Format::operator!= ( const ::QSnd::CTL_Format & ctl_format_n ) const
{
  return !operator== ( ctl_format_n );
}

void
CTL_Format::clone_def ( const ::QSnd::CTL_Format & ctl_format_n )
{
  _ctl_name = ctl_format_n.ctl_name ();
  for ( unsigned int ii = 0; ii < ctl_format_n.num_args (); ++ii ) {
    _args.append ( ctl_format_n.arg ( ii ) );
  }
}

} // namespace QSnd
