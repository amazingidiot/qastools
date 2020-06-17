/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_views_alsa_config_view_hpp__
#define __INC_views_alsa_config_view_hpp__

#include <QCheckBox>
#include <QModelIndex>
#include <QPushButton>
#include <QSpinBox>
#include <QStringList>

// Forward declaration
class QAbstractItemModel;
class QSortFilterProxyModel;
class QItemSelection;
namespace QSnd
{
class Alsa_Config_Model;
}
namespace Wdg
{
class Tree_View_KV;
}

namespace Views
{

class Alsa_Config_View : public QWidget
{
  Q_OBJECT

  // Public methods
  public:
  Alsa_Config_View ( QWidget * parent_n = 0 );

  ~Alsa_Config_View ();

  ::QSnd::Alsa_Config_Model *
  model () const;

  void
  set_model ( ::QSnd::Alsa_Config_Model * model_n );

  bool
  sorting_enabled () const;

  // Public slots
  public slots:

  void
  reload_config ();

  void
  expand_to_level ( bool expanded_n );

  void
  expand_to_level ();

  void
  collapse_to_level ();

  void
  set_sorting_enabled ( bool flag_n );

  // Protected slots
  protected slots:

  void
  items_selected ( const QItemSelection & sel0_n,
                   const QItemSelection & sel1_n );

  // Protected methods
  protected:
  void
  collect_expanded ( QStringList & lst_n,
                     const QModelIndex & = QModelIndex () );

  void
  set_expanded ( QStringList & lst_n );

  void
  update_button_state ();

  // Private attributes
  private:
  ::QSnd::Alsa_Config_Model * _alsa_cfg;
  QSortFilterProxyModel * _sort_model;

  // Widgets
  ::Wdg::Tree_View_KV * _tree_view;
  QPushButton _btn_expand;
  QPushButton _btn_collapse;
  QCheckBox _btn_sort;
  QSpinBox _expand_depth;

  QStringList _expanded_items;
};

inline ::QSnd::Alsa_Config_Model *
Alsa_Config_View::model () const
{
  return _alsa_cfg;
}

} // namespace Views

#endif
