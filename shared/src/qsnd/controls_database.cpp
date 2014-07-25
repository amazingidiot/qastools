//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "controls_database.hpp"

#include "qsnd/alsa.hpp"
#include "qsnd/card_info.hpp"
#include "qsnd/ctl_format.hpp"

#include <QStringList>
#include <iostream>


namespace QSnd
{

// Local functions

bool
setup_ctl_format (
	::QSnd::CTL_Format & ctl_format_n,
	snd_config_t * scfg_n );

void
setup_ctl_format_args (
	::QSnd::CTL_Format & ctl_format_n,
	snd_config_t * scfg_n );

void
setup_ctl_format_arg (
	::QSnd::CTL_Format_Argument & ctl_arg_n,
	snd_config_t * scfg_n );


// Controls_Database

Controls_Database::Controls_Database ( )
{
	reload_silent();
}


Controls_Database::~Controls_Database ( )
{
	clear_data();
}


const ::QSnd::CTL_Format *
Controls_Database::find_control_def (
	const QString & ctl_name_n ) const
{
	const ::QSnd::CTL_Format * res ( 0 );
	for ( int ii=0; ii != _ctl_formats.size(); ++ii ) {
		const ::QSnd::CTL_Format & ctl_format ( _ctl_formats[ii] );
		if ( ctl_format.ctl_name() == ctl_name_n ) {
			res = &ctl_format;
			break;
		}
	}
	return res;
}


void
Controls_Database::reload_silent ( )
{
	clear_data();
	load_plugins();
	load_cards();
}


void
Controls_Database::reload ( )
{
	emit sig_change_comming();
	reload_silent();
	emit sig_change_done();
}


void
Controls_Database::clear_data ( )
{
	// Controls
	_ctl_formats.clear();
	// Cards
	_card_infos.clear();
}


void
Controls_Database::load_plugins ( )
{
	// Load ALSA configuration tree and read ctl entries
	{
		snd_config_t * snd_cfg = 0;
		// Read ALSA config from disk
		{
			snd_config_update_t * snd_cfg_update = 0;
			snd_config_update_r ( &snd_cfg, &snd_cfg_update, 0 );
			snd_config_update_free ( snd_cfg_update );
		}
		if ( snd_cfg != 0 ) {
			snd_config_t * snd_cfg_ctl = 0;
			snd_config_search ( snd_cfg, "ctl", &snd_cfg_ctl );

			if ( snd_cfg_ctl != 0 ) {
				snd_config_iterator_t iter =
					snd_config_iterator_first ( snd_cfg_ctl );
				snd_config_iterator_t iter_end =
					snd_config_iterator_end ( snd_cfg_ctl );
				while ( iter != iter_end ) {
					snd_config_t * scfg = snd_config_iterator_entry ( iter );
					{
						::QSnd::CTL_Format ctl_format;
						if ( setup_ctl_format ( ctl_format, scfg ) ) {
							_ctl_formats.append ( ctl_format );
						}
					}
					iter = snd_config_iterator_next ( iter );
				}
			}

			snd_config_delete ( snd_cfg );
		}
	}
}


void
Controls_Database::load_cards ( )
{
	// Load card infos
	{
		int card_idx = -1;
		while (	( snd_card_next ( &card_idx ) == 0 ) &&
			( card_idx >= 0 ) )
		{
			_card_infos.append ( ::QSnd::Card_Info ( card_idx ) );
		}
	}
}


bool
setup_ctl_format (
	::QSnd::CTL_Format & ctl_format_n,
	snd_config_t * scfg_n )
{
	if ( scfg_n != 0 ) {
		const char * char_ptr ( 0 );
		int err = snd_config_get_id ( scfg_n, &char_ptr );
		if ( ( err == 0 ) && ( char_ptr != 0 ) ) {
			QString ctl_id ( char_ptr );
			ctl_format_n.set_ctl_name ( ctl_id );
			setup_ctl_format_args ( ctl_format_n, scfg_n );
		}
		return true;
	}
	return false;
}


inline
void
setup_ctl_format_args (
	::QSnd::CTL_Format & ctl_format_n,
	snd_config_t * scfg_n )
{
	snd_config_t * scfg_args ( 0 );
	snd_config_search ( scfg_n, "@args", &scfg_args );
	if ( scfg_args != 0 ) {
		QStringList arg_names;
		{
			QString str_num;
			for ( unsigned int ii=0; ii < 9; ++ii ) {
				str_num.setNum ( ii );
				snd_config_t * scfg_arg ( 0 );
				snd_config_search (
					scfg_args, str_num.toLatin1().constData(), &scfg_arg );
				if ( scfg_arg != 0 ) {
					if ( snd_config_get_type ( scfg_arg ) == SND_CONFIG_TYPE_STRING ) {
						const char * char_ptr ( 0 );
						int err = snd_config_get_string ( scfg_arg, &char_ptr );
						if ( ( err == 0 ) && ( char_ptr != 0 ) ) {
							arg_names.append ( char_ptr );
						}
					}
				}
			}
		}
		for ( int ii=0; ii < arg_names.size(); ++ii ) {
			::QSnd::CTL_Format_Argument ctl_arg;
			ctl_arg.arg_name = arg_names[ii];
			setup_ctl_format_arg ( ctl_arg, scfg_args );
			ctl_format_n.append_arg ( ctl_arg );
		}
	}
}


inline
void
setup_ctl_format_arg (
	::QSnd::CTL_Format_Argument & ctl_arg_n,
	snd_config_t * scfg_n )
{
	snd_config_t * scfg_arg_com ( 0 );
	{
		int err = snd_config_search (
			scfg_n, ctl_arg_n.arg_name.toUtf8().constData(), &scfg_arg_com );
		if ( ( err != 0 ) || ( scfg_arg_com == 0 ) ) {
			return;
		}
		if ( snd_config_get_type ( scfg_arg_com ) != SND_CONFIG_TYPE_COMPOUND ) {
			return;
		}
	}

	{
		snd_config_t * scfg_arg_type ( 0 );
		int err = snd_config_search ( scfg_arg_com, "type", &scfg_arg_type );
		if ( ( err == 0 ) && ( scfg_arg_type != 0 ) ) {
			if ( snd_config_get_type ( scfg_arg_type ) == SND_CONFIG_TYPE_STRING ) {
				const char * char_ptr ( 0 );
				err = snd_config_get_string ( scfg_arg_type, &char_ptr );
				if ( ( err == 0 ) && ( char_ptr != 0 ) ) {
					//::std::cout << "Found ctl arg type: " << char_ptr << "\n";
					ctl_arg_n.arg_type = char_ptr;
				}
			}
		}
	}
}


} // End of namespace
