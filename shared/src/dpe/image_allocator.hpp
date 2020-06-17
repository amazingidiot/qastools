/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_dpe_image_allocator_hpp__
#define __INC_dpe_image_allocator_hpp__

#include "is_buffer.hpp"
#include <QTimer>

// Forward declaration
namespace dpe
{
class Image_Set_Group;
class Image_Request;
class Painter;
class Painter_Thread;
class Painter_Thread_Shared;
} // namespace dpe

namespace dpe
{

/// @brief Image_Set_Group allocator
///
class Image_Allocator : public QObject
{
  Q_OBJECT

  // Public methods
  public:
  Image_Allocator ();

  ~Image_Allocator ();

  void
  install_painter ( ::dpe::Painter * painter_n );

  // Multithreading

  bool
  multithreading_is_safe () const;

  bool
  multithread () const;

  void
  set_multithread ( bool flag_n );

  void
  send_request ( ::dpe::Image_Request * request_n );

  void
  return_group ( ::dpe::Image_Set_Group * group_n );

  // Private slots
  private slots:

  void
  stop_timeout ();

  // Private methods
  private:
  void
  start_threads ();

  void
  stop_threads ();

  void
  process_handle ( ::dpe::IS_Buffer_Handle * handle_n );

  void
  render_handle ( ::dpe::IS_Buffer_Handle * handle_n );

  void
  enqueue_handle ( ::dpe::IS_Buffer_Handle * handle_n );

  // Private attributes
  private:
  ::dpe::IS_Buffer _buffer;
  ::dpe::Painter_Thread_Shared * _shared;
  QList<::dpe::Painter_Thread * > _threads;
  bool _multithread;

  QTimer _stop_timer;
};

inline bool
Image_Allocator::multithread () const
{
  return _multithread;
}

} // namespace dpe

#endif
