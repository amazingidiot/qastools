/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_static_tree_hpp__
#define __INC_static_tree_hpp__

#include <QList>
#include <QStack>
#include <cassert>

class Static_Tree_Node
{

  // Public typedefs
  public:
  typedef unsigned long Index_Type;

  // Public methods
  public:
  Static_Tree_Node ();

  unsigned long parent_idx;
  unsigned long self_idx;
  unsigned long children_idx;
};

class Static_Tree : public QList< Static_Tree_Node >
{
  public:
  typedef Static_Tree_Node Node;
  typedef Static_Tree_Node::Index_Type Index_Type;

  public:
  Static_Tree ();

  void
  reset ();

  Node *
  node ( Index_Type idx_n );

  const Node *
  node ( Index_Type idx_n ) const;

  bool
  is_root ( const Node * node_n ) const;

  const Node *
  root_node () const;

  Node *
  root_node ();

  const Node *
  parent_node ( const Node * node_n ) const;

  Node *
  parent_node ( const Node * node_n );

  const Node *
  child_node ( const Node * node_n, Index_Type child_n ) const;

  Node *
  child_node ( const Node * node_n, Index_Type child_n );

  Index_Type
  depth ( const Node * node_n ) const;

  Index_Type
  row ( const Node * node_n ) const;

  Index_Type
  num_children ( const Node * node_n ) const;

  /// @brief Calculates the rootline starting from the node downwards
  void
  rootline ( const Node * node_n, QList< unsigned int > & rows_n ) const;

  /// @brief Sets the number of children for a node and appends the children
  void
  append_children ( Node * node_n, Index_Type num_n );
};

inline bool
Static_Tree::is_root ( const Node * node_n ) const
{
  return ( node_n->self_idx == 0 );
}

inline const Static_Tree::Node *
Static_Tree::root_node () const
{
  return node ( 0 );
}

inline Static_Tree::Node *
Static_Tree::root_node ()
{
  return node ( 0 );
}

inline Static_Tree::Node *
Static_Tree::node ( Index_Type idx_n )
{
  if ( idx_n < Index_Type ( size () ) ) {
    return &operator[] ( idx_n );
  }
  return 0;
}

inline const Static_Tree::Node *
Static_Tree::node ( Index_Type idx_n ) const
{
  if ( idx_n < Index_Type ( size () ) ) {
    return &operator[] ( idx_n );
  }
  return 0;
}

#endif
