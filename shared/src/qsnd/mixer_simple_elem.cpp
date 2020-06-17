/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_simple_elem.hpp"
#include "event_types.hpp"
#include <QCoreApplication>
#include <QEvent>
#include <iostream>
#include <sstream>

namespace QSnd
{

Mixer_Simple_Elem::Mixer_Simple_Elem ( QObject * parent_n,
                                       snd_mixer_t * mixer_n,
                                       snd_mixer_selem_id_t * elem_id_n )
: QObject ( parent_n )
, _snd_mixer ( mixer_n )
, _values_changed ( false )
{
  snd_mixer_selem_id_malloc ( &_snd_mixer_selem_id );
  set_snd_mixer_selem_id ( elem_id_n );
}

Mixer_Simple_Elem::~Mixer_Simple_Elem ()
{
  snd_mixer_selem_id_free ( _snd_mixer_selem_id );
}

void
Mixer_Simple_Elem::clear ()
{
  _volumes_equal[ 0 ] = true;
  _volumes_equal[ 1 ] = true;

  _switches_equal[ 0 ] = true;
  _switches_equal[ 1 ] = true;

  _enums_equal[ 0 ] = true;
  _enums_equal[ 1 ] = true;

  _num_volume_channels[ 0 ] = 0;
  _num_volume_channels[ 1 ] = 0;

  _num_switch_channels[ 0 ] = 0;
  _num_switch_channels[ 1 ] = 0;

  _num_enum_channels[ 0 ] = 0;
  _num_enum_channels[ 1 ] = 0;

  _channels[ 0 ].clear ();
  _channels[ 1 ].clear ();

  _volume_min[ 0 ] = 0;
  _volume_min[ 1 ] = 0;

  _volume_max[ 0 ] = 0;
  _volume_max[ 1 ] = 0;

  _dB_min[ 0 ] = 0;
  _dB_min[ 1 ] = 0;

  _dB_max[ 0 ] = 0;
  _dB_max[ 1 ] = 0;

  _enum_num_items = 0;
  _enum_item_names.clear ();
}

unsigned int
Mixer_Simple_Elem::elem_index () const
{
  unsigned int res ( 0 );
  if ( snd_mixer_elem () != 0 ) {
    res = snd_mixer_selem_get_index ( snd_mixer_elem () );
  }
  return res;
}

const char *
Mixer_Simple_Elem::channel_name ( unsigned int snd_dir_n,
                                  unsigned int channel_idx_n ) const
{
  const char * res ( 0 );
  if ( channel_idx_n < _channels[ snd_dir_n ].size () ) {
    res = snd_mixer_selem_channel_name (
        _channels[ snd_dir_n ][ channel_idx_n ] );
  }
  return res;
}

void
Mixer_Simple_Elem::set_display_name ( const QString & name_n )
{
  _display_name = name_n;
}

void
Mixer_Simple_Elem::set_snd_mixer_selem_id ( snd_mixer_selem_id_t * selem_id_n )
{
  clear ();

  if ( selem_id_n == 0 ) {
    return;
  }

  snd_mixer_selem_id_copy ( _snd_mixer_selem_id, selem_id_n );
  snd_mixer_elem_t * snd_elem = snd_mixer_elem ();

  if ( snd_elem == 0 ) {
    return;
  }

  for ( int ii = 0; ii <= SND_MIXER_SCHN_LAST; ++ii ) {
    snd_mixer_selem_channel_id_t ch_id (
        static_cast< snd_mixer_selem_channel_id_t > ( ii ) );

    if ( snd_mixer_selem_has_playback_channel ( snd_elem, ch_id ) ) {
      _channels[ 0 ].push_back ( ch_id );
    }

    if ( snd_mixer_selem_has_capture_channel ( snd_elem, ch_id ) ) {
      _channels[ 1 ].push_back ( ch_id );
    }
  }

  if ( snd_mixer_selem_has_playback_volume ( snd_elem ) > 0 ) {
    const int snd_dir ( 0 );
    if ( _channels[ snd_dir ].size () > 0 ) {

      if ( snd_mixer_selem_has_playback_volume_joined ( snd_elem ) ) {
        _num_volume_channels[ snd_dir ] = 1;
      } else {
        _num_volume_channels[ snd_dir ] = _channels[ snd_dir ].size ();
      }

      snd_mixer_selem_get_playback_volume_range (
          snd_elem, &_volume_min[ snd_dir ], &_volume_max[ snd_dir ] );

      int err = snd_mixer_selem_get_playback_dB_range (
          snd_elem, &_dB_min[ snd_dir ], &_dB_max[ snd_dir ] );
      if ( err < 0 ) {
        _dB_min[ snd_dir ] = 0;
        _dB_max[ snd_dir ] = 0;
      }

    } else {

      QString msg (
          "[WW] %1 has a playback volume but no playback channels\n" );
      msg = msg.arg ( elem_name () );
      ::std::cerr << msg.toLocal8Bit ().constData ();
    }
  }

  if ( snd_mixer_selem_has_capture_volume ( snd_elem ) > 0 ) {
    const int snd_dir ( 1 );
    if ( _channels[ snd_dir ].size () > 0 ) {

      if ( snd_mixer_selem_has_capture_volume_joined ( snd_elem ) ) {
        _num_volume_channels[ snd_dir ] = 1;
      } else {
        _num_volume_channels[ snd_dir ] = _channels[ snd_dir ].size ();
      }

      snd_mixer_selem_get_capture_volume_range (
          snd_elem, &_volume_min[ snd_dir ], &_volume_max[ snd_dir ] );

      int err = snd_mixer_selem_get_capture_dB_range (
          snd_elem, &_dB_min[ snd_dir ], &_dB_max[ snd_dir ] );
      if ( err < 0 ) {
        _dB_min[ snd_dir ] = 0;
        _dB_max[ snd_dir ] = 0;
      }

    } else {

      QString msg ( "[WW] %1 has a capture volume but no capture channels\n" );
      msg = msg.arg ( elem_name () );
      ::std::cerr << msg.toLocal8Bit ().constData ();
    }
  }

  if ( snd_mixer_selem_has_playback_switch ( snd_elem ) > 0 ) {
    const int snd_dir ( 0 );
    if ( _channels[ snd_dir ].size () > 0 ) {

      if ( snd_mixer_selem_has_playback_switch_joined ( snd_elem ) ) {
        _num_switch_channels[ snd_dir ] = 1;
      } else {
        _num_switch_channels[ snd_dir ] = _channels[ snd_dir ].size ();
      }

    } else {

      QString msg (
          "[WW] %1 has a playback switch but no playback channels\n" );
      msg = msg.arg ( elem_name () );
      ::std::cerr << msg.toLocal8Bit ().constData ();
    }
  }

  if ( snd_mixer_selem_has_capture_switch ( snd_elem ) > 0 ) {
    const int snd_dir ( 1 );
    if ( _channels[ snd_dir ].size () > 0 ) {

      if ( snd_mixer_selem_has_capture_switch_joined ( snd_elem ) ) {
        _num_switch_channels[ snd_dir ] = 1;
      } else {
        _num_switch_channels[ snd_dir ] = _channels[ snd_dir ].size ();
      }

    } else {

      QString msg ( "[WW] %1 has a capture switch but no capture channels\n" );
      msg = msg.arg ( elem_name () );
      ::std::cerr << msg.toLocal8Bit ().constData ();
    }
  }

  // Enumerated
  if ( snd_mixer_selem_is_enumerated ( snd_elem ) ) {

    bool is_play ( snd_mixer_selem_is_enum_playback ( snd_elem ) > 0 );
    bool is_cap ( snd_mixer_selem_is_enum_capture ( snd_elem ) > 0 );

    if ( !( is_play || is_cap ) ) {
      // Workaround
      // Try to determine the direction from the name
      QString fstr ( elem_name () );
      fstr = fstr.toLower ();
      int fidx ( fstr.indexOf ( "mic" ) );
      if ( fidx < 0 ) {
        fidx = fstr.indexOf ( "adc " );
      }
      if ( fidx < 0 ) {
        fidx = fstr.indexOf ( "capture" );
      }
      if ( fidx < 0 ) {
        fidx = fstr.indexOf ( "input source" );
      }
      if ( fidx >= 0 ) {
        is_cap = true;
      }

      if ( !is_cap ) {
        if ( _channels[ 0 ].size () > 0 ) {
          is_play = true;
        } else if ( _channels[ 0 ].size () > 0 ) {
          is_cap = true;
        } else {
          is_play = true;
        }
      }
    }

    // Changing the channels for broken alsa implementations
    if ( is_play ) {
      _num_enum_channels[ 0 ] = 1;
      _channels[ 0 ].resize ( 1, SND_MIXER_SCHN_MONO );
    }
    if ( is_cap ) {
      _num_enum_channels[ 1 ] = 1;
      _channels[ 1 ].resize ( 1, SND_MIXER_SCHN_MONO );
    }

    // Read enum item names
    const int num_items ( snd_mixer_selem_get_enum_items ( snd_elem ) );
    if ( num_items > 0 ) {
      _enum_num_items = num_items;
      _enum_item_names.resize ( _enum_num_items );

      const unsigned int buff_size ( 256 );
      char buff[ buff_size ];

      for ( int ii = 0; ii < num_items; ++ii ) {
        int err = snd_mixer_selem_get_enum_item_name (
            snd_elem, ii, buff_size, buff );
        if ( err >= 0 ) {
          _enum_item_names[ ii ] =
              QCoreApplication::translate ( "ALSA::Enum_Value", buff );
        } else {
          print_alsa_error ( "snd_mixer_selem_get_enum_item_name", err );
        }
      }
    }
  }

  update_values_mark ();
}

// State flags

bool
Mixer_Simple_Elem::is_active () const
{
  return ( snd_mixer_selem_is_active ( snd_mixer_elem () ) != 0 );
}

// Joined feature

bool
Mixer_Simple_Elem::volume_joined ( unsigned int snd_dir_n ) const
{
  bool res ( false );

  snd_mixer_elem_t * snd_elem ( snd_mixer_elem () );
  if ( snd_dir_n == 0 ) {
    res = ( snd_mixer_selem_has_playback_volume_joined ( snd_elem ) != 0 );
  } else {
    res = ( snd_mixer_selem_has_capture_volume_joined ( snd_elem ) != 0 );
  }

  return res;
}

bool
Mixer_Simple_Elem::switch_joined ( unsigned int snd_dir_n ) const
{
  bool res ( false );

  snd_mixer_elem_t * snd_elem ( snd_mixer_elem () );
  if ( snd_dir_n == 0 ) {
    res = ( snd_mixer_selem_has_playback_switch_joined ( snd_elem ) != 0 );
  } else {
    res = ( snd_mixer_selem_has_capture_switch_joined ( snd_elem ) != 0 );
  }

  return res;
}

bool
Mixer_Simple_Elem::enum_joined ( unsigned int ) const
{
  return true;
}

unsigned int
Mixer_Simple_Elem::num_volume_channels ( unsigned int snd_dir_n ) const
{
  return _num_volume_channels[ snd_dir_n ];
}

long
Mixer_Simple_Elem::volume ( unsigned int snd_dir_n,
                            unsigned int channel_idx_n ) const
{
  long val ( 0 );

  snd_mixer_elem_t * snd_elem ( snd_mixer_elem () );
  if ( snd_dir_n == 0 ) {
    int err ( snd_mixer_selem_get_playback_volume (
        snd_elem, _channels[ 0 ][ channel_idx_n ], &val ) );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_get_playback_volume", err );
    }
  } else {
    int err ( snd_mixer_selem_get_capture_volume (
        snd_elem, _channels[ 1 ][ channel_idx_n ], &val ) );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_get_capture_volume", err );
    }
  }

  return val;
}

