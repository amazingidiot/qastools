/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_control_address_hpp__
#define __INC_qsnd_control_address_hpp__

#include "qsnd/ctl_address_argument.hpp"
#include <QList>
#include <QString>

namespace QSnd
{

/// @brief CTL_Address
///
class CTL_Address
{
  public:
  // -- Construction

  CTL_Address ( const char * ctl_str_n = 0 );

  CTL_Address ( const QString & ctl_str_n );

  CTL_Address ( const CTL_Address & ctl_addr_n );

  CTL_Address ( CTL_Address && ctl_addr_n );

  ~CTL_Address ();

  // -- Clear

  void
  clear ();

  bool
  is_valid () const
  {
    return !_ctl_name.isEmpty ();
  }

  // -- Control name

  const QString &
  ctl_name () const
  {
    return _ctl_name;
  }

  void
  set_ctl_name ( const QString & name_n );

  // -- Address string

  const QString &
  addr_str () const
  {
    return _addr_str;
  }

  void
  set_addr_str ( const QString & id_n );

  // -- Control arguments

  std::size_t
  num_args () const
  {
    return _args.size ();
  }

  const ::QSnd::CTL_Address_Argument &
  arg ( std::size_t idx_n ) const
  {
    return _args[ idx_n ];
  }

  void
  append_arg ( const ::QSnd::CTL_Address_Argument & arg_n );

  void
  append_arg ( ::QSnd::CTL_Address_Argument && arg_n );

  // -- Comparison

  bool
  match ( const ::QSnd::CTL_Address & ctl_addr_n ) const;

  // -- Assignment operators

  ::QSnd::CTL_Address &
  operator= ( const ::QSnd::CTL_Address & ctl_addr_n );

  ::QSnd::CTL_Address &
  operator= ( ::QSnd::CTL_Address && ctl_addr_n );

  private:
  // -- Utility

  void
  update_addr_str ();

  private:
  // -- Attributes
  QString _addr_str;
  QString _ctl_name;
  std::vector<::QSnd::CTL_Address_Argument > _args;
};

} // namespace QSnd

#endif
