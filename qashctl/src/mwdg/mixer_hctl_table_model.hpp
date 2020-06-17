/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mixer_hctl_table_model_hpp__
#define __INC_mixer_hctl_table_model_hpp__

#include <QAbstractTableModel>
#include <QList>
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

/// @brief Mixer_HCTL_Table_Model
///
class Mixer_HCTL_Table_Model : public QAbstractTableModel
{
  Q_OBJECT;

  // Public methods
  public:
  Mixer_HCTL_Table_Model ( QObject * parent_n = 0 );

  ~Mixer_HCTL_Table_Model ();

  // Mixer class

  ::QSnd::Mixer_HCTL *
  snd_mixer () const;

  void
  set_snd_mixer ( ::QSnd::Mixer_HCTL * snd_mixer_n );

  // Interface type index

  unsigned int
  iface_type_idx () const;

  void
  set_iface_type_idx ( unsigned int type_n );

  ::QSnd::Mixer_HCTL_Elem *
  elem ( const QModelIndex & idx_n ) const;

  int
  rowCount ( const QModelIndex & parent_n = QModelIndex () ) const;

  int
  columnCount ( const QModelIndex & parent_n = QModelIndex () ) const;

  QVariant
  data ( const QModelIndex & index_n, int role_n = Qt::DisplayRole ) const;

  QVariant
  headerData ( int section_n,
               Qt::Orientation orientation_n,
               int role_n = Qt::DisplayRole ) const;

  // Protected methods
  protected:
  void
  clear ();

  void
  load ();

  // Private attributes
  private:
  ::QSnd::Mixer_HCTL * _snd_mixer;

  QList<::QSnd::Mixer_HCTL_Elem * > _elems;

  unsigned int _iface_type_idx;

  QString _str_dev_mask;
  QString _ttip_dev;
  QString _ttip_name_mask;

  QString _col_names[ 7 ];
  QString _col_ttips[ 7 ];

  const unsigned int _num_columns;
  const Qt::Alignment _align_cc;
  const Qt::Alignment _align_lc;
};

inline ::QSnd::Mixer_HCTL *
Mixer_HCTL_Table_Model::snd_mixer () const
{
  return _snd_mixer;
}

inline unsigned int
Mixer_HCTL_Table_Model::iface_type_idx () const
{
  return _iface_type_idx;
}

} // namespace MWdg

#endif
