/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_ds_widget_types_hpp__
#define __INC_ds_widget_types_hpp__

namespace Wdg
{

enum DS_Widget_Type
{
  DS_UNKNOWN = 0,
  DS_SWITCH = 1,
  DS_SLIDER = 2
};

enum DS_Widget_State
{
  ST_NORMAL = 0,
  ST_DISABLED = 1,
  ST_INACTIVE = 2
};

enum DS_Switch_Variants
{
  DS_CIRCLE = 0,
  DS_SVG_JOINED = 1,
  DS_CLOSE = 2
};

} // namespace Wdg

#endif
