/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_hctl_tree_model_hpp__
#define __INC_mixer_hctl_tree_model_hpp__

#include <QAbstractItemModel>
#include <QList>
#include <QStandardItemModel>
#include <QStringList>

// Forward declaration
namespace QSnd
{
class Mixer_HCTL;
class Mixer_HCTL_Elem;
class Mixer_HCTL_Elem_Group;
} // namespace QSnd

namespace MWdg
{

/// @brief Mixer_HCTL_Tree_Model
///
class Mixer_HCTL_Tree_Model : public QStandardItemModel
{
  Q_OBJECT;

  // Public methods
  public:
  Mixer_HCTL_Tree_Model ( QObject * parent_n = 0 );

  ~Mixer_HCTL_Tree_Model ();

  // Mixer class

  ::QSnd::Mixer_HCTL *
  snd_mixer () const;

  void
  set_snd_mixer ( ::QSnd::Mixer_HCTL * snd_mixer_n );

  void
  set_snd_dir_foreground ( unsigned int dir_n, const QBrush & brush_n );

  /// @return 0 - nothing; 1 - iface_type; 2 - group/element; 3 - group/element
  unsigned int
  index_data ( const QModelIndex & idx_n,
               ::QSnd::Mixer_HCTL_Elem_Group ** grp_n,
               unsigned int * elem_index_n,
               unsigned int * iface_type_n = 0 ) const;

  QModelIndex
  elem_index ( ::QSnd::Mixer_HCTL_Elem * elem_n ) const;

  QModelIndex
  elem_desc_index ( const QString & iface_name_n,
                    const QString & elem_name_n,
                    unsigned int elem_idx_n ) const;

  // Protected typedefs
  protected:
  struct Type_Group
  {
    unsigned int iface_type_idx;
    QList<::QSnd::Mixer_HCTL_Elem_Group * > list;
  };

  // Private methods
  private:
  void
  clear ();

  void
  load ();

  void
  compress_group_list ( QList<::QSnd::Mixer_HCTL_Elem_Group * > & lst_n );

  QBrush
  get_foreground ( const QString & name_n );

  // Private attributes
  private:
  ::QSnd::Mixer_HCTL * _snd_mixer;
  QList< Type_Group * > _type_groups;

  QBrush _brush_snd_dir[ 2 ];
};

inline ::QSnd::Mixer_HCTL *
Mixer_HCTL_Tree_Model::snd_mixer () const
{
  return _snd_mixer;
}

} // namespace MWdg

#endif
