/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "static_tree_model.hpp"

Static_Tree_Model::Static_Tree_Model ()
: _num_columns ( 2 )
{
}

Static_Tree_Model::~Static_Tree_Model () {}

void
Static_Tree_Model::set_num_columns ( int num_n )
{
  _num_columns = num_n;
}

const Static_Tree_Model::Node *
Static_Tree_Model::get_node ( const QModelIndex & index_n ) const
{
  int idx ( 0 );
  if ( index_n.isValid () ) {
    idx = index_n.internalId ();
  }
  return stree ().node ( idx );
}

QModelIndex
Static_Tree_Model::index ( const Node * node_n ) const
{
  if ( node_n != 0 ) {
    const Node_Index node_row ( stree ().row ( node_n ) );
    return createIndex ( node_row, 0, quint32 ( node_n->self_idx ) );
  }
  return QModelIndex ();
}

QModelIndex
Static_Tree_Model::index ( int row,
                           int column,
                           const QModelIndex & parent_n ) const
{
  if ( ( parent_n.column () > 0 ) ) {
    return QModelIndex ();
  }

  const Node * node_p ( get_node ( parent_n ) );
  if ( node_p != 0 ) {
    if ( Node_Index ( row ) < stree ().num_children ( node_p ) ) {
      quint32 idx = node_p->children_idx + row;
      return createIndex ( row, column, idx );
    }
  }

  return QModelIndex ();
}

QModelIndex
Static_Tree_Model::parent ( const QModelIndex & index_n ) const
{
  const long idx ( index_n.internalId () );
  if ( index_n.isValid () && ( idx > 0 ) ) {
    const Node * node ( stree ().node ( idx ) );
    if ( node != 0 ) {
      const int ndepth ( stree ().depth ( node ) );
      if ( ndepth > 1 ) {
        node = stree ().parent_node ( node );
        return index ( node );
      }
    }
  }
  return QModelIndex ();
}

int
Static_Tree_Model::rowCount ( const QModelIndex & parent_n ) const
{
  int res ( 0 );

  const Node * node ( 0 );

  if ( parent_n.isValid () ) {
    if ( parent_n.column () == 0 ) {
      node = stree ().node ( parent_n.internalId () );
    }
  } else {
    node = stree ().node ( 0 );
  }

  if ( node != 0 ) {
    res = stree ().num_children ( node );
  }

  return res;
}

int
Static_Tree_Model::columnCount ( const QModelIndex & index_n ) const
{
  int res ( 0 );
  if ( rowCount ( index_n ) > 0 ) {
    res = _num_columns;
  }
  return res;
}
