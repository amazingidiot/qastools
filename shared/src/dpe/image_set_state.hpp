/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_dpe_is_state_hpp__
#define __INC_dpe_is_state_hpp__

#include <QAtomicInt>
#include <QList>
#include <QMutex>
#include <QWaitCondition>

namespace dpe
{

/// @brief Image_Set_State
///
class Image_Set_State
{
  // Public methods
  public:
  Image_Set_State ();

  void
  wait_for_finish ();

  void
  init_todo ( unsigned int num_n );

  /// @brief To be called by render threads ( thread safe )
  ///
  /// This method is called by render threads after finishing an
  /// image paint.
  ///
  /// @return True if there's more to do
  bool
  one_done ();

  // Private attributes
  private:
  QAtomicInt _num_todo;
  QMutex _mutex;
  QWaitCondition _cond;
  bool _finished;
};

} // namespace dpe

#endif
