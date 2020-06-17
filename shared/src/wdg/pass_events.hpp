/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_pass_events_hpp__
#define __INC_pass_events_hpp__

#include <QEvent>
#include <QFocusEvent>
#include <QKeyEvent>

namespace Wdg
{

/// @brief Pass_Event
///
class Pass_Event : public QEvent
{
  // Public methods
  public:
  Pass_Event ( QEvent::Type type_n,
               unsigned int group_idx_n = 0,
               unsigned int column_idx_n = 0,
               unsigned int row_idx_n = 0 );

  unsigned int group_idx;
  unsigned int column_idx;
  unsigned int row_idx;
};

/// @brief Pass_Event_Focus
///
class Pass_Event_Focus : public ::Wdg::Pass_Event
{
  // Public methods
  public:
  Pass_Event_Focus ( const QFocusEvent & event_n,
                     unsigned int group_idx_n = 0,
                     unsigned int column_idx_n = 0,
                     unsigned int row_idx_n = 0 );

  QFocusEvent ev_focus;
};

/// @brief Pass_Event_Key
///
class Pass_Event_Key : public ::Wdg::Pass_Event
{
  // Public methods
  public:
  Pass_Event_Key ( const QKeyEvent & event_n,
                   unsigned int group_idx_n = 0,
                   unsigned int column_idx_n = 0,
                   unsigned int row_idx_n = 0 );

  QKeyEvent ev_key;
};

} // namespace Wdg

#endif
