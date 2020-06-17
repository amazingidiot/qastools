/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "controls_database.hpp"
#include "qsnd/alsa.hpp"
#include <QStringList>
#include <iostream>

namespace
{

bool
setup_ctl_format ( ::QSnd::CTL_Format & ctl_format_n, snd_config_t * scfg_n );

void
setup_ctl_format_args ( ::QSnd::CTL_Format & ctl_format_n,
                        snd_config_t * scfg_n );

void
setup_ctl_format_arg ( ::QSnd::CTL_Format_Argument & ctl_arg_n,
                       snd_config_t * scfg_n );

bool
setup_ctl_format ( ::QSnd::CTL_Format & ctl_format_n, snd_config_t * scfg_n )
{
  if ( scfg_n == nullptr ) {
    return false;
  }

  const char * char_ptr = nullptr;
  int err = snd_config_get_id ( scfg_n, &char_ptr );
  if ( ( err == 0 ) && ( char_ptr != nullptr ) ) {
    ctl_format_n.set_ctl_name ( char_ptr );
    setup_ctl_format_args ( ctl_format_n, scfg_n );
  }
  return true;
}

inline void
setup_ctl_format_args ( ::QSnd::CTL_Format & ctl_format_n,
                        snd_config_t * scfg_n )
{
  snd_config_t * scfg_args = nullptr;
  snd_config_search ( scfg_n, "@args", &scfg_args );
  if ( scfg_args == nullptr ) {
    return;
  }

  QStringList arg_names;
  for ( std::size_t ii = 0; ii != 9; ++ii ) {
    snd_config_t * scfg_arg = nullptr;
    snd_config_search ( scfg_args, std::to_string ( ii ).c_str (), &scfg_arg );
    if ( scfg_arg == nullptr ) {
      continue;
    }
    if ( snd_config_get_type ( scfg_arg ) == SND_CONFIG_TYPE_STRING ) {
      const char * char_ptr = nullptr;
      int err = snd_config_get_string ( scfg_arg, &char_ptr );
      if ( ( err == 0 ) && ( char_ptr != 0 ) ) {
        arg_names.append ( char_ptr );
      }
    }
  }
  for ( auto & arg_name : arg_names ) {
    ::QSnd::CTL_Format_Argument ctl_arg;
    ctl_arg.arg_name = arg_name;
    setup_ctl_format_arg ( ctl_arg, scfg_args );
    ctl_format_n.append_arg ( std::move ( ctl_arg ) );
  }
}

inline void
setup_ctl_format_arg ( ::QSnd::CTL_Format_Argument & ctl_arg_n,
                       snd_config_t * scfg_n )
{
  snd_config_t * scfg_arg_com = nullptr;
  {
    int err = snd_config_search (
        scfg_n, ctl_arg_n.arg_name.toUtf8 ().constData (), &scfg_arg_com );
    if ( ( err != 0 ) || ( scfg_arg_com == nullptr ) ) {
      return;
    }
    if ( snd_config_get_type ( scfg_arg_com ) != SND_CONFIG_TYPE_COMPOUND ) {
      return;
    }
  }

  {
    snd_config_t * scfg_arg_type = nullptr;
    int err = snd_config_search ( scfg_arg_com, "type", &scfg_arg_type );
    if ( ( err != 0 ) || ( scfg_arg_type == nullptr ) ) {
      return;
    }
    if ( snd_config_get_type ( scfg_arg_type ) == SND_CONFIG_TYPE_STRING ) {
      const char * char_ptr = nullptr;
      err = snd_config_get_string ( scfg_arg_type, &char_ptr );
      if ( ( err == 0 ) && ( char_ptr != nullptr ) ) {
        ctl_arg_n.arg_type = char_ptr;
      }
    }
  }
}
} // namespace

namespace QSnd
{

// Controls_Database

Controls_Database::Controls_Database ()
{
  reload_silent ();
}

Controls_Database::~Controls_Database ()
{
  clear_silent ();
}

Controls_Database::Const_Handle
Controls_Database::find_control_format ( const QString & ctl_name_n ) const
{
  for ( const auto & ctl_format : _controls ) {
    if ( ctl_format->ctl_name () == ctl_name_n ) {
      return ctl_format;
    }
  }
  return Const_Handle ();
}

void
Controls_Database::clear ()
{
  emit sig_reload_begin ();
  clear_silent ();
  emit sig_reload_end ();
}

void
Controls_Database::clear_silent ()
{
  _controls.clear ();
}

void
Controls_Database::reload ()
{
  emit sig_reload_begin ();
  reload_silent ();
  emit sig_reload_end ();
}

void
Controls_Database::reload_silent ()
{
  clear_silent ();
  load_plugins ();
}

void
Controls_Database::load_plugins ()
{
  // Load ALSA configuration tree and read ctl entries
  snd_config_t * snd_cfg = nullptr;

  // Read ALSA config from disk
  {
    snd_config_update_t * snd_cfg_update = nullptr;
    snd_config_update_r ( &snd_cfg, &snd_cfg_update, 0 );
    snd_config_update_free ( snd_cfg_update );
  }
  if ( snd_cfg == nullptr ) {
    return;
  }

  snd_config_t * snd_cfg_ctl = nullptr;
  snd_config_search ( snd_cfg, "ctl", &snd_cfg_ctl );

  if ( snd_cfg_ctl != nullptr ) {
    snd_config_iterator_t iter = snd_config_iterator_first ( snd_cfg_ctl );
    snd_config_iterator_t iter_end = snd_config_iterator_end ( snd_cfg_ctl );
    while ( iter != iter_end ) {
      snd_config_t * scfg = snd_config_iterator_entry ( iter );
      {
        auto ctl_format = std::make_shared<::QSnd::CTL_Format > ();
        if ( setup_ctl_format ( *ctl_format, scfg ) ) {
          _controls.push_back ( std::move ( ctl_format ) );
        }
      }
      iter = snd_config_iterator_next ( iter );
    }
  }

  snd_config_delete ( snd_cfg );
}

} // namespace QSnd