void
Mixer_Simple_Elem::set_volume ( unsigned int snd_dir_n,
                                int channel_idx_n,
                                long volume_n )
{
  int err;
  snd_mixer_elem_t * snd_elem ( snd_mixer_elem () );

  //::std::cout << "Mixer_Simple_Elem::set_volume " << volume_n << "\n";

  if ( snd_dir_n == 0 ) {
    err = snd_mixer_selem_set_playback_volume (
        snd_elem, _channels[ 0 ][ channel_idx_n ], volume_n );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_set_playback_volume", err );
    }
  } else {
    err = snd_mixer_selem_set_capture_volume (
        snd_elem, _channels[ 1 ][ channel_idx_n ], volume_n );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_set_capture_volume", err );
    }
  }

  value_was_set ();
}

void
Mixer_Simple_Elem::set_volume_all ( unsigned int snd_dir_n, long volume_n )
{
  int err;
  snd_mixer_elem_t * snd_elem ( snd_mixer_elem () );

  //::std::cout << "Mixer_Simple_Elem::set_volume_all " << volume_n << "\n";

  if ( snd_dir_n == 0 ) {
    err = snd_mixer_selem_set_playback_volume_all ( snd_elem, volume_n );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_set_playback_volume_all", err );
    }
  } else {
    err = snd_mixer_selem_set_capture_volume_all ( snd_elem, volume_n );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_set_capture_volume_all", err );
    }
  }

  value_was_set ();
}

