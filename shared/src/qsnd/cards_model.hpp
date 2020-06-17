/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_qsnd_cards_model_hpp__
#define __INC_qsnd_cards_model_hpp__

#include "qsnd/card_info.hpp"
#include <QAbstractListModel>
#include <memory>
#include <vector>

namespace QSnd
{

/// @brief Cards_Model
///
class Cards_Model : public QAbstractListModel
{
  Q_OBJECT;

  public:
  // -- Types

  using Info_Handle = std::shared_ptr<::QSnd::Card_Info >;
  using Const_Info_Handle = std::shared_ptr< const ::QSnd::Card_Info >;
  using Card_Infos = std::vector< Const_Info_Handle >;

  enum Extra_Roles
  {
    ROLE_INDEX = ( Qt::UserRole + 1 ),
    ROLE_ID = ( Qt::UserRole + 2 ),
    ROLE_DRIVER = ( Qt::UserRole + 3 ),
    ROLE_NAME = ( Qt::UserRole + 4 ),
    ROLE_LONG_NAME = ( Qt::UserRole + 5 ),
    ROLE_MIXER_NAME = ( Qt::UserRole + 6 ),
    ROLE_COMPONENTS = ( Qt::UserRole + 7 )
  };

  // -- Properties

  Q_PROPERTY ( int count READ count NOTIFY countChanged )

  // -- Construction

  Cards_Model ( QObject * parent_n = nullptr );

  ~Cards_Model ();

  // -- Card info accessors

  int
  count () const
  {
    return _cards.size ();
  }

  Q_SIGNAL
  void
  countChanged ();

  std::size_t
  num_cards () const
  {
    return _cards.size ();
  }

  const Const_Info_Handle &
  card_info ( std::size_t index_n ) const
  {
    return _cards[ index_n ];
  }

  Const_Info_Handle
  card_info_by_card_index ( int index_n );

  Const_Info_Handle
  card_info_by_model_index ( const QModelIndex & idx_n ) const;

  QModelIndex
  model_index_by_card_id ( const QString & id_str_n ) const;

  /// @brief Reload cards list
  Q_SLOT
  void
  reload ();

  // -- Model interface

  QHash< int, QByteArray >
  roleNames () const override;

  int
  rowCount ( const QModelIndex & parent_n = QModelIndex () ) const override;

  QVariant
  data ( const QModelIndex & index_n,
         int role_n = Qt::DisplayRole ) const override;

  protected:
  // -- Utility

  void
  load_cards ( Card_Infos & card_infos_n );

  private:
  // -- Attributes
  Card_Infos _cards;
};

} // namespace QSnd

#endif
