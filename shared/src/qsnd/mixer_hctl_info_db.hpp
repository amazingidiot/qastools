/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_mixer_hctl_info_db_hpp__
#define __INC_qsnd_mixer_hctl_info_db_hpp__

#include "qsnd/alsa.hpp"
#include <QList>
#include <QString>

namespace QSnd
{

/// @brief Mixer_HCTL_Info_Db
///
/// Offers localized strings and translation tables
/// for Alsa objects
class Mixer_HCTL_Info_Db
{
  // Public methods
  public:
  Mixer_HCTL_Info_Db ();

  // Element types

  unsigned int
  num_element_types () const;

  unsigned int
  element_type_idx ( snd_ctl_elem_type_t type_n ) const;

  snd_ctl_elem_type_t
  element_idx_type ( unsigned int idx_n ) const;

  const QString &
  elem_type_name ( snd_ctl_elem_type_t type_n ) const;

  const QString &
  elem_type_name ( unsigned int type_idx_n ) const;

  const QString &
  elem_type_display_name ( snd_ctl_elem_type_t type_n ) const;

  const QString &
  elem_type_display_name ( unsigned int type_idx_n ) const;

  // Interface types

  unsigned int
  num_iface_types () const;

  unsigned int
  iface_type_idx ( snd_ctl_elem_iface_t type_n ) const;

  snd_ctl_elem_iface_t
  iface_idx_type ( unsigned int idx_n ) const;

  const QString &
  iface_name ( snd_ctl_elem_iface_t type_n ) const;

  const QString &
  iface_name ( unsigned int type_idx_n ) const;

  const QString &
  iface_display_name ( snd_ctl_elem_iface_t type_n ) const;

  const QString &
  iface_display_name ( unsigned int type_idx_n ) const;

  const QString &
  flag_readable_text ( bool flag_n = true ) const;

  const QString &
  flag_readable_char ( bool flag_n = true ) const;

  const QString &
  flag_writable_text ( bool flag_n = true ) const;

  const QString &
  flag_writable_char ( bool flag_n = true ) const;

  const QString &
  flag_active_text ( bool flag_n = true ) const;

  const QString &
  flag_active_char ( bool flag_n = true ) const;

  const QString &
  flag_volatile_text ( bool flag_n = true ) const;

  const QString &
  flag_volatile_char ( bool flag_n = true ) const;

  // Private attributes
  private:
  static const unsigned int _num_iface_types = 7;
  static const unsigned int _num_elem_types = 7;

  QString _etype_names[ _num_elem_types + 1 ];
  QString _etype_display_names[ _num_elem_types + 1 ];

  QString _iface_names[ _num_iface_types + 1 ];
  QString _iface_display_names[ _num_iface_types + 1 ];