void
Mixer_Simple_Elem::level_volumes ( unsigned int snd_dir_n )
{
  const long num_volumes ( _num_volume_channels[ snd_dir_n ] );
  if ( volumes_equal ( snd_dir_n ) || ( num_volumes < 2 ) ) {
    return;
  }

  snd_mixer_elem_t * snd_elem ( snd_mixer_elem () );
  int ( *vol_func ) (
      snd_mixer_elem_t *, snd_mixer_selem_channel_id_t, long * );
  if ( snd_dir_n == 0 ) {
    vol_func = &snd_mixer_selem_get_playback_volume;
  } else {
    vol_func = &snd_mixer_selem_get_capture_volume;
  }

  long vol_avrg_accu ( 0 );
  long vol_mod_accu ( 0 );

  for ( int ii = 0; ii < num_volumes; ++ii ) {
    long val ( 0 );
    vol_func ( snd_elem, _channels[ snd_dir_n ][ ii ], &val );

    long vol_mod;
    if ( ( val >= num_volumes ) || ( val <= ( -num_volumes ) ) ) {
      const long vol_avrg ( val / num_volumes );
      vol_avrg_accu += vol_avrg;
      vol_mod = val;
      vol_mod -= vol_avrg * ( num_volumes - 1 ); // To prevent an overflow
      vol_mod -= vol_avrg;
    } else {
      vol_mod = val;
    }

    vol_mod_accu += vol_mod;

    if ( ( vol_mod_accu >= num_volumes ) ||
         ( vol_mod_accu <= ( -num_volumes ) ) ) {
      const long vol_avrg ( vol_mod_accu / num_volumes );
      vol_avrg_accu += vol_avrg;
      vol_mod_accu -= ( vol_avrg * num_volumes );
    }
  }

  set_volume_all ( snd_dir_n, vol_avrg_accu );
}

