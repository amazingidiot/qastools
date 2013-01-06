//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_wdg2_scene_database_hpp__
#define __INC_wdg2_scene_database_hpp__

#include "dpe2/pixmap_server.hpp"
#include "timer_server.hpp"
#include "input_keys_database.hpp"
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

	::Wdg2::Input_Keys_Database *
	inputs_db ( ) const;


	void
	install_theme (
		::Wdg2::Theme * theme_n );

	void
	remove_theme (
		::Wdg2::Theme * theme_n );


	// Public attributes
	public:

	// Timer ids
	unsigned int tid_animation;

	// Input key ids
	unsigned int ikid_joinable_toggle_joined;


	// Private attributes
	private:

	::dpe2::Pixmap_Server * _pxm_server;
	::Wdg2::Timer_Server * _timer_server;
	::Wdg2::Input_Keys_Database * _inputs_db;
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

inline
::Wdg2::Input_Keys_Database *
Scene_Database::inputs_db ( ) const
{
	return _inputs_db;
}


} // End of namespace

#endif
