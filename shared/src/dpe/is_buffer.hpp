/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_dpe_image_set_buffer_hpp__
#define __INC_dpe_image_set_buffer_hpp__

#include "is_buffer_handle.hpp"
#include <QObject>
#include <QTimer>

namespace dpe
{

/// @brief IS_Buffer
///
class IS_Buffer : public QObject
{
  Q_OBJECT

  // Public methods
  public:
  IS_Buffer ();

  ~IS_Buffer ();

  void
  clear ();

  // Buffer info

  unsigned int
  num_sets () const;

  unsigned int
  byte_count () const;

  // Image set accquiring an returning

  ::dpe::IS_Buffer_Handle *
  acquire_return_handle ( const ::dpe::Image_Set_Meta * meta_n,
                          ::dpe::Image_Set * cur_set_n = 0 );

  void
  append_handle ( ::dpe::IS_Buffer_Handle * handle_n );

  void
  return_img_set ( ::dpe::Image_Set * img_set_n );

  // Private slots
  private slots:

  void
  remove_poll ();

  // Private methods
  private:
  void
  destroy_handle ( ::dpe::IS_Buffer_Handle * handle_n );

  // Private attributes
  private:
  QList<::dpe::IS_Buffer_Handle * > _handles;
  QTimer _remove_poll_timer;

  unsigned int _storage_limit;
};

inline unsigned int
IS_Buffer::num_sets () const
{
  return _handles.size ();
}

} // namespace dpe

#endif
