/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_pad_focus_info_hpp__
#define __INC_pad_focus_info_hpp__

namespace Wdg
{

/// @brief Specifies which element of a slider/switches pad has the focus
///
class Pad_Focus_Info
{
  // Public methods
  public:
  Pad_Focus_Info ();

  void
  clear ();

  // Public attributes

  bool has_focus;
  unsigned int group_idx;
  unsigned int column_idx;
  unsigned int row_idx;
};

} // namespace Wdg

#endif