long
Mixer_Simple_Elem::ask_dB_vol_nearest ( unsigned int snd_dir_n,
                                        long dB_value_n )
{
  long res ( 0 );

  const long vol_above ( ask_dB_vol ( snd_dir_n, dB_value_n, 1 ) );
  const long vol_below ( ask_dB_vol ( snd_dir_n, dB_value_n, -1 ) );
  long db_above ( ask_vol_dB ( snd_dir_n, vol_above ) );
  long db_below ( ask_vol_dB ( snd_dir_n, vol_below ) );

  db_above = ::std::abs ( db_above - dB_value_n );
  db_below = ::std::abs ( dB_value_n - db_below );

  if ( db_above > db_below ) {
    res = vol_below;
  } else {
    res = vol_above;
  }

  return res;
}

long
Mixer_Simple_Elem::dB_value ( unsigned int snd_dir_n,
                              unsigned int channel_idx_n ) const
{
  long val ( 0 );

  snd_mixer_elem_t * snd_elem ( snd_mixer_elem () );
  if ( snd_dir_n == 0 ) {
    int err ( snd_mixer_selem_get_playback_dB (
        snd_elem, _channels[ 0 ][ channel_idx_n ], &val ) );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_get_playback_dB", err );
    }
  } else {
    int err ( snd_mixer_selem_get_capture_dB (
        snd_elem, _channels[ 1 ][ channel_idx_n ], &val ) );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_get_capture_dB", err );
    }
  }

  return val;
}

