//
// C++ Implementation:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "is_buffer_handle.hpp"

#include "image_set_state.hpp"


namespace dpe
{


IS_Buffer_Handle::IS_Buffer_Handle ( ) :
num_users ( 0 ),
state ( new ::dpe::Image_Set_State )
{
}


IS_Buffer_Handle::~IS_Buffer_Handle ( )
{
	delete state;
}


} // End of namespace
