/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_mixer_simple_elem_hpp__
#define __INC_qsnd_mixer_simple_elem_hpp__

#include "qsnd/alsa.hpp"
#include <QObject>
#include <QString>
#include <vector>

namespace QSnd
{

/// @brief Mixer_Simple_Elem
///
/// Connects an ALSA simple mixer element with Qt
///
class Mixer_Simple_Elem : public QObject
{
  Q_OBJECT

  // Public typedefs
  public:
  typedef std::vector< snd_mixer_selem_channel_id_t > Channel_Buffer;
  typedef std::vector< QString > Enum_Names_Buffer;

  // Public methods
  public:
  Mixer_Simple_Elem ( QObject * parent_n = 0,
                      snd_mixer_t * mixer_n = 0,
                      snd_mixer_selem_id_t * elem_id_n = 0 );

  ~Mixer_Simple_Elem ();

  void
  clear ();

  // Alsa snd_mixer

  snd_mixer_t *
  snd_mixer () const;

  void
  set_snd_mixer ( snd_mixer_t * mixer_n );

  // Alsa set_snd_mixer_selem_id

  void
  set_snd_mixer_selem_id ( snd_mixer_selem_id_t * selem_id_n );

  snd_mixer_selem_id_t *
  snd_mixer_selem_id () const;

  // Alsa snd_mixer_elem_t

  snd_mixer_elem_t *
  snd_mixer_elem () const;

  const QString &
  display_name () const;

  void
  set_display_name ( const QString & name_n );

  const char *
  elem_name () const;

  unsigned int
  elem_index () const;

  unsigned int
  num_channels ( unsigned int snd_dir_n ) const;

  snd_mixer_selem_channel_id_t
  channel ( unsigned int snd_dir_n, unsigned int channel_idx_n ) const;

  const char *
  channel_name ( unsigned int snd_dir_n, unsigned int channel_idx_n ) const;

  // Availability

  bool
  has_volume ( unsigned int snd_dir_n ) const;

  bool
  has_dB ( unsigned int snd_dir_n ) const;

  bool
  has_switch ( unsigned int snd_dir_n ) const;

  bool
  has_enum ( unsigned int snd_dir_n ) const;

  // State flags

  bool
  is_active () const;

  // Joined feature

  bool
  volume_joined ( unsigned int snd_dir_n ) const;

  bool
  switch_joined ( unsigned int snd_dir_n ) const;

  bool
  enum_joined ( unsigned int snd_dir_n ) const;

  // Equality

  bool
  volumes_equal ( unsigned int snd_dir_n ) const;

  bool
  switches_equal ( unsigned int snd_dir_n ) const;

  bool
  enums_equal ( unsigned int snd_dir_n ) const;

  // Volume

  unsigned int
  num_volume_channels ( unsigned int snd_dir_n ) const;

  long
  volume ( unsigned int snd_dir_n, unsigned int channel_idx_n ) const;

  long
  volume_min ( unsigned int snd_dir_n ) const;

  long
  volume_max ( unsigned int snd_dir_n ) const;

  // Decibel

  long
  dB_value ( unsigned int snd_dir_n, unsigned int channel_idx_n ) const;

  long
  dB_min ( unsigned int snd_dir_n ) const;

  long
  dB_max ( unsigned int snd_dir_n ) const;

  long
  ask_dB_vol ( unsigned int snd_dir_n, long dB_value_n, int dir_n = -1 );

  long
  ask_vol_dB ( unsigned int snd_dir_n, long volume_n );

  long
  ask_dB_vol_nearest ( unsigned int snd_dir_n, long dB_value_n );

  // Switch

  unsigned int
  num_switch_channels ( unsigned int snd_dir_n ) const;

  bool
  switch_state ( unsigned int snd_dir_n, unsigned int channel_idx_n ) const;

  // Enumerated

  unsigned int
  num_enum_channels ( unsigned int snd_dir_n ) const;

  unsigned int
  enum_index ( unsigned int snd_dir_n, unsigned int channel_idx_n ) const;

  unsigned int
  enum_num_items () const;

  const Enum_Names_Buffer &
  enum_item_names () const;

  // Volume setting

  void
  set_volume ( unsigned int snd_dir_n, int channel_idx_n, long volume_n );

  void
  set_volume_all ( unsigned int snd_dir_n, long volume_n );

  void
  level_volumes ( unsigned int snd_dir_n );

  // Decibel setting

  void
  set_dB ( unsigned int snd_dir_n,
           int channel_idx_n,
           long dB_val_n,
           int dir_n = -1 );

  void
  set_dB_all ( unsigned int snd_dir_n, long dB_val_n, int dir_n = -1 );

  // Switch setting

  void
  set_switch ( unsigned int snd_dir_n, int channel_idx_n, bool index_n );

  void
  set_switch_all ( unsigned int snd_dir_n, bool index_n );

  void
  invert_switches ( unsigned int snd_dir_n );

  void
  level_switches ( unsigned int snd_dir_n );

  // Enum setting

  void
  set_enum_index ( unsigned int snd_dir_n,
                   int channel_idx_n,
                   unsigned int index_n );

  void
  set_enum_index_all ( unsigned int snd_dir_n, unsigned int index_n );

  /// @brief Reads all values from alsa
  void
  update_values ();

  // Callback methods

  /// @brief Reads all values from alsa and marks a change
  void
  update_values_mark ();

  void
  signalize_changes ();

  /// @brief Signalizes the parent that this element changed
  void
  signalize_element_changed ();

  // Alsa callbacks