void
Mixer_Simple_Elem::set_dB ( unsigned int snd_dir_n,
                            int channel_idx_n,
                            long dB_val_n,
                            int dir_n )
{
  snd_mixer_elem_t * snd_elem ( snd_mixer_elem () );

  //::std::cout << "Mixer_Simple_Elem::set_dB " << dB_val_n << "\n";

  if ( snd_dir_n == 0 ) {
    int err = snd_mixer_selem_set_playback_dB (
        snd_elem, _channels[ 0 ][ channel_idx_n ], dB_val_n, dir_n );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_set_playback_dB", err );
    }
  } else {
    int err = snd_mixer_selem_set_capture_dB (
        snd_elem, _channels[ 1 ][ channel_idx_n ], dB_val_n, dir_n );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_set_capture_dB", err );
    }
  }

  value_was_set ();
}

void
Mixer_Simple_Elem::set_dB_all ( unsigned int snd_dir_n,
                                long dB_val_n,
                                int dir_n )
{
  snd_mixer_elem_t * snd_elem ( snd_mixer_elem () );

  //::std::cout << "Mixer_Simple_Elem::set_dB_all " << dB_val_n << "\n";

  if ( snd_dir_n == 0 ) {
    int err = snd_mixer_selem_set_playback_dB_all ( snd_elem, dB_val_n, dir_n );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_set_playback_dB_all", err );
    }
  } else {
    int err = snd_mixer_selem_set_capture_dB_all ( snd_elem, dB_val_n, dir_n );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_set_capture_dB_all", err );
    }
  }

  value_was_set ();
}

unsigned int
Mixer_Simple_Elem::num_switch_channels ( unsigned int snd_dir_n ) const
{
  return _num_switch_channels[ snd_dir_n ];
}

bool
Mixer_Simple_Elem::switch_state ( unsigned int snd_dir_n,
                                  unsigned int channel_idx_n ) const
{
  int val ( 0 );

  snd_mixer_elem_t * snd_elem ( snd_mixer_elem () );
  if ( snd_dir_n == 0 ) {
    int err ( snd_mixer_selem_get_playback_switch (
        snd_elem, _channels[ 0 ][ channel_idx_n ], &val ) );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_get_playback_switch", err );
    }
  } else {
    int err ( snd_mixer_selem_get_capture_switch (
        snd_elem, _channels[ 1 ][ channel_idx_n ], &val ) );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_get_capture_switch", err );
    }
  }

  return ( val != 0 );
}

void
Mixer_Simple_Elem::set_switch ( unsigned int snd_dir_n,
                                int channel_idx_n,
                                bool state_n )
{
  snd_mixer_elem_t * snd_elem ( snd_mixer_elem () );

  //::std::cout << "Mixer_Simple_Elem::set_switch " << state_n << "\n";

  if ( snd_dir_n == 0 ) {
    int err = snd_mixer_selem_set_playback_switch (
        snd_elem, _channels[ 0 ][ channel_idx_n ], state_n );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_set_playback_switch", err );
    }
  } else {
    int err = snd_mixer_selem_set_capture_switch (
        snd_elem, _channels[ 1 ][ channel_idx_n ], state_n );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_set_capture_switch", err );
    }
  }

  value_was_set ();
}

void
Mixer_Simple_Elem::set_switch_all ( unsigned int snd_dir_n, bool state_n )
{
  snd_mixer_elem_t * snd_elem ( snd_mixer_elem () );

  //::std::cout << "Mixer_Simple_Elem::set_switch_all " << state_n << "\n";

  if ( snd_dir_n == 0 ) {
    int err = snd_mixer_selem_set_playback_switch_all ( snd_elem, state_n );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_set_playback_switch_all", err );
    }
  } else {
    int err = snd_mixer_selem_set_capture_switch_all ( snd_elem, state_n );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_set_capture_switch_all", err );
    }
  }

  value_was_set ();
}

