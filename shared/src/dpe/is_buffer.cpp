/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "is_buffer.hpp"
#include "image_set.hpp"
#include "image_set_meta.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>

namespace dpe
{

IS_Buffer::IS_Buffer ()
: _storage_limit ( 4 * 1024 * 1024 )
{
  _remove_poll_timer.setInterval ( 1000 / 3 );
  connect ( &_remove_poll_timer,
            SIGNAL ( timeout () ),
            this,
            SLOT ( remove_poll () ) );
}

IS_Buffer::~IS_Buffer ()
{
  clear ();
}

unsigned int
IS_Buffer::byte_count () const
{
  unsigned int res ( 0 );
  for ( int ii = 0; ii < _handles.size (); ++ii ) {
    res += _handles[ ii ]->img_set->byte_count ();
  }
  return res;
}

void
IS_Buffer::clear ()
{
  //::std::cout << "IS_Buffer::clear byte count " << byte_count() << "\n";
  if ( _handles.size () > 0 ) {
    for ( int ii = 0; ii < _handles.size (); ++ii ) {
      destroy_handle ( _handles[ ii ] );
    }
    _handles.clear ();
  }
}

void
IS_Buffer::destroy_handle ( ::dpe::IS_Buffer_Handle * handle_n )
{
  assert ( handle_n->num_users == 0 );

  delete handle_n->img_set;
  delete handle_n->meta;
  delete handle_n;
}

::dpe::IS_Buffer_Handle *
IS_Buffer::acquire_return_handle ( const ::dpe::Image_Set_Meta * meta_n,
                                   ::dpe::Image_Set * cur_set_n )
{
  ::dpe::IS_Buffer_Handle * res ( 0 );

  // Try to find an image set with matching meta information
  for ( int ii = 0; ii < _handles.size (); ++ii ) {
    ::dpe::IS_Buffer_Handle * handle ( _handles[ ii ] );
    if ( meta_n->matches ( handle->meta ) ) {
      res = handle;
      break;
    }
  }

  bool no_match ( true );
  if ( res != 0 ) {
    // Increment user count of the set found
    // if the argument set is not the found set
    no_match = ( res->img_set != cur_set_n );
    if ( no_match ) {
      ++res->num_users;
    }
  }

  if ( ( cur_set_n != 0 ) && no_match ) {
    return_img_set ( cur_set_n );
  }

  return res;
}

void
IS_Buffer::append_handle ( ::dpe::IS_Buffer_Handle * handle_n )
{
  ++handle_n->num_users;
  _handles.append ( handle_n );
}

void
IS_Buffer::return_img_set ( ::dpe::Image_Set * img_set_n )
{
  if ( img_set_n == 0 ) {
    return;
  }

  int list_idx ( _handles.size () );

  for ( int ii = 0; ii < _handles.size (); ++ii ) {
    ::dpe::Image_Set * img_set ( _handles[ ii ]->img_set );
    if ( img_set == img_set_n ) {
      list_idx = ii;
      break;
    }
  }

  if ( list_idx < _handles.size () ) {
    // Image set found in list
    ::dpe::IS_Buffer_Handle * handle ( _handles[ list_idx ] );

    //::std::cout << "IS_Buffer::return_img_set users " << handle->num_users <<
    //"\n";
    if ( handle->num_users > 0 ) {
      --handle->num_users;
    }

    if ( handle->num_users == 0 ) {
      //::std::cout << "IS_Buffer::return_img_set bytes " << byte_count() <<
      //"\n";
      if ( byte_count () <= _storage_limit ) {
        // Delete later
        handle->remove_time.start ();
        handle->remove_time = handle->remove_time.addMSecs ( 1000 );
        if ( !_remove_poll_timer.isActive () ) {
          _remove_poll_timer.start ();
        }
      } else {
        destroy_handle ( handle );
        _handles.removeAt ( list_idx );
      }
    }
  }

  //::std::cout << "::dpe::IS_Buffer::return_img_set: ";
  //::std::cout << "List size " << _handles.size() << "\n";
}

void
IS_Buffer::remove_poll ()
{
  unsigned int pending ( 0 );

  QTime time_now;
  time_now.start ();

  int idx ( 0 );
  while ( idx < _handles.size () ) {
    ::dpe::IS_Buffer_Handle * handle ( _handles[ idx ] );
    bool do_remove ( false );

    if ( handle->num_users == 0 ) {
      QTime & remove_time ( handle->remove_time );
      if ( remove_time.isValid () ) {
        if ( remove_time <= time_now ) {
          do_remove = true;
        } else {
          ++pending;
        }
      }
    }

    if ( do_remove ) {
      destroy_handle ( handle );
      _handles.removeAt ( idx );
    } else {
      ++idx;
    }
  }

  if ( pending == 0 ) {
    _remove_poll_timer.stop ();
  }

  //::std::cout << "::dpe::IS_Buffer::remove_poll: ";
  //::std::cout << "List size " << _handles.size() << "\n";
}

} // namespace dpe
