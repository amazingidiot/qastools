/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_mixer_hctl_elem_hpp__
#define __INC_qsnd_mixer_hctl_elem_hpp__

#include "qsnd/alsa.hpp"
#include "qsnd/mixer_hctl_info_db.hpp"
#include <QObject>
#include <QString>
#include <vector>

namespace QSnd
{

/// @brief Mixer_HCTL_Elem
///
/// Brings Qt and ALSA objects together but without
/// any GUI objects
///
class Mixer_HCTL_Elem : public QObject
{
  Q_OBJECT

  // Public typedefs
  public:
  typedef ::std::vector< QString > Enum_Names_Buffer;

  // Public methods
  public:
  Mixer_HCTL_Elem ( const Mixer_HCTL_Info_Db & info_db_n,
                    QObject * parent_n = 0,
                    snd_hctl_t * hctl_n = 0,
                    snd_hctl_elem_t * hctl_elem_n = 0 );

  ~Mixer_HCTL_Elem ();

  void
  clear ();

  // Alsa hctl

  snd_hctl_t *
  snd_hctl () const;

  void
  set_snd_hctl ( snd_hctl_t * hctl_n );

  // Alsa hctl elem

  void
  set_snd_hctl_elem ( snd_hctl_elem_t * hctl_elem_n );

  snd_hctl_elem_t *
  snd_hctl_elem () const;

  // Alsa snd_ctl_elem_info_t

  const snd_ctl_elem_info_t *
  snd_ctl_info () const;

  const QString &
  display_name () const;

  void
  set_display_name ( const QString & name_n );

  const char *
  elem_name () const;

  unsigned int
  elem_numid () const;

  unsigned int
  elem_index () const;

  unsigned int
  device () const;

  unsigned int
  subdevice () const;

  snd_ctl_elem_iface_t
  iface () const;

  unsigned int
  iface_type_idx () const;

  const QString &
  iface_name () const;

  const QString &
  iface_display_name () const;

  unsigned int
  count () const;

  // Type

  snd_ctl_elem_type_t
  elem_type () const;

  const QString &
  elem_type_name () const;

  const QString &
  elem_type_display_name () const;

  bool
  is_boolean () const;

  bool
  is_integer () const;

  bool
  is_enumerated () const;

  bool
  is_bytes () const;

  bool
  is_IEC958 () const;

  bool
  is_integer64 () const;

  // State flags

  bool
  is_volatile () const;

  bool
  is_readable () const;

  bool
  is_writable () const;

  bool
  is_active () const;

  // Boolean type

  bool
  switch_state ( unsigned int idx_n ) const;

  bool
  switches_equal () const;

  void
  set_switch_state ( unsigned int idx_n, bool state_n );

  void
  set_switch_all ( bool state_n );

  void
  invert_switch_state ( unsigned int idx_n );

  void
  invert_switch_all ();

  void
  level_switches ();

  // Integer type

  long
  integer_min () const;

  long
  integer_max () const;

  long
  integer_value ( unsigned int idx_n ) const;

  bool
  integers_equal () const;

  void
  set_integer ( unsigned int idx_n, long value_n );

  void
  set_integer_all ( long value_n );

  void
  level_integers ();

  // Decibel

  bool
  has_dB () const;

  /// @return < 0 on error
  int
  dB_range ( long * min_n, long * max_n ) const;

  long
  dB_value ( unsigned int idx_n ) const;

  long
  ask_dB_from_int ( long int_value_n ) const;

  long
  ask_int_from_dB ( long value_n, int round_dir_n = 0 ) const;

  // Enum type

  unsigned int
  enum_num_items () const;

  const char *
  enum_item_name ( unsigned int enum_idx_n ) const;

  QString
  enum_item_display_name ( unsigned int enum_idx_n ) const;

  unsigned int
  enum_index ( unsigned int channel_idx_n ) const;

  bool
  enum_idices_equal () const;

  void
  set_enum_index ( unsigned int channel_idx_n, unsigned int enum_idx_n );

  void
  set_enum_index_all ( unsigned int enum_idx_n );

  void
  level_enums ();

  // General

  bool
  values_equal () const;

  void
  level_values ();

  bool
  values_changed () const;

  /// @brief Reads all value from alsa
  void
  update_value ();

  // Callback methods

  /// @brief Reads all value from alsa and flags a change
  void
  update_value_mark ();

  void
  signalize_changes ();

  /// @brief Signalizes the parent that this element changed
  void
  signalize_element_changed ();

  // Alsa callbacks

  static int
  alsa_callback_hctl_elem ( snd_hctl_elem_t * elem_n, unsigned int mask_n );

  // Signals
  signals:

  /// @brief Gets emitted when a value was set
  void
  sig_values_changed ();

  // Protected methods
  protected:
  void
  value_was_set ();

  // Private attributes
  private:
  const Mixer_HCTL_Info_Db & _info_db;

  snd_hctl_t * _snd_hctl;
  snd_hctl_elem_t * _snd_hctl_elem;
  snd_ctl_elem_info_t * _snd_ctl_elem_info;
  snd_ctl_elem_value_t * _snd_ctl_elem_value;

  bool _values_changed;