void
Mixer_Simple_Elem::invert_switches ( unsigned int snd_dir_n )
{
  snd_mixer_elem_t * snd_elem ( snd_mixer_elem () );

  if ( snd_dir_n == 0 ) {
    for ( unsigned int cii = 0; cii < _num_switch_channels[ 0 ]; ++cii ) {
      int err = snd_mixer_selem_set_playback_switch (
          snd_elem, _channels[ 0 ][ cii ], !switch_state ( 0, cii ) );
      if ( err < 0 ) {
        print_alsa_error ( "snd_mixer_selem_set_playback_switch", err );
      }
    }
  } else {
    for ( unsigned int cii = 0; cii < _num_switch_channels[ 1 ]; ++cii ) {
      int err = snd_mixer_selem_set_capture_switch (
          snd_elem, _channels[ 1 ][ cii ], !switch_state ( 1, cii ) );
      if ( err < 0 ) {
        print_alsa_error ( "snd_mixer_selem_set_capture_switch", err );
      }
    }
  }

  value_was_set ();
}

void
Mixer_Simple_Elem::level_switches ( unsigned int snd_dir_n )
{
  const unsigned int num_switches ( _num_switch_channels[ snd_dir_n ] );
  if ( switches_equal ( snd_dir_n ) || ( num_switches < 2 ) ) {
    return;
  }

  unsigned int num_on ( 0 );
  unsigned int num_off ( 0 );

  {
    snd_mixer_elem_t * snd_elem ( snd_mixer_elem () );
    int ( *state_func ) (
        snd_mixer_elem_t *, snd_mixer_selem_channel_id_t, int * );
    if ( snd_dir_n == 0 ) {
      state_func = &snd_mixer_selem_get_playback_switch;
    } else {
      state_func = &snd_mixer_selem_get_capture_switch;
    }

    for ( unsigned int ii = 0; ii < num_switches; ++ii ) {
      int val ( 0 );
      state_func ( snd_elem, _channels[ snd_dir_n ][ ii ], &val );
      if ( val != 0 ) {
        ++num_on;
      } else {
        ++num_off;
      }
    }
  }

  set_switch_all ( snd_dir_n, ( num_on >= num_off ) );
}

unsigned int
Mixer_Simple_Elem::num_enum_channels ( unsigned int snd_dir_n ) const
{
  return _num_enum_channels[ snd_dir_n ];
}

unsigned int
Mixer_Simple_Elem::enum_index ( unsigned int snd_dir_n,
                                unsigned int channel_idx_n ) const
{
  unsigned int val ( 0 );

  snd_mixer_elem_t * snd_elem ( snd_mixer_elem () );
  int err ( snd_mixer_selem_get_enum_item (
      snd_elem, _channels[ snd_dir_n ][ channel_idx_n ], &val ) );
  if ( err < 0 ) {
    print_alsa_error ( "snd_mixer_selem_get_enum_item", err );
  }

  return val;
}

void
Mixer_Simple_Elem::set_enum_index ( unsigned int snd_dir_n,
                                    int channel_idx_n,
                                    unsigned int index_n )
{
  snd_mixer_elem_t * snd_elem ( snd_mixer_elem () );

  //::std::cout << "Mixer_Simple_Elem::set_enum_index " << index_n << "\n";

  int err = snd_mixer_selem_set_enum_item (
      snd_elem, _channels[ snd_dir_n ][ channel_idx_n ], index_n );
  if ( err < 0 ) {
    print_alsa_error ( "snd_mixer_selem_set_enum_item", err );
  }

  value_was_set ();
}

