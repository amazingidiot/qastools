/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_hctl_editor_bool_hpp__
#define __INC_mixer_hctl_editor_bool_hpp__

#include "mwdg/mixer_hctl_editor.hpp"
#include <QList>

namespace MWdg
{

// Forward declaration
class Mixer_HCTL_Proxies_Group;

/// @brief Mixer_HCTL_Edit_Bool
///
class Mixer_HCTL_Edit_Bool : public Mixer_HCTL_Editor
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_HCTL_Edit_Bool ( Mixer_HCTL_Editor_Data * data_n,
                         QWidget * parent_n = 0 );

  ~Mixer_HCTL_Edit_Bool ();

  void
  setup_single ();

  void
  setup_multi ();

  Mixer_HCTL_Proxies_Group *
  create_proxies_group ( ::QSnd::Mixer_HCTL_Elem * elem_n );

  // Public slots
  public slots:

  void
  update_proxies_values ();

  // Private attributes
  private:
  QList< Mixer_HCTL_Proxies_Group * > _proxies_groups;
};

} // namespace MWdg

#endif
