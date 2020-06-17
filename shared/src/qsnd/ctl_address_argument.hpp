/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_control_address_argument_hpp__
#define __INC_qsnd_control_address_argument_hpp__

#include <QString>

namespace QSnd
{

/// @brief CTL_Address_Argument
///
class CTL_Address_Argument
{
  public:
  // -- Construction

  CTL_Address_Argument ();

  CTL_Address_Argument ( const QString & name_n,
                         const QString & value_n = QString () );

  void
  clear ();

  // -- Attributes
  QString arg_name;
  QString arg_value;
};

} // namespace QSnd

#endif
