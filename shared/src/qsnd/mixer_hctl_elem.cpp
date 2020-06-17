/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl_elem.hpp"
#include "qsnd/event_types.hpp"
#include <QCoreApplication>
#include <QEvent>
#include <iostream>
#include <sstream>

namespace QSnd
{

Mixer_HCTL_Elem::Mixer_HCTL_Elem ( const Mixer_HCTL_Info_Db & info_db_n,
                                   QObject * parent_n,
                                   snd_hctl_t * hctl_n,
                                   snd_hctl_elem_t * hctl_elem_n )
: QObject ( parent_n )
, _info_db ( info_db_n )
, _snd_hctl ( hctl_n )
, _values_changed ( false )
{
  snd_ctl_elem_info_malloc ( &_snd_ctl_elem_info );
  snd_ctl_elem_value_malloc ( &_snd_ctl_elem_value );

  snd_ctl_elem_info_clear ( _snd_ctl_elem_info );
  snd_ctl_elem_value_clear ( _snd_ctl_elem_value );

  set_snd_hctl_elem ( hctl_elem_n );
}

Mixer_HCTL_Elem::~Mixer_HCTL_Elem ()
{
  snd_ctl_elem_info_free ( _snd_ctl_elem_info );
  snd_ctl_elem_value_free ( _snd_ctl_elem_value );
}

QString
Mixer_HCTL_Elem::enum_item_display_name ( unsigned int enum_idx_n ) const
{
  return QCoreApplication::translate ( "ALSA::Enum_Value",
                                       enum_item_name ( enum_idx_n ) );
}

void
Mixer_HCTL_Elem::clear ()
{
  _enum_item_names.clear ();
}

void
Mixer_HCTL_Elem::set_snd_hctl ( snd_hctl_t * hctl_n )
{
  clear ();

  _snd_hctl = hctl_n;
}

void
Mixer_HCTL_Elem::set_display_name ( const QString & name_n )
{
  _display_name = name_n;
}

void
Mixer_HCTL_Elem::set_snd_hctl_elem ( snd_hctl_elem_t * hctl_elem_n )
{
  clear ();

  if ( hctl_elem_n == 0 ) {
    return;
  }

  _snd_hctl_elem = hctl_elem_n;

  if ( snd_hctl_elem () == 0 ) {
    return;
  }

  // Load info
  snd_hctl_elem_info ( snd_hctl_elem (), _snd_ctl_elem_info );

  _display_name = QCoreApplication::translate (
      "ALSA::Elem_Name", snd_hctl_elem_get_name ( snd_hctl_elem () ) );

  update_value_mark ();
}

// Boolean type

bool
Mixer_HCTL_Elem::switches_equal () const
{
  bool res ( true );
  const unsigned int num ( count () );
  if ( num > 1 ) {
    long state0 ( switch_state ( 0 ) );
    for ( unsigned int ii = 1; ii < num; ++ii ) {
      if ( switch_state ( ii ) != state0 ) {
        res = false;
        break;
      }
    }
  }
  return res;
}

void
Mixer_HCTL_Elem::set_switch_state ( unsigned int idx_n, bool state_n )
{
  snd_ctl_elem_value_set_boolean ( _snd_ctl_elem_value, idx_n, state_n );
  value_was_set ();
}

void
Mixer_HCTL_Elem::set_switch_all ( bool state_n )
{
  unsigned int num ( count () );
  for ( unsigned int ii = 0; ii < num; ++ii ) {
    snd_ctl_elem_value_set_boolean ( _snd_ctl_elem_value, ii, state_n );
  }
  value_was_set ();
}

void
Mixer_HCTL_Elem::invert_switch_state ( unsigned int idx_n )
{
  bool state ( !switch_state ( idx_n ) );
  snd_ctl_elem_value_set_boolean ( _snd_ctl_elem_value, idx_n, state );
  value_was_set ();
}

void
Mixer_HCTL_Elem::invert_switch_all ()
{
  unsigned int num ( count () );
  for ( unsigned int ii = 0; ii < num; ++ii ) {
    bool state ( !switch_state ( ii ) );
    snd_ctl_elem_value_set_boolean ( _snd_ctl_elem_value, ii, state );
  }
  value_was_set ();
}

void
Mixer_HCTL_Elem::level_switches ()
{
  const unsigned int num_switches ( count () );

  unsigned int num_on ( 0 );
  unsigned int num_off ( 0 );

  for ( unsigned int ii = 0; ii < num_switches; ++ii ) {
    if ( switch_state ( ii ) ) {
      ++num_on;
    } else {
      ++num_off;
    }
  }

  set_switch_all ( ( num_on >= num_off ) );
}

// Integer type

bool
Mixer_HCTL_Elem::integers_equal () const
{
  bool res ( true );
  const unsigned int num ( count () );
  if ( num > 1 ) {
    long val0 ( integer_value ( 0 ) );
    for ( unsigned int ii = 1; ii < num; ++ii ) {
      if ( integer_value ( ii ) != val0 ) {
        res = false;
        break;
      }
    }
  }
  return res;
}

void
Mixer_HCTL_Elem::set_integer ( unsigned int idx_n, long value_n )
{
  //::std::cout << "Mixer_HCTL_Elem::set_integer " << idx_n << " value " <<
  //value_n << "\n";

  snd_ctl_elem_value_set_integer ( _snd_ctl_elem_value, idx_n, value_n );
  value_was_set ();
}

void
Mixer_HCTL_Elem::set_integer_all ( long value_n )
{
  unsigned int num ( count () );
  for ( unsigned int ii = 0; ii < num; ++ii ) {
    snd_ctl_elem_value_set_integer ( _snd_ctl_elem_value, ii, value_n );
  }
  value_was_set ();
}

void
Mixer_HCTL_Elem::level_integers ()
{
  const unsigned int num ( count () );

  if ( num < 2 ) {
    return;
  }

  const long num_int ( num );
  long vol_avrg_accu ( 0 );
  long vol_mod_accu ( 0 );

  for ( unsigned int ii = 0; ii < num; ++ii ) {
    const long val ( integer_value ( ii ) );

    long vol_mod;
    if ( ( val >= num_int ) || ( val <= ( -num_int ) ) ) {
      const long vol_avrg ( val / num_int );
      vol_avrg_accu += vol_avrg;
      vol_mod = val;
      // Subtraction performed in two steps to prevent and overflow
      vol_mod -= vol_avrg * ( num_int - 1 );
      vol_mod -= vol_avrg;
    } else {
      vol_mod = val;
    }

    vol_mod_accu += vol_mod;

    if ( ( vol_mod_accu >= num_int ) || ( vol_mod_accu <= ( -num_int ) ) ) {
      const long vol_avrg ( vol_mod_accu / num_int );
      vol_avrg_accu += vol_avrg;
      vol_mod_accu -= ( vol_avrg * num_int );
    }
  }

  set_integer_all ( vol_avrg_accu );
}

// Decibel

bool
Mixer_HCTL_Elem::has_dB () const
{
  long tmp[ 2 ];
  return ( dB_range ( &tmp[ 0 ], &tmp[ 1 ] ) == 0 );
}

int
Mixer_HCTL_Elem::dB_range ( long * min_n, long * max_n ) const
{
  snd_ctl_elem_id_t * elem_id;
  snd_ctl_elem_id_alloca ( &elem_id );
  snd_ctl_elem_info_get_id ( _snd_ctl_elem_info, elem_id );
  return snd_ctl_get_dB_range (
      snd_hctl_ctl ( _snd_hctl ), elem_id, min_n, max_n );
}

long
Mixer_HCTL_Elem::dB_value ( unsigned int idx_n ) const
{
  return ask_dB_from_int ( integer_value ( idx_n ) );
}

long
Mixer_HCTL_Elem::ask_dB_from_int ( long int_value_n ) const
{
  long res ( 0 );

  snd_ctl_elem_id_t * elem_id;
  snd_ctl_elem_id_alloca ( &elem_id );
  snd_ctl_elem_info_get_id ( _snd_ctl_elem_info, elem_id );
  snd_ctl_convert_to_dB (
      snd_hctl_ctl ( _snd_hctl ), elem_id, int_value_n, &res );

  return res;
}

long
Mixer_HCTL_Elem::ask_int_from_dB ( long db_value_n, int round_dir_n ) const
{
  long res ( 0 );

  snd_ctl_elem_id_t * elem_id;
  snd_ctl_elem_id_alloca ( &elem_id );
  snd_ctl_elem_info_get_id ( _snd_ctl_elem_info, elem_id );
  snd_ctl_convert_from_dB (
      snd_hctl_ctl ( _snd_hctl ), elem_id, db_value_n, &res, round_dir_n );

  return res;
}

// Enum type

bool
Mixer_HCTL_Elem::enum_idices_equal () const
{
  bool res ( true );
  const unsigned int num ( count () );
  if ( num > 1 ) {
    unsigned int idx0 ( enum_index ( 0 ) );
    for ( unsigned int ii = 1; ii < num; ++ii ) {
      if ( enum_index ( ii ) != idx0 ) {
        res = false;
        break;
      }
    }
  }
  return res;
}

void
Mixer_HCTL_Elem::set_enum_index ( unsigned int channel_idx_n,
                                  unsigned int enum_idx_n )
{
  snd_ctl_elem_value_set_enumerated (
      _snd_ctl_elem_value, channel_idx_n, enum_idx_n );
  value_was_set ();
}

void
Mixer_HCTL_Elem::set_enum_index_all ( unsigned int enum_idx_n )
{
  unsigned int num ( count () );
  for ( unsigned int ii = 0; ii < num; ++ii ) {
    snd_ctl_elem_value_set_enumerated ( _snd_ctl_elem_value, ii, enum_idx_n );
  }
  value_was_set ();
}

void
Mixer_HCTL_Elem::level_enums ()
{
  unsigned int idx ( 0 );
  if ( count () > 0 ) {
    idx = enum_index ( 0 );
  }
  set_enum_index_all ( idx );
}

// General

bool
Mixer_HCTL_Elem::values_equal () const
{
  bool res ( true );
  if ( is_boolean () ) {
    res = switches_equal ();
  } else if ( is_integer () ) {
    res = integers_equal ();
  } else if ( is_enumerated () ) {
    res = enum_idices_equal ();
  }
  return res;
}

void
Mixer_HCTL_Elem::level_values ()
{
  if ( is_boolean () ) {
    level_switches ();
  } else if ( is_integer () ) {
    level_integers ();
  } else if ( is_enumerated () ) {
    level_enums ();
  }
}

void
Mixer_HCTL_Elem::value_was_set ()
{
  snd_hctl_elem_t * hctl_elem ( snd_hctl_elem () );
  if ( hctl_elem != 0 ) {
    int err = snd_hctl_elem_write ( hctl_elem, _snd_ctl_elem_value );
    if ( err >= 0 ) {

      // Read values back
      update_value_mark ();

      if ( parent () != 0 ) {
        QEvent ev_req ( ::QSnd::evt_update_values_request );
        QCoreApplication::sendEvent ( parent (), &ev_req );
      }

    } else {
      print_alsa_error ( "snd_hctl_elem_write", err );
    }
  }
}

void
Mixer_HCTL_Elem::update_value ()
{
  snd_hctl_elem_t * hctl_elem ( snd_hctl_elem () );
  if ( hctl_elem != 0 ) {
    int err = snd_hctl_elem_read ( hctl_elem, _snd_ctl_elem_value );
    if ( err < 0 ) {
      print_alsa_error ( "snd_hctl_elem_read", err );
    }
  }
}

void
Mixer_HCTL_Elem::update_value_mark ()
{
  _values_changed = true;
  update_value ();
}

void
Mixer_HCTL_Elem::signalize_changes ()
{
  if ( _values_changed ) {
    _values_changed = false;
    emit sig_values_changed ();
  }
}

void
Mixer_HCTL_Elem::signalize_element_changed ()
{
  if ( snd_hctl_elem () != 0 ) {
    snd_hctl_elem_info ( snd_hctl_elem (), _snd_ctl_elem_info );
  }
  if ( parent () != 0 ) {
    QEvent ev_req ( ::QSnd::evt_reload_request );
    QCoreApplication::sendEvent ( parent (), &ev_req );
  }
}

// Alsa callbacks

int
Mixer_HCTL_Elem::alsa_callback_hctl_elem ( snd_hctl_elem_t * elem_n,
                                           unsigned int mask_n )
{
  int res ( 0 );

  Mixer_HCTL_Elem * smce;
  {
    void * priv ( snd_hctl_elem_get_callback_private ( elem_n ) );
    smce = reinterpret_cast< Mixer_HCTL_Elem * > ( priv );
  }

  if ( smce != 0 ) {
    const unsigned int change_mask ( SND_CTL_EVENT_MASK_INFO |
                                     SND_CTL_EVENT_MASK_ADD |
                                     SND_CTL_EVENT_MASK_TLV );

    if ( ( mask_n == SND_CTL_EVENT_MASK_REMOVE ) ||
         ( ( mask_n & change_mask ) != 0 ) ) {
      smce->signalize_element_changed ();
    } else if ( ( mask_n & SND_CTL_EVENT_MASK_VALUE ) != 0 ) {
      smce->update_value_mark ();
    } else {
      // Unusual mask
      {
        ::std::stringstream msg;
        msg << "Mixer_HCTL_Elem::alsa_callback_hctl_elem: ";
        msg << "Unknown mask ( " << mask_n << " )" << ::std::endl;
        ::std::cerr << msg.str ();
      }
      res = -1;
    }
  }

  return res;
}

} // namespace QSnd
