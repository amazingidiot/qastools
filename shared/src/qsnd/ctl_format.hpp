/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_control_format_hpp__
#define __INC_qsnd_control_format_hpp__

#include "qsnd/ctl_address.hpp"
#include "qsnd/ctl_format_argument.hpp"
#include <QString>
#include <vector>

namespace QSnd
{

/// @brief CTL_Format
///
class CTL_Format
{
  public:
  // -- Construction

  CTL_Format ( const char * ctl_str_n = nullptr );

  CTL_Format ( const QString & ctl_str_n );

  CTL_Format ( const CTL_Format & ctl_format_n );

  CTL_Format ( CTL_Format && ctl_format_n );

  ~CTL_Format ();

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

  // -- Control arguments

  std::size_t
  num_args () const
  {
    return _args.size ();
  }

  const ::QSnd::CTL_Format_Argument &
  arg ( std::size_t idx_n ) const
  {
    return _args[ idx_n ];
  }

  void
  append_arg ( const ::QSnd::CTL_Format_Argument & arg_n );

  void
  append_arg ( ::QSnd::CTL_Format_Argument && arg_n );

  // -- Address matching

  bool
  match ( const ::QSnd::CTL_Address & ctl_addr_n ) const;

  // -- Assignment Operators

  CTL_Format &
  operator= ( const ::QSnd::CTL_Format & ctl_format_n );

  CTL_Format &
  operator= ( ::QSnd::CTL_Format && ctl_format_n );

  // -- Comparison operators

  bool
  operator== ( const ::QSnd::CTL_Format & ctl_format_n ) const;

  bool
  operator!= ( const ::QSnd::CTL_Format & ctl_format_n ) const;

  private:
  // -- Attributes
  QString _ctl_name;
  std::vector<::QSnd::CTL_Format_Argument > _args;
};

} // namespace QSnd

#endif
