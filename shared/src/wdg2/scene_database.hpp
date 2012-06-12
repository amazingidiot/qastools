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
#include <QGraphicsItem>

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


	// Private attributes
	private:

	::dpe2::Pixmap_Server * _pxm_server;
};


inline
::dpe2::Pixmap_Server *
Scene_Database::pxm_server ( ) const
{
	return _pxm_server;
}


} // End of namespace

#endif