void
Mixer_Simple_Elem::set_enum_index_all ( unsigned int snd_dir_n,
                                        unsigned int index_n )
{
  snd_mixer_elem_t * snd_elem ( snd_mixer_elem () );

  //::std::cout << "Mixer_Simple_Elem::set_enum_index_all " << index_n << "\n";

  for ( unsigned int ii = 0; ii < _num_enum_channels[ snd_dir_n ]; ++ii ) {
    int err = snd_mixer_selem_set_enum_item (
        snd_elem, _channels[ snd_dir_n ][ ii ], index_n );
    if ( err < 0 ) {
      print_alsa_error ( "snd_mixer_selem_set_enum_item", err );
    }
  }

  value_was_set ();
}

void
Mixer_Simple_Elem::value_was_set ()
{
  // Read values back
  update_values_mark ();

  if ( parent () != 0 ) {
    QEvent ev_req ( ::QSnd::evt_update_values_request );
    QCoreApplication::sendEvent ( parent (), &ev_req );
  }
}

void
Mixer_Simple_Elem::update_values ()
{
  snd_mixer_elem_t * snd_elem = snd_mixer_elem ();
  if ( snd_elem == 0 ) {
    return;
  }

  //::std::cout << "Mixer_Simple_Elem::update_mixer_values" << "\n";

  // Playback volume equality
  if ( _num_volume_channels[ 0 ] > 1 ) {
    const Channel_Buffer & chs ( _channels[ 0 ] );
    bool vols_equal ( false );

    long val_first ( 0 );
    int err ( snd_mixer_selem_get_playback_volume (
        snd_elem, chs[ 0 ], &val_first ) );

    if ( err == 0 ) {
      vols_equal = true;

      const unsigned int num_ch ( _num_volume_channels[ 0 ] );
      for ( unsigned int ii = 1; ii < num_ch; ++ii ) {
        long val ( 0 );
        err = snd_mixer_selem_get_playback_volume ( snd_elem, chs[ ii ], &val );

        if ( ( err < 0 ) || ( val != val_first ) ) {
          vols_equal = false;
          break;
        }
      }
    }

    _volumes_equal[ 0 ] = vols_equal;
  }

  // Capture volume equality
  if ( _num_volume_channels[ 1 ] > 1 ) {
    const Channel_Buffer & chs ( _channels[ 1 ] );
    bool vols_equal ( false );

    long val_first ( 0 );
    int err (
        snd_mixer_selem_get_capture_volume ( snd_elem, chs[ 0 ], &val_first ) );

    if ( err == 0 ) {
      vols_equal = true;

      const unsigned int num_ch ( _num_volume_channels[ 1 ] );
      for ( unsigned int ii = 1; ii < num_ch; ++ii ) {
        long val ( 0 );
        err = snd_mixer_selem_get_capture_volume ( snd_elem, chs[ ii ], &val );

        if ( ( err < 0 ) || ( val != val_first ) ) {
          vols_equal = false;
          break;
        }
      }
    }

    _volumes_equal[ 1 ] = vols_equal;
  }

  // Playback switch equality
  if ( _num_switch_channels[ 0 ] > 1 ) {
    const Channel_Buffer & chs ( _channels[ 0 ] );
    bool states_equal ( false );

    int state_first ( 0 );
    int err ( snd_mixer_selem_get_playback_switch (
        snd_elem, chs[ 0 ], &state_first ) );

    if ( err == 0 ) {
      states_equal = true;

      const unsigned int num_ch ( _num_switch_channels[ 0 ] );
      for ( unsigned int ii = 1; ii < num_ch; ++ii ) {
        int state ( 0 );
        err =
            snd_mixer_selem_get_playback_switch ( snd_elem, chs[ ii ], &state );

        if ( ( err < 0 ) || ( state != state_first ) ) {
          states_equal = false;
          break;
        }
      }
    }

    _switches_equal[ 0 ] = states_equal;
  }

  // Capture switch equality
  if ( _num_switch_channels[ 1 ] > 1 ) {
    const Channel_Buffer & chs ( _channels[ 1 ] );
    bool states_equal ( false );

    int state_first ( 0 );
    int err ( snd_mixer_selem_get_capture_switch (
        snd_elem, chs[ 0 ], &state_first ) );

    if ( err == 0 ) {
      states_equal = true;

      const unsigned int num_ch ( _num_switch_channels[ 1 ] );
      for ( unsigned int ii = 1; ii < num_ch; ++ii ) {
        int state ( 0 );
        err =
            snd_mixer_selem_get_capture_switch ( snd_elem, chs[ ii ], &state );

        if ( ( err < 0 ) || ( state != state_first ) ) {
          states_equal = false;
          break;
        }
      }
    }

    _switches_equal[ 1 ] = states_equal;
  }

  // Playback enum equality
  if ( _num_enum_channels[ 0 ] > 1 ) {
    const Channel_Buffer & chs ( _channels[ 0 ] );
    bool indices_equal ( false );

    unsigned int idx_first ( 0 );
    int err (
        snd_mixer_selem_get_enum_item ( snd_elem, chs[ 0 ], &idx_first ) );

    if ( err == 0 ) {
      indices_equal = true;

      const unsigned int num_ch ( _num_enum_channels[ 0 ] );
      for ( unsigned int ii = 1; ii < num_ch; ++ii ) {
        unsigned int idx ( 0 );
        err = snd_mixer_selem_get_enum_item ( snd_elem, chs[ ii ], &idx );

        if ( ( err < 0 ) || ( idx != idx_first ) ) {
          indices_equal = false;
          break;
        }
      }
    }

    _enums_equal[ 0 ] = indices_equal;
  }

  // Capture enum equality
  if ( _num_enum_channels[ 1 ] > 1 ) {
    const Channel_Buffer & chs ( _channels[ 1 ] );
    bool indices_equal ( false );

    unsigned int idx_first ( 0 );
    int err (
        snd_mixer_selem_get_enum_item ( snd_elem, chs[ 0 ], &idx_first ) );

    if ( err == 0 ) {
      indices_equal = true;

      const unsigned int num_ch ( _num_enum_channels[ 1 ] );
      for ( unsigned int ii = 1; ii < num_ch; ++ii ) {
        unsigned int idx ( 0 );
        err = snd_mixer_selem_get_enum_item ( snd_elem, chs[ ii ], &idx );

        if ( ( err < 0 ) || ( idx != idx_first ) ) {
          indices_equal = false;
          break;
        }
      }
    }

    _enums_equal[ 1 ] = indices_equal;
  }
}

