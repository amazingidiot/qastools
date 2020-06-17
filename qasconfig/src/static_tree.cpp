/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "static_tree.hpp"

Static_Tree_Node::Static_Tree_Node ()
: parent_idx ( 0 )
, self_idx ( 0 )
, children_idx ( 0 )
{
}

Static_Tree::Static_Tree ()
{
  reset ();
}

void
Static_Tree::reset ()
{
  clear ();

  Node nn_root;
  nn_root.parent_idx = 0;
  nn_root.self_idx = 0;
  nn_root.children_idx = 0;
  append ( nn_root );
}

const Static_Tree::Node *
Static_Tree::parent_node ( const Node * node_n ) const
{
  if ( node_n != 0 ) {
    if ( !is_root ( node_n ) ) {
      return node ( node_n->parent_idx );
    }
  }
  return 0;
}

Static_Tree::Node *
Static_Tree::parent_node ( const Node * node_n )
{
  if ( node_n != 0 ) {
    if ( !is_root ( node_n ) ) {
      return node ( node_n->parent_idx );
    }
  }
  return 0;
}

const Static_Tree::Node *
Static_Tree::child_node ( const Node * node_n, Index_Type child_n ) const
{
  if ( node_n != 0 ) {
    if ( node_n->children_idx > 0 ) {
      return node ( node_n->children_idx + child_n );
    }
  }
  return 0;
}

Static_Tree::Node *
Static_Tree::child_node ( const Node * node_n, Index_Type child_n )
{
  if ( node_n != 0 ) {
    if ( node_n->children_idx > 0 ) {
      return node ( node_n->children_idx + child_n );
    }
  }
  return 0;
}

Static_Tree::Index_Type
Static_Tree::depth ( const Node * node_n ) const
{
  Index_Type res ( 0 );
  if ( node_n != 0 ) {
    while ( !is_root ( node_n ) ) {
      node_n = parent_node ( node_n );
      ++res;
    }
  }
  return res;
}

Static_Tree::Index_Type
Static_Tree::row ( const Node * node_n ) const
{
  Index_Type idx ( 0 );
  if ( node_n != 0 ) {
    idx = node_n->self_idx;
    while ( idx > 0 ) {
      const Node * nn ( node ( idx ) );
      if ( nn->parent_idx != node_n->parent_idx ) {
        break;
      }
      --idx;
    }
    return ( node_n->self_idx - idx ) - 1;
  }
  return idx;
}

Static_Tree::Index_Type
Static_Tree::num_children ( const Node * node_n ) const
{
  Index_Type res ( 0 );
  if ( node_n != 0 ) {
    if ( node_n->children_idx > 0 ) {
      const Index_Type idx_lim ( size () );
      Index_Type idx ( node_n->children_idx );
      while ( idx < idx_lim ) {
        const Node * node_c = node ( idx );
        if ( node_c->parent_idx != node_n->self_idx ) {
          break;
        }
        ++res;
        ++idx;
      }
    }
  }
  return res;
}

void
Static_Tree::rootline ( const Node * node_n,
                        QList< unsigned int > & rows_n ) const
{
  rows_n.clear ();
  if ( node_n != 0 ) {
    while ( !is_root ( node_n ) ) {
      rows_n.append ( row ( node_n ) );
      node_n = parent_node ( node_n );
    }
  }
}

void
Static_Tree::append_children ( Node * node_n, Index_Type num_n )
{
  if ( ( node_n == 0 ) || ( num_n <= 0 ) ) {
    return;
  }

  node_n->children_idx = size ();
  for ( Index_Type ii = 0; ii < num_n; ++ii ) {
    Node nn;
    nn.parent_idx = node_n->self_idx;
    nn.self_idx = size ();
    append ( nn );
  }
}