  QString _flag_readable_text[ 2 ];
  QString _flag_readable_char[ 2 ];
  QString _flag_writable_text[ 2 ];
  QString _flag_writable_char[ 2 ];
  QString _flag_volatile_text[ 2 ];
  QString _flag_volatile_char[ 2 ];
  QString _flag_active_text[ 2 ];
  QString _flag_active_char[ 2 ];
};

// Element types

inline unsigned int
Mixer_HCTL_Info_Db::num_element_types () const
{
  return _num_elem_types;
}

inline unsigned int
Mixer_HCTL_Info_Db::element_type_idx ( snd_ctl_elem_type_t type_n ) const
{
  unsigned int res ( 0 );
  switch ( type_n ) {
  case SND_CTL_ELEM_TYPE_NONE:
    res = 0;
    break;
  case SND_CTL_ELEM_TYPE_BOOLEAN:
    res = 1;
    break;
  case SND_CTL_ELEM_TYPE_INTEGER:
    res = 2;
    break;
  case SND_CTL_ELEM_TYPE_ENUMERATED:
    res = 3;
    break;
  case SND_CTL_ELEM_TYPE_BYTES:
    res = 4;
    break;
  case SND_CTL_ELEM_TYPE_IEC958:
    res = 5;
    break;
  case SND_CTL_ELEM_TYPE_INTEGER64:
    res = 6;
    break;
  default:
    break;
  }
  return res;
}

inline snd_ctl_elem_type_t
Mixer_HCTL_Info_Db::element_idx_type ( unsigned int idx_n ) const
{
  snd_ctl_elem_type_t res ( SND_CTL_ELEM_TYPE_NONE );
  switch ( idx_n ) {
  case 0:
    res = SND_CTL_ELEM_TYPE_NONE;
    break;
  case 1:
    res = SND_CTL_ELEM_TYPE_BOOLEAN;
    break;
  case 2:
    res = SND_CTL_ELEM_TYPE_INTEGER;
    break;
  case 3:
    res = SND_CTL_ELEM_TYPE_ENUMERATED;
    break;
  case 4:
    res = SND_CTL_ELEM_TYPE_BYTES;
    break;
  case 5:
    res = SND_CTL_ELEM_TYPE_IEC958;
    break;
  case 6:
    res = SND_CTL_ELEM_TYPE_INTEGER64;
    break;
  default:
    break;
  }
  return res;
}

inline const QString &
Mixer_HCTL_Info_Db::elem_type_name ( unsigned int type_idx_n ) const
{
  if ( type_idx_n > num_element_types () ) {
    type_idx_n = num_element_types ();
  }
  return _etype_names[ type_idx_n ];
}

inline const QString &
Mixer_HCTL_Info_Db::elem_type_display_name ( unsigned int type_idx_n ) const
{
  if ( type_idx_n > num_element_types () ) {
    type_idx_n = num_element_types ();
  }
  return _etype_display_names[ type_idx_n ];
}

// Interface types

inline unsigned int
Mixer_HCTL_Info_Db::num_iface_types () const
{
  return _num_iface_types;
}

inline unsigned int
Mixer_HCTL_Info_Db::iface_type_idx ( snd_ctl_elem_iface_t type_n ) const
{
  unsigned int res ( 2 );
  switch ( type_n ) {
  case SND_CTL_ELEM_IFACE_CARD:
    res = 0;
    break;
  case SND_CTL_ELEM_IFACE_HWDEP:
    res = 1;
    break;
  case SND_CTL_ELEM_IFACE_MIXER:
    res = 2;
    break;
  case SND_CTL_ELEM_IFACE_PCM:
    res = 3;
    break;
  case SND_CTL_ELEM_IFACE_RAWMIDI:
    res = 4;
    break;
  case SND_CTL_ELEM_IFACE_TIMER:
    res = 5;
    break;
  case SND_CTL_ELEM_IFACE_SEQUENCER:
    res = 6;
    break;
  default:
    break;
  }
  return res;
}

inline snd_ctl_elem_iface_t
Mixer_HCTL_Info_Db::iface_idx_type ( unsigned int idx_n ) const
{
  snd_ctl_elem_iface_t res ( SND_CTL_ELEM_IFACE_CARD );
  switch ( idx_n ) {
  case 0:
    res = SND_CTL_ELEM_IFACE_CARD;
    break;
  case 1:
    res = SND_CTL_ELEM_IFACE_HWDEP;
    break;
  case 2:
    res = SND_CTL_ELEM_IFACE_MIXER;
    break;
  case 3:
    res = SND_CTL_ELEM_IFACE_PCM;
    break;
  case 4:
    res = SND_CTL_ELEM_IFACE_RAWMIDI;
    break;
  case 5:
    res = SND_CTL_ELEM_IFACE_TIMER;
    break;
  case 6:
    res = SND_CTL_ELEM_IFACE_SEQUENCER;
    break;
  default:
    break;
  }
  return res;
}

inline const QString &
Mixer_HCTL_Info_Db::iface_name ( unsigned int type_idx_n ) const
{
  if ( type_idx_n > num_iface_types () ) {
    type_idx_n = num_iface_types ();
  }
  return _iface_names[ type_idx_n ];
}

inline const QString &
Mixer_HCTL_Info_Db::iface_display_name ( unsigned int type_idx_n ) const
{
  if ( type_idx_n > num_iface_types () ) {
    type_idx_n = num_iface_types ();
  }
  return _iface_display_names[ type_idx_n ];
}

inline const QString &
Mixer_HCTL_Info_Db::flag_readable_text ( bool flag_n ) const
{
  return _flag_readable_text[ ( flag_n ? 1 : 0 ) ];
}

inline const QString &
Mixer_HCTL_Info_Db::flag_readable_char ( bool flag_n ) const
{
  return _flag_readable_char[ ( flag_n ? 1 : 0 ) ];
}

inline const QString &
Mixer_HCTL_Info_Db::flag_writable_text ( bool flag_n ) const
{
  return _flag_writable_text[ ( flag_n ? 1 : 0 ) ];
}

inline const QString &
Mixer_HCTL_Info_Db::flag_writable_char ( bool flag_n ) const
{
  return _flag_writable_char[ ( flag_n ? 1 : 0 ) ];
}

inline const QString &
Mixer_HCTL_Info_Db::flag_volatile_text ( bool flag_n ) const
{
  return _flag_volatile_text[ ( flag_n ? 1 : 0 ) ];
}

inline const QString &
Mixer_HCTL_Info_Db::flag_volatile_char ( bool flag_n ) const
{
  return _flag_volatile_char[ ( flag_n ? 1 : 0 ) ];
}

inline const QString &
Mixer_HCTL_Info_Db::flag_active_text ( bool flag_n ) const
{
  return _flag_active_text[ ( flag_n ? 1 : 0 ) ];
}

inline const QString &
Mixer_HCTL_Info_Db::flag_active_char ( bool flag_n ) const
{
  return _flag_active_char[ ( flag_n ? 1 : 0 ) ];
}

} // namespace QSnd

#endif
