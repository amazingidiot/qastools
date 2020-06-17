/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_views_mixer_hctl_hpp__
#define __INC_views_mixer_hctl_hpp__

#include "views/view_base.hpp"
#include <QStackedLayout>
#include <QTableView>

// Forward declaration
namespace QSnd
{
class Mixer_HCTL;
}
namespace Wdg
{
class Tree_View_KV;
}
namespace MWdg
{
class Mixer_HCTL_Tree_Model;
class Mixer_HCTL_Table_Model;
class Mixer_HCTL;
} // namespace MWdg
namespace Views
{
class Mixer_HCTL_Setup;
}

namespace Views
{

class Mixer_HCTL : public ::Views::View_Base
{
  Q_OBJECT

  // Public methods
  public:
  Mixer_HCTL ( QWidget * parent_n = 0 );

  ~Mixer_HCTL ();

  // Mixer device and view setup

  void
  set_mdev_setup ( const ::MWdg::Mixer_Device_Setup * setup_n );

  void
  set_inputs_setup ( const ::MWdg::Inputs_Setup * setup_n );

  void
  set_view_setup ( ::Views::View_Base_Setup * setup_n );

  // Protected slots
  protected slots:

  void
  tree_element_selected ( const QModelIndex & idx_n );

  void
  table_element_selected ( const QModelIndex & idx_n );

  // Protected methods
  protected:
  void
  setup_view ();

  void
  expand_tree_items ();

  void
  adjust_table_columns ();

  void
  restore_state ();

  // Private attributes
  private:
  ::Views::Mixer_HCTL_Setup * _view_setup;

  // Databases
  ::QSnd::Mixer_HCTL * _snd_mixer;
  ::MWdg::Mixer_HCTL_Tree_Model * _tree_model;
  ::MWdg::Mixer_HCTL_Table_Model * _table_model;

  // Side widgets
  QWidget _wdg_side;
  ::Wdg::Tree_View_KV * _tree_view;

  // Central widgets
  QWidget _wdg_center;
  QStackedLayout * _lay_center_stack;
  QTableView * _table_view;
  ::MWdg::Mixer_HCTL * _mixer_hctl;

  unsigned int _default_iface_type_idx;
};

} // namespace Views

#endif
