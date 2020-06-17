/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "image_set_state.hpp"

namespace dpe
{

Image_Set_State::Image_Set_State ()
: _num_todo ( 0 )
, _finished ( false )
{
}

void
Image_Set_State::wait_for_finish ()
{
  _mutex.lock ();
  while ( !_finished ) {
    _cond.wait ( &_mutex );
  }
  _mutex.unlock ();
}

void
Image_Set_State::init_todo ( unsigned int num_n )
{
  _num_todo = num_n;
  _finished = ( _num_todo == 0 );
}

bool
Image_Set_State::one_done ()
{
  bool res = _num_todo.deref ();
  if ( !res ) {
    _mutex.lock ();
    _finished = true;
    _mutex.unlock ();
    _cond.wakeAll ();
  }
  return res;
}

} // namespace dpe
