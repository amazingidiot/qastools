/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "pass_events.hpp"
#include "wdg/event_types.hpp"

namespace Wdg
{

Pass_Event::Pass_Event ( QEvent::Type type_n,
                         unsigned int group_idx_n,
                         unsigned int column_idx_n,
                         unsigned int row_idx_n )
: QEvent ( type_n )
, group_idx ( group_idx_n )
, column_idx ( column_idx_n )
, row_idx ( row_idx_n )
{
}

Pass_Event_Focus::Pass_Event_Focus ( const QFocusEvent & event_n,
                                     unsigned int group_idx_n,
                                     unsigned int column_idx_n,
                                     unsigned int row_idx_n )
: ::Wdg::Pass_Event (
      ::Wdg::evt_pass_event_focus, group_idx_n, column_idx_n, row_idx_n )
, ev_focus ( event_n )
{
}

Pass_Event_Key::Pass_Event_Key ( const QKeyEvent & event_n,
                                 unsigned int group_idx_n,
                                 unsigned int column_idx_n,
                                 unsigned int row_idx_n )
: ::Wdg::Pass_Event (
      ::Wdg::evt_pass_event_key, group_idx_n, column_idx_n, row_idx_n )
, ev_key ( event_n )
{
}

} // namespace Wdg
