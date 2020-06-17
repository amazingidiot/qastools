/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_controls_model_hpp__
#define __INC_qsnd_controls_model_hpp__

#include <QStandardItemModel>

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
class Controls_Model : public QStandardItemModel
{
  Q_OBJECT;

  public:
  // -- Types

  enum Extra_Roles
  {
    ROLE_DB_INDEX = ( Qt::UserRole + 1 ),
    ROLE_L10N_ARGS = ( Qt::UserRole + 2 )
  };

  // -- Construction

  Controls_Model ( QObject * parent_n = 0 );

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

  const ::QSnd::CTL_Format *
  ctl_format ( const QModelIndex & idx_n ) const;

  void
  ctl_format ( ::QSnd::CTL_Format & ctl_format_n,
               const QModelIndex & idx_n ) const;

  QModelIndex
  ctl_format_index ( const QString & ctl_addr_n ) const;

  Q_SLOT
  void
  reload ();

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
};

} // namespace QSnd

#endif
