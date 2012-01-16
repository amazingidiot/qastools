//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "controls_db.hpp"

#include "qsnd/alsa.hpp"
#include "qsnd/card_info.hpp"
#include "qsnd/ctl_def.hpp"

#include <QStringList>
#include <iostream>


namespace QSnd
{

// Local functions

::QSnd::CTL_Def *
create_ctl_def (
	snd_config_t * scfg_n );

void
create_ctl_def_args (
	snd_config_t * scfg_n,
	::QSnd::CTL_Def * ctl_def_n );

void
setup_ctl_def_args (
	snd_config_t * scfg_n,
	::QSnd::CTL_Def_Arg * ctl_def_n );


// Controls_DB

Controls_DB::Controls_DB ( )
{
	reload_silent();
}


Controls_DB::~Controls_DB ( )
{
	clear_data();
}


const ::QSnd::CTL_Def *
Controls_DB::find_control_def (
	const QString & ctl_name_n ) const
{
	const ::QSnd::CTL_Def * res ( 0 );
	for ( int ii=0; ii < _ctl_defs.size(); ++ii ) {
		const ::QSnd::CTL_Def * ctl_def ( _ctl_defs[ii] );
		if ( ctl_def->ctl_name() == ctl_name_n ) {
			res = ctl_def;
			break;
		}
	}
	return res;
}


void
Controls_DB::reload_silent ( )
{
	clear_data();
	load_plugins();
	load_cards();
}


void
Controls_DB::reload ( )
{
	emit sig_change_comming();
	reload_silent();
	emit sig_change_done();
}


void
Controls_DB::clear_data ( )
{
	// Controls
	if ( _ctl_defs.size() > 0 ) {
		for ( int ii=0; ii < _ctl_defs.size(); ++ii ) {
			delete _ctl_defs[ii];
		}
		_ctl_defs.clear();
	}

	// Cards
	if ( _card_infos.size() > 0 ) {
		for ( int ii=0; ii < _card_infos.size(); ++ii ) {
			delete _card_infos[ii];
		}
		_card_infos.clear();
	}
}


void
Controls_DB::load_plugins ( )
{
	QStringList ctl_names;

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
					::QSnd::CTL_Def * ctl_def ( create_ctl_def ( scfg ) );
					if ( ctl_def != 0 ) {
						_ctl_defs.append ( ctl_def );
					}
					iter = snd_config_iterator_next ( iter );
				}
			}

			snd_config_delete ( snd_cfg );
		}
	}
}


void
Controls_DB::load_cards ( )
{
	// Load card infos
	{
		int card_idx = -1;
		while (	( snd_card_next ( &card_idx ) == 0 ) &&
			( card_idx >= 0 ) )
		{
			::QSnd::Card_Info * cinfo (
				new ::QSnd::Card_Info ( card_idx ) );
			_card_infos.append ( cinfo );
		}
	}
}


::QSnd::CTL_Def *
create_ctl_def (
	snd_config_t * scfg_n )
{
	::QSnd::CTL_Def * ctl_def ( 0 );

	if ( scfg_n != 0 ) {
		const char * char_ptr ( 0 );
		int err = snd_config_get_id ( scfg_n, &char_ptr );
		if ( ( err == 0 ) && ( char_ptr != 0 ) ) {
			QString ctl_id ( char_ptr );
			ctl_def = new ::QSnd::CTL_Def ( ctl_id );
			create_ctl_def_args ( scfg_n, ctl_def );
		}
	}

	return ctl_def;
}


inline
void
create_ctl_def_args (
	snd_config_t * scfg_n,
	::QSnd::CTL_Def * ctl_def_n )
{
	snd_config_t * scfg_args ( 0 );
	snd_config_search ( scfg_n, "@args", &scfg_args );
	if ( scfg_args == 0 ) {
		return;
	}

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
		::QSnd::CTL_Def_Arg * ctl_arg ( new ::QSnd::CTL_Def_Arg );
		ctl_arg->arg_name = arg_names[ii];
		setup_ctl_def_args ( scfg_args, ctl_arg );
		ctl_def_n->append_arg ( ctl_arg );
	}

}


inline
void
setup_ctl_def_args (
	snd_config_t * scfg_n,
	::QSnd::CTL_Def_Arg * ctl_arg_n )
{
	snd_config_t * scfg_arg_com ( 0 );
	{
		int err = snd_config_search (
			scfg_n, ctl_arg_n->arg_name.toLatin1().constData(), &scfg_arg_com );
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
					ctl_arg_n->arg_type = char_ptr;
				}
			}
		}
	}
}


} // End of namespace