  QString _display_name;
  Enum_Names_Buffer _enum_item_names;
};

inline snd_hctl_t *
Mixer_HCTL_Elem::snd_hctl () const
{
  return _snd_hctl;
}

inline snd_hctl_elem_t *
Mixer_HCTL_Elem::snd_hctl_elem () const
{
  return _snd_hctl_elem;
}

inline const snd_ctl_elem_info_t *
Mixer_HCTL_Elem::snd_ctl_info () const
{
  return _snd_ctl_elem_info;
}

inline const QString &
Mixer_HCTL_Elem::display_name () const
{
  return _display_name;
}

inline const char *
Mixer_HCTL_Elem::elem_name () const
{
  return snd_ctl_elem_info_get_name ( _snd_ctl_elem_info );
}

inline unsigned int
Mixer_HCTL_Elem::elem_numid () const
{
  return snd_ctl_elem_info_get_numid ( _snd_ctl_elem_info );
}

inline unsigned int
Mixer_HCTL_Elem::elem_index () const
{
  return snd_ctl_elem_info_get_index ( _snd_ctl_elem_info );
}

inline unsigned int
Mixer_HCTL_Elem::device () const
{
  return snd_ctl_elem_info_get_device ( _snd_ctl_elem_info );
}

inline unsigned int
Mixer_HCTL_Elem::subdevice () const
{
  return snd_ctl_elem_info_get_subdevice ( _snd_ctl_elem_info );
}

inline snd_ctl_elem_iface_t
Mixer_HCTL_Elem::iface () const
{
  return snd_ctl_elem_info_get_interface ( _snd_ctl_elem_info );
}

inline unsigned int
Mixer_HCTL_Elem::iface_type_idx () const
{
  return _info_db.iface_type_idx ( iface () );
}

inline const QString &
Mixer_HCTL_Elem::iface_name () const
{
  return _info_db.iface_name ( iface () );
}

inline const QString &
Mixer_HCTL_Elem::iface_display_name () const
{
  return _info_db.iface_display_name ( iface () );
}

inline unsigned int
Mixer_HCTL_Elem::count () const
{
  return snd_ctl_elem_info_get_count ( _snd_ctl_elem_info );
}

// Type

inline snd_ctl_elem_type_t
Mixer_HCTL_Elem::elem_type () const
{
  return snd_ctl_elem_info_get_type ( _snd_ctl_elem_info );
}

inline const QString &
Mixer_HCTL_Elem::elem_type_name () const
{
  return _info_db.elem_type_name ( elem_type () );
}

inline const QString &
Mixer_HCTL_Elem::elem_type_display_name () const
{
  return _info_db.elem_type_display_name ( elem_type () );
}

inline bool
Mixer_HCTL_Elem::is_boolean () const
{
  return ( elem_type () == SND_CTL_ELEM_TYPE_BOOLEAN );
}

inline bool
Mixer_HCTL_Elem::is_integer () const
{
  return ( elem_type () == SND_CTL_ELEM_TYPE_INTEGER );
}

inline bool
Mixer_HCTL_Elem::is_enumerated () const
{
  return ( elem_type () == SND_CTL_ELEM_TYPE_ENUMERATED );
}

inline bool
Mixer_HCTL_Elem::is_bytes () const
{
  return ( elem_type () == SND_CTL_ELEM_TYPE_BYTES );
}

inline bool
Mixer_HCTL_Elem::is_IEC958 () const
{
  return ( elem_type () == SND_CTL_ELEM_TYPE_IEC958 );
}

inline bool
Mixer_HCTL_Elem::is_integer64 () const
{
  return ( elem_type () == SND_CTL_ELEM_TYPE_INTEGER64 );
}

inline bool
Mixer_HCTL_Elem::is_volatile () const
{
  return snd_ctl_elem_info_is_volatile ( _snd_ctl_elem_info );
}

inline bool
Mixer_HCTL_Elem::is_readable () const
{
  return snd_ctl_elem_info_is_readable ( _snd_ctl_elem_info );
}

inline bool
Mixer_HCTL_Elem::is_writable () const
{
  return snd_ctl_elem_info_is_writable ( _snd_ctl_elem_info );
}

inline bool
Mixer_HCTL_Elem::is_active () const
{
  return ( snd_ctl_elem_info_is_inactive ( _snd_ctl_elem_info ) == 0 );
}

// Integer type

inline long
Mixer_HCTL_Elem::integer_min () const
{
  return snd_ctl_elem_info_get_min ( _snd_ctl_elem_info );
}

inline long
Mixer_HCTL_Elem::integer_max () const
{
  return snd_ctl_elem_info_get_max ( _snd_ctl_elem_info );
}

inline long
Mixer_HCTL_Elem::integer_value ( unsigned int idx_n ) const
{
  return snd_ctl_elem_value_get_integer ( _snd_ctl_elem_value, idx_n );
}

// Switch value

inline bool
Mixer_HCTL_Elem::switch_state ( unsigned int idx_n ) const
{
  return snd_ctl_elem_value_get_boolean ( _snd_ctl_elem_value, idx_n );
}

inline unsigned int
Mixer_HCTL_Elem::enum_num_items () const
{
  return snd_ctl_elem_info_get_items ( _snd_ctl_elem_info );
}

inline const char *
Mixer_HCTL_Elem::enum_item_name ( unsigned int enum_idx_n ) const
{
  snd_ctl_elem_info_set_item ( _snd_ctl_elem_info, enum_idx_n );
  snd_hctl_elem_info ( snd_hctl_elem (), _snd_ctl_elem_info );
  return snd_ctl_elem_info_get_item_name ( _snd_ctl_elem_info );
}

inline unsigned int
Mixer_HCTL_Elem::enum_index ( unsigned int channel_idx_n ) const
{
  return snd_ctl_elem_value_get_enumerated ( _snd_ctl_elem_value,
                                             channel_idx_n );
}

inline bool
Mixer_HCTL_Elem::values_changed () const
{
  return _values_changed;
}

} // namespace QSnd

#endif
