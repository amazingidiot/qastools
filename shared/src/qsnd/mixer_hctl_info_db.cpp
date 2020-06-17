/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl_info_db.hpp"
#include <QCoreApplication>
#include <QEvent>
#include <iostream>

namespace QSnd
{

Mixer_HCTL_Info_Db::Mixer_HCTL_Info_Db ()
{

  // Element type names
  for ( unsigned int ii = 0; ii < num_iface_types (); ++ii ) {
    const char * if_name ( snd_ctl_elem_type_name ( element_idx_type ( ii ) ) );
    _etype_names[ ii ] = if_name;
    _etype_display_names[ ii ] =
        QCoreApplication::translate ( "ALSA::CTL_Elem_Type_Name", if_name );

    // Fix untranslated names
    if ( _etype_names[ ii ] == _etype_display_names[ ii ] ) {
      QString & idn ( _etype_display_names[ ii ] );
      if ( idn == "NONE" ) {
        idn = "None";
      } else if ( idn == "BOOLEAN" ) {
        idn = "Boolean";
      } else if ( idn == "INTEGER" ) {
        idn = "Integer";
      } else if ( idn == "ENUMERATED" ) {
        idn = "Enumerated";
      } else if ( idn == "BYTES" ) {
        idn = "Bytes";
      } else if ( idn == "IEC958" ) {
        idn = "IEC958";
      } else if ( idn == "INTEGER64" ) {
        idn = "Integer64";
      }
    }
  }

  {
    const unsigned int idx ( num_element_types () );
    _etype_names[ idx ] =
        QCoreApplication::translate ( "ALSA::CTL_Elem_Type_Name", "Unknown" );
    _etype_display_names[ idx ] = _etype_names[ idx ];
  }

  // Interface names
  for ( unsigned int ii = 0; ii < num_iface_types (); ++ii ) {
    const char * if_name = snd_ctl_elem_iface_name ( iface_idx_type ( ii ) );
    _iface_names[ ii ] = if_name;
    _iface_display_names[ ii ] =
        QCoreApplication::translate ( "ALSA::CTL_Elem_IFace_Name", if_name );

    // Fix untranslated names
    if ( _iface_names[ ii ] == _iface_display_names[ ii ] ) {
      QString & idn ( _iface_display_names[ ii ] );
      if ( idn == "CARD" ) {
        idn = "Card";
      } else if ( idn == "HWDEP" ) {
        idn = "Hw dep.";
      } else if ( idn == "MIXER" ) {
        idn = "Mixer";
      } else if ( idn == "PCM" ) {
        idn = "PCM";
      } else if ( idn == "RAWMIDI" ) {
        idn = "Raw midi";
      } else if ( idn == "TIMER" ) {
        idn = "Timer";
      } else if ( idn == "SEQUENCER" ) {
        idn = "Sequencer";
      }
    }
  }

  {
    const unsigned int idx ( num_iface_types () );
    _iface_names[ idx ] =
        QCoreApplication::translate ( "ALSA::CTL_Elem_IFace_Name", "Unknown" );
    _iface_display_names[ idx ] = _iface_names[ idx ];
  }

  // Flag names
  _flag_readable_text[ 0 ] =
      QCoreApplication::translate ( "ALSA::Flags", "not readable" );
  _flag_readable_text[ 1 ] =
      QCoreApplication::translate ( "ALSA::Flags", "readable" );

  _flag_readable_char[ 0 ] = "-";
  _flag_readable_char[ 1 ] = "r";

  _flag_writable_text[ 0 ] =
      QCoreApplication::translate ( "ALSA::Flags", "not writable" );
  _flag_writable_text[ 1 ] =
      QCoreApplication::translate ( "ALSA::Flags", "writable" );

  _flag_writable_char[ 0 ] = "-";
  _flag_writable_char[ 1 ] = "w";

  _flag_volatile_text[ 0 ] =
      QCoreApplication::translate ( "ALSA::Flags", "not volatile" );
  _flag_volatile_text[ 1 ] =
      QCoreApplication::translate ( "ALSA::Flags", "volatile" );

  _flag_volatile_char[ 0 ] = "-";
  _flag_volatile_char[ 1 ] = "v";

  _flag_active_text[ 0 ] =
      QCoreApplication::translate ( "ALSA::Flags", "not active" );
  _flag_active_text[ 1 ] =
      QCoreApplication::translate ( "ALSA::Flags", "active" );

  _flag_active_char[ 0 ] = "-";
  _flag_active_char[ 1 ] = "a";
}

const QString &
Mixer_HCTL_Info_Db::elem_type_name ( snd_ctl_elem_type_t type_n ) const
{
  return elem_type_name ( element_type_idx ( type_n ) );
}

const QString &
Mixer_HCTL_Info_Db::elem_type_display_name ( snd_ctl_elem_type_t type_n ) const
{
  return elem_type_display_name ( element_type_idx ( type_n ) );
}

const QString &
Mixer_HCTL_Info_Db::iface_name ( snd_ctl_elem_iface_t type_n ) const
{
  return iface_name ( iface_type_idx ( type_n ) );
}

const QString &
Mixer_HCTL_Info_Db::iface_display_name ( snd_ctl_elem_iface_t type_n ) const
{
  return iface_display_name ( iface_type_idx ( type_n ) );
}

} // namespace QSnd
