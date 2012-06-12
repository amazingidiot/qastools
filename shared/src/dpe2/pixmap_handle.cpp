//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "pixmap_handle.hpp"
#include "pixmap.hpp"
#include <iostream>
#include <cassert>


namespace dpe2
{


Pixmap_Handle::Pixmap_Handle ( ) :
pixmap ( 0 ),
num_users ( 0 )
{
}

Pixmap_Handle::~Pixmap_Handle ( )
{
}


} // End of namespace
