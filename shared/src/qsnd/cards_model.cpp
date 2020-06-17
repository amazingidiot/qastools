/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "cards_model.hpp"
#include "qsnd/alsa.hpp"
#include "qsnd/udev_device_lookout.hpp"
#include <algorithm>
#include <iostream>

namespace QSnd
{

Cards_Model::Cards_Model ( QObject * parent_n )
: QAbstractListModel ( parent_n )
{
  connect ( this,
            &QAbstractListModel::rowsInserted,
            this,
            &Cards_Model::countChanged );
  connect ( this,
            &QAbstractListModel::rowsRemoved,
            this,
            &Cards_Model::countChanged );

  // Device lookout
  {
    auto * lookout = new ::QSnd::UDev_Device_Lookout ( this );
    connect ( lookout, SIGNAL ( sig_change () ), this, SLOT ( reload () ) );
  }

  // Initial reload
  reload ();
}

Cards_Model::~Cards_Model () {}

Cards_Model::Const_Info_Handle
Cards_Model::card_info_by_card_index ( int index_n )
{
  for ( auto & item : _cards ) {
    if ( item->index () == index_n ) {
      return item;
    }
  }
  return Const_Info_Handle ();
}

Cards_Model::Const_Info_Handle
Cards_Model::card_info_by_model_index ( const QModelIndex & idx_n ) const
{
  if ( !idx_n.isValid () || idx_n.parent ().isValid () ||
       ( idx_n.column () != 0 ) || ( idx_n.row () < 0 ) ||
       ( idx_n.row () >= static_cast< int > ( _cards.size () ) ) ) {
    return Const_Info_Handle ();
  }
  return _cards[ idx_n.row () ];
}

QModelIndex
Cards_Model::model_index_by_card_id ( const QString & id_str_n ) const
{
  bool is_int = false;
  int int_val = id_str_n.toInt ( &is_int );

  int ii = 0;
  for ( const auto & item : _cards ) {
    if ( is_int && ( item->index () == int_val ) ) {
      return index ( ii, 0 );
    }
    if ( item->id () == id_str_n ) {
      return index ( ii, 0 );
    }
    ++ii;
  }

  return QModelIndex ();
}

void
Cards_Model::reload ()
{
  Card_Infos cards;
  // Reserve some more space
  cards.reserve ( _cards.size () + 4 );
  load_cards ( cards );

  // -- Remove disappeared cards
  for ( auto it = _cards.cbegin (); it != _cards.cend (); ) {
    auto itf =
        std::find_if ( cards.cbegin (),
                       cards.cend (),
                       [& card = *it] ( const Const_Info_Handle & card_n ) {
                         return *card == *card_n;
                       } );
    if ( itf != cards.cend () ) {
      ++it;
      continue;
    }
    // Not found. Erase.
    {
      auto index = std::distance ( _cards.cbegin (), it );
      beginRemoveRows ( QModelIndex (), index, index );
      it = _cards.erase ( it );
      endRemoveRows ();
    }
  }

  // -- Insert new cards
  for ( const auto & card : cards ) {
    auto itf = std::find_if ( _cards.cbegin (),
                              _cards.cend (),
                              [&card] ( const Const_Info_Handle & card_n ) {
                                return *card == *card_n;
                              } );
    if ( itf != _cards.cend () ) {
      continue;
    }
    // Not found. Insert.
    {
      auto index = _cards.size ();
      beginInsertRows ( QModelIndex (), index, index );
      _cards.emplace_back ( card );
      endInsertRows ();
    }
  }
}

void
Cards_Model::load_cards ( Card_Infos & card_infos_n )
{
  int card_idx = -1;
  while ( true ) {
    if ( ::snd_card_next ( &card_idx ) != 0 ) {
      break;
    }
    if ( card_idx < 0 ) {
      break;
    }
    card_infos_n.emplace_back (
        std::make_shared<::QSnd::Card_Info > ( card_idx ) );
  }
  card_infos_n.shrink_to_fit ();
}

QHash< int, QByteArray >
Cards_Model::roleNames () const
{
  auto res = QAbstractListModel::roleNames ();
  res.insert ( ROLE_INDEX, "index" );
  res.insert ( ROLE_ID, "id" );
  res.insert ( ROLE_DRIVER, "driver" );
  res.insert ( ROLE_NAME, "name" );
  res.insert ( ROLE_LONG_NAME, "longName" );
  res.insert ( ROLE_MIXER_NAME, "mixerName" );
  res.insert ( ROLE_COMPONENTS, "components" );
  return res;
}

int
Cards_Model::rowCount ( const QModelIndex & parent_n ) const
{
  if ( parent_n.isValid () ) {
    return 0;
  }
  return _cards.size ();
}

QVariant
Cards_Model::data ( const QModelIndex & index_n, int role_n ) const
{
  auto handle = card_info_by_model_index ( index_n );
  if ( handle ) {
    switch ( role_n ) {
    case Qt::DisplayRole:
      return QVariant ( handle->name () );
    case ROLE_INDEX:
      return QVariant ( handle->index () );
    case ROLE_ID:
      return QVariant ( handle->id () );
    case ROLE_DRIVER:
      return QVariant ( handle->driver () );
    case ROLE_NAME:
      return QVariant ( handle->name () );
    case ROLE_LONG_NAME:
      return QVariant ( handle->long_name () );
    case ROLE_MIXER_NAME:
      return QVariant ( handle->mixer_name () );
    case ROLE_COMPONENTS:
      return QVariant ( handle->components () );
    default:
      break;
    }
  }
  return QVariant ();
}

} // namespace QSnd
