/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_hctl_editor_hpp__
#define __INC_mixer_hctl_editor_hpp__

#include <QAbstractButton>
#include <QGridLayout>
#include <QLabel>
#include <QLocale>
#include <QPalette>
#include <QWidget>

// Forward declaration
namespace QSnd
{
class Mixer_HCTL_Elem;
class Mixer_HCTL_Elem_Group;
} // namespace QSnd
namespace MWdg
{
class Mixer_HCTL_Editor_Data;
class Mixer_HCTL_Proxies_Group;
class Inputs_Setup;
} // namespace MWdg

namespace MWdg
{

/// @brief Mixer_HCTL_Editor
///
class Mixer_HCTL_Editor : public QWidget
{
  Q_OBJECT

  // Public typedefs
  public:
  // Public methods
  public:
  Mixer_HCTL_Editor ( Mixer_HCTL_Editor_Data * data_n, QWidget * parent_n = 0 );

  ~Mixer_HCTL_Editor ();

  // Editor data

  const Mixer_HCTL_Editor_Data *
  editor_data () const;

  Mixer_HCTL_Editor_Data *
  editor_data ();

  // Inputs setup

  const ::MWdg::Inputs_Setup *
  inputs_setup () const;

  virtual void
  set_inputs_setup ( const ::MWdg::Inputs_Setup * setup_n );

  virtual void
  update_proxies_values ();

  QWidget *
  create_small_joined_switch ( ::MWdg::Mixer_HCTL_Proxies_Group * pgroup_n,
                               ::QSnd::Mixer_HCTL_Elem * elem_n );

  // Protected methods
  protected:
  unsigned int
  elem_style_id ( const ::QSnd::Mixer_HCTL_Elem * elem_n ) const;

  QGridLayout *
  create_channel_grid ( const QList< QWidget * > & items_n,
                        bool bold_labels_n = false );

  // Private attributes
  private:
  Mixer_HCTL_Editor_Data * _editor_data;
  const ::MWdg::Inputs_Setup * _inputs_setup;
};

inline const Mixer_HCTL_Editor_Data *
Mixer_HCTL_Editor::editor_data () const
{
  return _editor_data;
}

inline Mixer_HCTL_Editor_Data *
Mixer_HCTL_Editor::editor_data ()
{
  return _editor_data;
}

inline const ::MWdg::Inputs_Setup *
Mixer_HCTL_Editor::inputs_setup () const
{
  return _inputs_setup;
}

} // namespace MWdg

#endif