  static int
  alsa_callback_mixer_elem ( snd_mixer_elem_t * elem_n, unsigned int mask_n );

  // Signals
  signals:

  /// @brief Gets emitted when values changed
  void
  sig_values_changed ();

  // Protected methods
  protected:
  void
  value_was_set ();

  // Private attributes
  private:
  snd_mixer_t * _snd_mixer;

  snd_mixer_selem_id_t * _snd_mixer_selem_id;

  QString _display_name;

  bool _values_changed;

  bool _volumes_equal[ 2 ];
  bool _switches_equal[ 2 ];
  bool _enums_equal[ 2 ];

  unsigned int _num_volume_channels[ 2 ];
  unsigned int _num_switch_channels[ 2 ];
  unsigned int _num_enum_channels[ 2 ];

  Channel_Buffer _channels[ 2 ];

  long _volume_min[ 2 ];
  long _volume_max[ 2 ];

  long _dB_min[ 2 ];
  long _dB_max[ 2 ];

  unsigned int _enum_num_items;

  Enum_Names_Buffer _enum_item_names;
};

inline snd_mixer_t *
Mixer_Simple_Elem::snd_mixer () const
{
  return _snd_mixer;
}

inline snd_mixer_selem_id_t *
Mixer_Simple_Elem::snd_mixer_selem_id () const
{
  return _snd_mixer_selem_id;
}

inline snd_mixer_elem_t *
Mixer_Simple_Elem::snd_mixer_elem () const
{
  return snd_mixer_find_selem ( snd_mixer (), snd_mixer_selem_id () );
}

inline void
Mixer_Simple_Elem::set_snd_mixer ( snd_mixer_t * mixer_n )
{
  _snd_mixer = mixer_n;
}

inline const QString &
Mixer_Simple_Elem::display_name () const
{
  return _display_name;
}

inline const char *
Mixer_Simple_Elem::elem_name () const
{

  return snd_mixer_selem_get_name ( snd_mixer_elem () );
}

inline snd_mixer_selem_channel_id_t
Mixer_Simple_Elem::channel ( unsigned int snd_dir_n,
                             unsigned int channel_idx_n ) const
{
  return _channels[ snd_dir_n ][ channel_idx_n ];
}

inline unsigned int
Mixer_Simple_Elem::num_channels ( unsigned int snd_dir_n ) const
{
  return _channels[ snd_dir_n ].size ();
}

// Availability

inline bool
Mixer_Simple_Elem::has_volume ( unsigned int snd_dir_n ) const
{
  return ( _num_volume_channels[ snd_dir_n ] > 0 );
}

inline bool
Mixer_Simple_Elem::has_dB ( unsigned int snd_dir_n ) const
{
  return ( _dB_min[ snd_dir_n ] != _dB_max[ snd_dir_n ] );
}

inline bool
Mixer_Simple_Elem::has_switch ( unsigned int snd_dir_n ) const
{
  return ( _num_switch_channels[ snd_dir_n ] > 0 );
}

inline bool
Mixer_Simple_Elem::has_enum ( unsigned int snd_dir_n ) const
{
  return ( _num_enum_channels[ snd_dir_n ] > 0 );
}

// Equality

inline bool
Mixer_Simple_Elem::volumes_equal ( unsigned int snd_dir_n ) const
{
  return _volumes_equal[ snd_dir_n ];
}

inline bool
Mixer_Simple_Elem::switches_equal ( unsigned int snd_dir_n ) const
{
  return _switches_equal[ snd_dir_n ];
}

inline bool
Mixer_Simple_Elem::enums_equal ( unsigned int snd_dir_n ) const
{
  return _enums_equal[ snd_dir_n ];
}

inline long
Mixer_Simple_Elem::volume_min ( unsigned int snd_dir_n ) const
{
  return _volume_min[ snd_dir_n ];
}

inline long
Mixer_Simple_Elem::volume_max ( unsigned int snd_dir_n ) const
{
  return _volume_max[ snd_dir_n ];
}

inline long
Mixer_Simple_Elem::dB_min ( unsigned int snd_dir_n ) const
{
  return _dB_min[ snd_dir_n ];
}

inline long
Mixer_Simple_Elem::dB_max ( unsigned int snd_dir_n ) const
{
  return _dB_max[ snd_dir_n ];
}

inline long
Mixer_Simple_Elem::ask_dB_vol ( unsigned int snd_dir_n,
                                long dB_value_n,
                                int dir_n )
{
  long res ( 0 );
  if ( snd_dir_n == 0 ) {
    snd_mixer_selem_ask_playback_dB_vol (
        snd_mixer_elem (), dB_value_n, dir_n, &res );
  } else {
    snd_mixer_selem_ask_capture_dB_vol (
        snd_mixer_elem (), dB_value_n, dir_n, &res );
  }
  return res;
}

inline long
Mixer_Simple_Elem::ask_vol_dB ( unsigned int snd_dir_n, long volume_n )
{
  long res ( 0 );
  if ( snd_dir_n == 0 ) {
    snd_mixer_selem_ask_playback_vol_dB ( snd_mixer_elem (), volume_n, &res );
  } else {
    snd_mixer_selem_ask_capture_vol_dB ( snd_mixer_elem (), volume_n, &res );
  }
  return res;
}

inline unsigned int
Mixer_Simple_Elem::enum_num_items () const
{
  return _enum_num_items;
}

inline const Mixer_Simple_Elem::Enum_Names_Buffer &
Mixer_Simple_Elem::enum_item_names () const
{
  return _enum_item_names;
}

} // namespace QSnd

#endif
