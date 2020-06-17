/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_controls_model_hpp__
#define __INC_qsnd_controls_model_hpp__

#include <QAbstractListModel>
#include <memory>
#include <vector>

// Forward declaration
namespace QSnd
{
class CTL_Format;
class Controls_Database;
} // namespace QSnd

namespace QSnd
{

/// @brief Controls_Model
///
class Controls_Model : public QAbstractListModel
{
  Q_OBJECT;

  public:
  // -- Types

  enum Extra_Roles
  {
    ROLE_L10N_ARGS = ( Qt::UserRole + 1 )
  };

  // -- Properties

  Q_PROPERTY ( int count READ count NOTIFY countChanged )

  // -- Construction

  Controls_Model ( QObject * parent_n = nullptr );

  ~Controls_Model ();

  // -- Controls database

  ::QSnd::Controls_Database *
  controls_db () const
  {
    return _ctl_db;
  }

  void
  set_controls_db ( ::QSnd::Controls_Database * ctl_db_n );

  // -- Control definition access

  int
  count () const
  {
    return _controls.size ();
  }

  Q_SIGNAL
  void
  countChanged ();

  const ::QSnd::CTL_Format *
  control_by_index ( const QModelIndex & idx_n ) const;

  QModelIndex
  control_index ( const QString & ctl_addr_n ) const;

  Q_SLOT
  void
  reload ();

  // -- Model inteface

  QHash< int, QByteArray >
  roleNames () const override;

  int
  rowCount ( const QModelIndex & parent_n = QModelIndex () ) const override;

  QVariant
  data ( const QModelIndex & index_n,
         int role_n = Qt::DisplayRole ) const override;

  protected:
  // -- Utility

  Q_SLOT
  void
  reload_begin ();

  Q_SLOT
  void
  reload_finish ();

  void
  load_data ();

  private:
  // -- Attributes
  ::QSnd::Controls_Database * _ctl_db = nullptr;
  std::vector< std::shared_ptr< const ::QSnd::CTL_Format > > _controls;
};

} // namespace QSnd

#endif