void
Mixer_Simple_Elem::update_values_mark ()
{
  _values_changed = true;
  update_values ();
}

void
Mixer_Simple_Elem::signalize_changes ()
{
  if ( _values_changed ) {
    _values_changed = false;
    emit sig_values_changed ();
  }
}

void
Mixer_Simple_Elem::signalize_element_changed ()
{
  if ( parent () != 0 ) {
    QEvent ev_req ( ::QSnd::evt_reload_request );
    QCoreApplication::sendEvent ( parent (), &ev_req );
  }
}

// Alsa callbacks

int
Mixer_Simple_Elem::alsa_callback_mixer_elem ( snd_mixer_elem_t * elem_n,
                                              unsigned int mask_n )
{
  int res ( 0 );

  Mixer_Simple_Elem * smse;
  {
    void * priv ( snd_mixer_elem_get_callback_private ( elem_n ) );
    smse = reinterpret_cast< Mixer_Simple_Elem * > ( priv );
  }

  if ( smse != 0 ) {
    const unsigned int change_mask ( SND_CTL_EVENT_MASK_INFO |
                                     SND_CTL_EVENT_MASK_ADD |
                                     SND_CTL_EVENT_MASK_TLV );

    if ( ( mask_n == SND_CTL_EVENT_MASK_REMOVE ) ||
         ( ( mask_n & change_mask ) != 0 ) ) {
      smse->signalize_element_changed ();
    } else if ( ( mask_n & SND_CTL_EVENT_MASK_VALUE ) != 0 ) {
      smse->update_values_mark ();
    } else {
      // Unusual mask
      {
        ::std::stringstream msg;
        msg << "Mixer_Simple_Elem::alsa_callback_mixer_elem: ";
        msg << "Unknown mask ( " << mask_n << " )" << ::std::endl;
        ::std::cerr << msg.str ();
      }
      res = -1;
    }
  }

  return res;
}

} // namespace QSnd
