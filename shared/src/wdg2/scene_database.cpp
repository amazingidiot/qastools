//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "scene_database.hpp"
#include <QEvent>

namespace Wdg2
{


Scene_Database::Scene_Database ( )
{
	// TODO: ScopedPointer?
	_pxm_server = new ::dpe2::Pixmap_Server;
	_pxm_server->start();
}

Scene_Database::~Scene_Database ( )
{
	delete _pxm_server;
}


} // End of namespace
