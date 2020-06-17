/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "is_buffer_handle.hpp"
#include "image_set_state.hpp"

namespace dpe
{

IS_Buffer_Handle::IS_Buffer_Handle ()
: num_users ( 0 )
, state ( new ::dpe::Image_Set_State )
{
}

IS_Buffer_Handle::~IS_Buffer_Handle ()
{
  delete state;
}

} // namespace dpe
