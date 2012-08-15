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

	tid_animation = _timer_server->create_timer ( 1000 / 60 );
}

Scene_Database::~Scene_Database ( )
{
	_pxm_server->stop();
	// Remove all themes
	while ( _themes.size() != 0 ) {
		remove_theme ( _themes.back() );
	}
	delete _pxm_server;
	delete _timer_server;
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
