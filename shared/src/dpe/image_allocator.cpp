/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "image_allocator.hpp"
#include "image_request.hpp"
#include "image_set.hpp"
#include "image_set_group.hpp"
#include "image_set_meta.hpp"
#include "image_set_state.hpp"
#include "paint_job.hpp"
#include "painter.hpp"
#include "painter_thread.hpp"
#include "painter_thread_shared.hpp"
#include <cassert>
#include <iostream>

namespace dpe
{

Image_Allocator::Image_Allocator ()
{
  _shared = new ::dpe::Painter_Thread_Shared;
  _multithread = multithreading_is_safe ();

  _stop_timer.setInterval ( 1500 );
  _stop_timer.setSingleShot ( true );
  connect (
      &_stop_timer, SIGNAL ( timeout () ), this, SLOT ( stop_timeout () ) );
}

Image_Allocator::~Image_Allocator ()
{
  stop_threads ();
  delete _shared;
}

void
Image_Allocator::install_painter ( ::dpe::Painter * painter_n )
{
  _shared->painters.append ( painter_n );
}

bool
Image_Allocator::multithreading_is_safe () const
{
  bool res ( true );
  // https://bugreports.qt.nokia.com/browse/QTBUG-14614
  if ( QT_VERSION < QT_VERSION_CHECK ( 4, 7, 2 ) ) {
    res = false;
  }
  return res;
}

void
Image_Allocator::set_multithread ( bool flag_n )
{
  flag_n = ( flag_n && multithreading_is_safe () );
  if ( flag_n != _multithread ) {
    if ( _multithread ) {
      _shared->wait_for_finish ();
    }
    _multithread = flag_n;
  }
}

void
Image_Allocator::start_threads ()
{
  int num_threads ( QThread::idealThreadCount () );
  if ( num_threads <= 0 ) {
    num_threads = 1;
  }

  for ( int ii = 0; ii < num_threads; ++ii ) {
    _threads.append ( new Painter_Thread ( *_shared ) );
  }
  for ( int ii = 0; ii < num_threads; ++ii ) {
    _threads[ ii ]->start ();
  }
}

void
Image_Allocator::stop_threads ()
{
  if ( _threads.size () > 0 ) {
    // Send abort signal
    _shared->abort_threads ( _threads.size () );
    for ( int ii = 0; ii < _threads.size (); ++ii ) {
      _threads[ ii ]->wait ();
      delete _threads[ ii ];
    }
    _threads.clear ();
  }
}

void
Image_Allocator::stop_timeout ()
{
  _shared->num_rendering.ref ();
  if ( !_shared->num_rendering.deref () ) {
    stop_threads ();
  } else {
    _stop_timer.start ();
  }
}

void
Image_Allocator::send_request ( ::dpe::Image_Request * request_n )
{
  if ( request_n == 0 ) {
    return;
  }

  for ( unsigned int ii = 0; ii < request_n->group->img_sets.size (); ++ii ) {
    ::dpe::IS_Buffer_Handle * handle;

    handle = _buffer.acquire_return_handle ( request_n->meta[ ii ],
                                             request_n->group->img_sets[ ii ] );
    if ( handle == 0 ) {
      // No matching image set found!
      // Create new handle and image set
      // and enqueue for rendering
      ::dpe::IS_Buffer_Handle * hnew ( new ::dpe::IS_Buffer_Handle );
      hnew->meta = request_n->meta[ ii ]->new_copy ();
      hnew->img_set = new ::dpe::Image_Set ( hnew->meta->num_images );
      _buffer.append_handle ( hnew );
      process_handle ( hnew );

      handle = hnew;
    }
    request_n->group->img_sets[ ii ] = handle->img_set;
    request_n->states[ ii ] = handle->state;
  }
}

void
Image_Allocator::process_handle ( ::dpe::IS_Buffer_Handle * handle_n )
{
  if ( _multithread ) {
    if ( _threads.size () <= 0 ) {
      start_threads ();
    }
    _stop_timer.start ();
    enqueue_handle ( handle_n );
  } else {
    render_handle ( handle_n );
  }
}

void
Image_Allocator::render_handle ( ::dpe::IS_Buffer_Handle * handle_n )
{
  handle_n->state->init_todo ( 0 );

  // Create paint jobs
  unsigned int num ( handle_n->img_set->num_images () );
  for ( unsigned int ii = 0; ii < num; ++ii ) {
    ::dpe::Paint_Job pjob;
    pjob.meta = handle_n->meta;
    pjob.img_set = handle_n->img_set;
    pjob.img_idx = ii;
    pjob.state = handle_n->state;

    _shared->paint_job ( &pjob );
  }
}

void
Image_Allocator::enqueue_handle ( ::dpe::IS_Buffer_Handle * handle_n )
{
  // Increment todo counters
  handle_n->state->init_todo ( handle_n->img_set->num_images () );
  _shared->num_rendering.ref ();

  // Create paint jobs and enqueue
  unsigned int num ( handle_n->img_set->num_images () );
  for ( unsigned int ii = 0; ii < num; ++ii ) {
    ::dpe::Paint_Job * pjob ( new ::dpe::Paint_Job );
    pjob->meta = handle_n->meta;
    pjob->img_set = handle_n->img_set;
    pjob->img_idx = ii;
    pjob->state = handle_n->state;
    _shared->enqueue_job ( pjob );
  }
}

void
Image_Allocator::return_group ( ::dpe::Image_Set_Group * group_n )
{
  for ( unsigned int ii = 0; ii < group_n->img_sets.size (); ++ii ) {
    _buffer.return_img_set ( group_n->img_sets[ ii ] );
    group_n->img_sets[ ii ] = 0;
  }
}

} // namespace dpe
