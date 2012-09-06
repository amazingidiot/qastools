//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "scene_database.hpp"
#include "theme.hpp"
#include "theme_painters.hpp"
#include <cassert>

namespace Wdg2
{


Scene_Database::Scene_Database ( )
{
	// TODO: ScopedPointer?
	_pxm_server = new ::dpe2::Pixmap_Server;
	_pxm_server->start();
	_timer_server = new ::Wdg2::Timer_Server;
	_inputs_db = new ::Wdg2::Input_Keys_Database;

	tid_animation = timer_server()->register_timer ( 1000 / 60 );

	{
		unsigned int ikey_id ( inputs_db()->register_action() );
		{
			::Wdg2::Input_Action ikey ( *inputs_db()->action ( ikey_id ) );
			ikey.default_key = Qt::Key_J;
			ikey.reset_to_default();
			inputs_db()->set_action ( ikey_id, ikey );
		}
		ikid_joinable_toggle_joined = ikey_id;
	}
}

Scene_Database::~Scene_Database ( )
{
	_pxm_server->stop();
	// Remove all themes
	while ( _themes.size() != 0 ) {
		remove_theme ( _themes.back() );
	}

	inputs_db()->unregister_action ( ikid_joinable_toggle_joined );
	timer_server()->unregister_timer ( tid_animation );

	delete _pxm_server;
	delete _timer_server;
	delete _inputs_db;
}

void
Scene_Database::install_theme (
	::Wdg2::Theme * theme_n )
{
	assert ( theme_n != 0 );

	_themes.append ( theme_n );
	{
		const bool was_running ( _pxm_server->threads_running() != 0 );
		if ( was_running ) {
			_pxm_server->stop();
		}

		// Install painters
		for ( unsigned int ii=0; ii < theme_n->num_painters(); ++ii ) {
			_pxm_server->install_painter ( theme_n->painter ( ii ) );
		}

		if ( was_running ) {
			_pxm_server->start();
		}
	}
}

void
Scene_Database::remove_theme (
	::Wdg2::Theme * theme_n )
{
	assert ( theme_n != 0 );

	if ( _themes.removeOne ( theme_n ) ) {
		const bool was_running ( _pxm_server->threads_running() != 0 );
		if ( was_running ) {
			_pxm_server->stop();
		}

		// Remove painters
		for ( unsigned int ii=0; ii < theme_n->num_painters(); ++ii ) {
			_pxm_server->remove_painter ( theme_n->painter ( ii ) );
		}

		if ( was_running ) {
			_pxm_server->start();
		}
	}
}


} // End of namespace
