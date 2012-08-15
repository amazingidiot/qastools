//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_scene_database_hpp__
#define __INC_wdg2_scene_database_hpp__

#include "dpe2/pixmap_server.hpp"
#include "timer_server.hpp"
#include <QList>

// Forward declaration
namespace Wdg2
{
	class Theme;
}

namespace Wdg2
{


/// @brief Scene_Database
///
class Scene_Database
{
	// Public methods
	public:

	Scene_Database ( );

	~Scene_Database ( );


	::dpe2::Pixmap_Server *
	pxm_server ( ) const;

	::Wdg2::Timer_Server *
	timer_server ( ) const;


	void
	install_theme (
		::Wdg2::Theme * theme_n );

	void
	remove_theme (
		::Wdg2::Theme * theme_n );


	// Public attributes
	public:

	unsigned int tid_animation;


	// Private attributes
	private:

	::dpe2::Pixmap_Server * _pxm_server;
	::Wdg2::Timer_Server * _timer_server;
	QList < ::Wdg2::Theme * > _themes;
};


inline
::dpe2::Pixmap_Server *
Scene_Database::pxm_server ( ) const
{
	return _pxm_server;
}

inline
::Wdg2::Timer_Server *
Scene_Database::timer_server ( ) const
{
	return _timer_server;
}


} // End of namespace

#endif
