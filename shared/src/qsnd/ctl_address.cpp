/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "ctl_address.hpp"

namespace QSnd
{

CTL_Address::CTL_Address ( const char * addr_str_n )
{
  if ( addr_str_n != nullptr ) {
    set_addr_str ( QString ( addr_str_n ) );
  }
}

CTL_Address::CTL_Address ( const QString & addr_str_n )
{
  if ( !addr_str_n.isEmpty () ) {
    set_addr_str ( addr_str_n );
  }
}

CTL_Address::CTL_Address ( const CTL_Address & ctl_addr_n ) = default;

CTL_Address::CTL_Address ( CTL_Address && ctl_addr_n ) = default;

CTL_Address::~CTL_Address () = default;

void
CTL_Address::clear ()
{
  _addr_str.clear ();
  _ctl_name.clear ();
  _args.clear ();
}

void
CTL_Address::set_ctl_name ( const QString & name_n )
{
  _ctl_name = name_n;
  update_addr_str ();
}

void
CTL_Address::set_addr_str ( const QString & addr_str_n )
{
  clear ();

  int idx = addr_str_n.indexOf ( ':' );
  if ( idx == 0 ) {
    // Invalid
    return;
  }
  if ( idx < 0 ) {
    // Simple name
    _ctl_name = addr_str_n;
    _addr_str = _ctl_name;
    return;
  }
  // Name with arguments
  {
    QString pstr = addr_str_n;
    _ctl_name = pstr.left ( idx );
    pstr = pstr.mid ( idx + 1 );

    QString arg_str;
    while ( !pstr.isEmpty () ) {
      idx = pstr.indexOf ( ',' );
      if ( idx < 0 ) {
        arg_str = pstr;
        pstr.clear ();
      } else {
        arg_str = pstr.left ( idx );
        pstr = pstr.mid ( idx + 1 );
      }

      ::QSnd::CTL_Address_Argument argm;

      idx = arg_str.indexOf ( '=' );
      if ( idx < 0 ) {
        argm.arg_value = arg_str;
      } else {
        argm.arg_name = arg_str.left ( idx );
        argm.arg_value = arg_str.mid ( idx + 1 );
      }

      if ( !argm.arg_value.isEmpty () ) {
        _args.push_back ( std::move ( argm ) );
      }
    }
  }
  update_addr_str ();
}

void
CTL_Address::append_arg ( const ::QSnd::CTL_Address_Argument & arg_n )
{
  _args.push_back ( arg_n );
  update_addr_str ();
}

void
CTL_Address::append_arg ( ::QSnd::CTL_Address_Argument && arg_n )
{
  _args.push_back ( std::move ( arg_n ) );
  update_addr_str ();
}

bool
CTL_Address::match ( const ::QSnd::CTL_Address & ctl_addr_n ) const
{
  if ( _ctl_name != ctl_addr_n.ctl_name () ) {
    return false;
  }
  if ( num_args () != ctl_addr_n.num_args () ) {
    return false;
  }

  for ( std::size_t ii = 0; ii != num_args (); ++ii ) {
    const auto & arg1 = arg ( ii );
    const auto & arg2 = ctl_addr_n.arg ( ii );
    // Same value?
    if ( arg1.arg_value != arg2.arg_value ) {
      return false;
    }
    // Also same name?
    if ( !arg1.arg_name.isEmpty () && !arg2.arg_name.isEmpty () &&
         ( arg1.arg_name != arg2.arg_name ) ) {
      return false;
    }
  }

  return true;
}

::QSnd::CTL_Address &
CTL_Address::operator= ( const ::QSnd::CTL_Address & ctl_addr_n ) = default;

::QSnd::CTL_Address &
CTL_Address::operator= ( ::QSnd::CTL_Address && ctl_addr_n ) = default;

void
CTL_Address::update_addr_str ()
{
  _addr_str = _ctl_name;
  if ( _args.empty () ) {
    return;
  }

  _addr_str.append ( ":" );
  for ( std::size_t ii = 0; ii < _args.size (); ++ii ) {
    const auto & argm = arg ( ii );
    if ( ii != 0 ) {
      _addr_str.append ( "," );
    }
    if ( !argm.arg_name.isEmpty () ) {
      _addr_str.append ( argm.arg_name );
      _addr_str.append ( "=" );
    }
    _addr_str.append ( argm.arg_value );
  }
}

} // namespace QSnd
