/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_hctl_editor_data_hpp__
#define __INC_mixer_hctl_editor_data_hpp__

#include <QLocale>
#include <QString>

// Forward declaration
namespace QSnd
{
class Mixer_HCTL_Info_Db;
class Mixer_HCTL_Elem;
class Mixer_HCTL_Elem_Group;
} // namespace QSnd
namespace dpe
{
class Image_Allocator;
}
namespace Wdg
{
class DS_Widget_Style_Db;
}

namespace MWdg
{

/// @brief Mixer_HCTL_Editor_Data
///
class Mixer_HCTL_Editor_Data
{
  // Public methods
  public:
  Mixer_HCTL_Editor_Data ();

  ~Mixer_HCTL_Editor_Data ();

  // Public attributes
  public:
  const ::QSnd::Mixer_HCTL_Info_Db * ctl_info_db;
  const ::Wdg::DS_Widget_Style_Db * wdg_style_db;

  // Group and element selection
  ::QSnd::Mixer_HCTL_Elem_Group * snd_elem_group;
  unsigned int elem_idx;

  ::dpe::Image_Allocator * image_alloc;

  // Strings
  QLocale loc;
  QString str_joined;
  QString ttip_grid_lbl_elem;
  QString ttip_grid_lbl_channel;
  QString ttip_grid_widget;

  QString str_list_channel;
  QString ttip_list_channel;
};

} // namespace MWdg

#endif
