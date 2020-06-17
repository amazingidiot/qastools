/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_hctl_editor_unsupported_hpp__
#define __INC_mixer_hctl_editor_unsupported_hpp__

#include "mwdg/mixer_hctl_editor.hpp"

namespace MWdg
{

/// @brief Mixer_HCTL_Edit_Unsupported
///
class Mixer_HCTL_Edit_Unsupported : public Mixer_HCTL_Editor
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_HCTL_Edit_Unsupported ( Mixer_HCTL_Editor_Data * data_n,
                                QWidget * parent_n = 0 );

  ~Mixer_HCTL_Edit_Unsupported ();

  void
  setup_single ();

  // Private attributes
  private:
  QString _str_unsupported;
};

} // namespace MWdg

#endif
