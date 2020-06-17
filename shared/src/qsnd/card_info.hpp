/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_qsnd_card_info_hpp__
#define __INC_qsnd_qsnd_card_info_hpp__

#include "qsnd/alsa.hpp"
#include <QString>

namespace QSnd
{

/// @brief Card_Info
///
class Card_Info
{
  public:
  // -- Construction

  Card_Info ();

  Card_Info ( const int hw_idx_n );

  Card_Info ( const QString & dev_str_n );

  // -- Setup

  void
  clear ();

  int
  acquire_info ( const int hw_idx_n );

  int
  acquire_info ( const QString & dev_str_n );

  int
  acquire_info ( snd_hctl_t * snd_hctl_n );

  // -- Accessors

  bool
  is_valid () const
  {
    return ( _index >= 0 );
  }

  int
  index () const
  {
    return _index;
  }

  const QString &
  id () const
  {
    return _id;
  }

  const QString &
  driver () const
  {
    return _driver;
  }

  const QString &
  name () const
  {
    return _name;
  }

  const QString &
  long_name () const
  {
    return _long_name;
  }

  const QString &
  mixer_name () const
  {
    return _mixer_name;
  }

  const QString &
  components () const
  {
    return _components;
  }

  // -- Comparison operators

  bool
  operator== ( const ::QSnd::Card_Info & cinfo_n ) const;

  bool
  operator!= ( const ::QSnd::Card_Info & cinfo_n ) const;

  private:
  // -- Attributes
  int _index = -1;
  QString _id;
  QString _driver;
  QString _name;
  QString _long_name;
  QString _mixer_name;
  QString _components;
};

} // namespace QSnd

#endif
