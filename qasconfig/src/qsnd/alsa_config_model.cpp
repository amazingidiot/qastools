/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "alsa_config_model.hpp"
#include <QFont>
#include <QString>
#include <QStringList>
#include <iostream>
#include <sstream>

namespace QSnd
{

Alsa_Config_Model::Alsa_Config_Model ()
: _snd_cfg_root ( 0 )
{
  reload ();
}

Alsa_Config_Model::~Alsa_Config_Model ()
{
  clear_config ();
}

void
Alsa_Config_Model::clear_config ()
{

  const int rows ( rowCount () );
  if ( rows > 0 ) {
    beginRemoveRows ( QModelIndex (), 0, rows - 1 );
  }

  stree ().reset ();
  if ( _snd_cfg_root != 0 ) {
    snd_config_delete ( _snd_cfg_root );
    _snd_cfg_root = 0;
  }

  if ( rows > 0 ) {
    endRemoveRows ();
  }
}

int
Alsa_Config_Model::load_config ()
{
  int err;

  {
    snd_config_update_t * snd_cfg_upd = 0;
    err = snd_config_update_r ( &_snd_cfg_root, &snd_cfg_upd, 0 );
    if ( snd_cfg_upd != 0 ) {
      snd_config_update_free ( snd_cfg_upd );
      snd_cfg_upd = 0;
    }
  }

  if ( ( err < 0 ) || ( _snd_cfg_root == 0 ) ) {
    {
      ::std::stringstream msg;
      msg << "[EE] Alsa configuration reading failed" << ::std::endl;
      ::std::cerr << msg.str ();
    }
    ::QSnd::print_alsa_error ( "snd_config_update_r()", err );
  } else {
    add_children_recursively ( stree ().root_node (), _snd_cfg_root );

    const int rows ( rowCount () );
    if ( rows > 0 ) {
      beginInsertRows ( QModelIndex (), 0, rows - 1 );
      endInsertRows ();
    }
  }

  return err;
}

int
Alsa_Config_Model::reload ()
{
  int res ( 0 );
  beginResetModel ();

  clear_config ();
  res = load_config ();

  endResetModel ();
  return res;
}

int
Alsa_Config_Model::add_children_recursively ( Node * node_n,
                                              snd_config_t * cfg_n )
{
  if ( ( node_n == 0 ) || ( cfg_n == 0 ) ) {
    return -1;
  }

  const int num_children = cfg_count_children ( cfg_n );

  if ( num_children > 0 ) {
    stree ().append_children ( node_n, num_children );

    snd_config_iterator_t iter;
    snd_config_iterator_t iter_end;
    iter = snd_config_iterator_first ( cfg_n );
    iter_end = snd_config_iterator_end ( cfg_n );

    int count ( 0 );
    while ( ( iter != iter_end ) && ( count < num_children ) ) {
      Node * node_child = stree ().child_node ( node_n, count );
      snd_config_t * cfg_child = snd_config_iterator_entry ( iter );

      add_children_recursively ( node_child, cfg_child );

      iter = snd_config_iterator_next ( iter );
      ++count;
    }
  }
  return 0;
}

snd_config_t *
Alsa_Config_Model::cfg_struct ( const Node * node_n ) const
{
  snd_config_t * res ( 0 );
  QList< unsigned int > rl;
  stree ().rootline ( node_n, rl );

  res = _snd_cfg_root;
  for ( int ii = rl.size () - 1; ii >= 0; --ii ) {
    res = cfg_child ( res, rl[ ii ] );
  }

  return res;
}

snd_config_t *
Alsa_Config_Model::cfg_struct ( const QModelIndex & index_n ) const
{
  snd_config_t * res ( 0 );
  const Node * node = get_node ( index_n );
  if ( node != 0 ) {
    res = cfg_struct ( node );
  }
  return res;
}

int
Alsa_Config_Model::cfg_count_children ( snd_config_t * cfg_n ) const
{
  int count ( 0 );

  if ( cfg_n != 0 ) {
    if ( snd_config_get_type ( cfg_n ) == SND_CONFIG_TYPE_COMPOUND ) {
      snd_config_iterator_t iter;
      snd_config_iterator_t iter_end;
      iter = snd_config_iterator_first ( cfg_n );
      iter_end = snd_config_iterator_end ( cfg_n );
      while ( iter != iter_end ) {
        ++count;
        iter = snd_config_iterator_next ( iter );
      }
    }
  }
  return count;
}

snd_config_t *
Alsa_Config_Model::cfg_child ( snd_config_t * cfg_n,
                               unsigned int index_n ) const
{
  snd_config_t * res ( 0 );

  if ( cfg_n != 0 ) {
    if ( snd_config_get_type ( cfg_n ) == SND_CONFIG_TYPE_COMPOUND ) {
      snd_config_iterator_t iter;
      snd_config_iterator_t iter_end;
      iter = snd_config_iterator_first ( cfg_n );
      iter_end = snd_config_iterator_end ( cfg_n );
      unsigned int count ( 0 );
      while ( iter != iter_end ) {
        if ( count == index_n ) {
          res = snd_config_iterator_entry ( iter );
          break;
        }
        iter = snd_config_iterator_next ( iter );
        ++count;
      }
    }
  }
  return res;
}

QString
Alsa_Config_Model::cfg_id_string ( snd_config_t * cfg_n ) const
{
  QString res;
  const char * char_ptr ( 0 );
  int err = snd_config_get_id ( cfg_n, &char_ptr );
  if ( ( err == 0 ) && ( char_ptr != 0 ) ) {
    res = char_ptr;
  }
  return res;
}

QString
Alsa_Config_Model::cfg_value_string ( snd_config_t * cfg_n ) const
{
  QString res;
  char * char_ptr ( 0 );
  int err = snd_config_get_ascii ( cfg_n, &char_ptr );
  if ( ( err == 0 ) && ( char_ptr != 0 ) ) {
    res = char_ptr;
    free ( char_ptr );
  }
  return res;
}

QVariant
Alsa_Config_Model::headerData ( int section,
                                Qt::Orientation orientation,
                                int role ) const
{
  if ( orientation == Qt::Horizontal ) {
    if ( role == Qt::DisplayRole ) {
      if ( section == 0 ) {
        return tr ( "Key" );
      } else if ( section == 1 ) {
        return tr ( "Value" );
      }
    }
  }
  return QVariant ();
}

Qt::ItemFlags
Alsa_Config_Model::flags ( const QModelIndex & ) const
{
  Qt::ItemFlags ff ( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
  return ff;
}

QVariant
Alsa_Config_Model::data ( const QModelIndex & index_n, int role_n ) const
{
  QVariant res;
  snd_config_t * cfg ( cfg_struct ( index_n ) );

  if ( cfg != 0 ) {
    if ( role_n == Qt::DisplayRole ) {
      if ( index_n.column () == 0 ) {
        res = cfg_id_string ( cfg );
      } else if ( index_n.column () == 1 ) {
        res = cfg_value_string ( cfg );
      }
    }
  }

  return res;
}

QString
Alsa_Config_Model::index_address_str ( const QModelIndex & index_n )
{
  QString res;

  QModelIndex idx ( index_n );
  while ( idx.isValid () ) {
    if ( !res.isEmpty () ) {
      res.prepend ( "." );
    }
    res.prepend ( data ( idx, Qt::DisplayRole ).toString () );
    idx = idx.parent ();
  }

  return res;
}

QModelIndex
Alsa_Config_Model::index_from_address ( const QString & addr_n )
{
  QModelIndex res;

  const QStringList lst ( addr_n.split ( "." ) );
  QModelIndex idx_base;
  for ( int depth = 0; depth < lst.size (); ++depth ) {
    unsigned int num_rows ( rowCount ( idx_base ) );
    for ( unsigned int row = 0; row < num_rows; ++row ) {
      const QModelIndex idx ( index ( row, 0, idx_base ) );
      if ( data ( idx, Qt::DisplayRole ).toString () == lst[ depth ] ) {
        if ( depth == ( lst.size () - 1 ) ) {
          res = idx;
        } else {
          idx_base = idx;
        }
        break;
      }
    }
  }

  return res;
}

} // namespace QSnd
