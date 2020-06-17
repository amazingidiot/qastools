/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_static_tree_model_hpp__
#define __INC_static_tree_model_hpp__

#include "static_tree.hpp"
#include <QAbstractItemModel>

/// @brief Static_Tree_Model
///
class Static_Tree_Model : public QAbstractItemModel
{
  // Public typedefs
  public:
  typedef Static_Tree::Index_Type Node_Index;
  typedef Static_Tree::Node Node;

  // Public methods
  public:
  Static_Tree_Model ();

  ~Static_Tree_Model ();

  int
  num_columns () const;

  void
  set_num_columns ( int num_n );

  const Node *
  get_node ( const QModelIndex & index_n ) const;

  QModelIndex
  index ( const Node * node_n ) const;

  // Model methods

  QModelIndex
  index ( int row,
          int column = 0,
          const QModelIndex & parent = QModelIndex () ) const;

  QModelIndex
  parent ( const QModelIndex & index ) const;

  int
  rowCount ( const QModelIndex & parent = QModelIndex () ) const;

  int
  columnCount ( const QModelIndex & parent = QModelIndex () ) const;

  // Protected methods
  protected:
  Static_Tree &
  stree ();

  const Static_Tree &
  stree () const;

  // Private attributes
  private:
  Static_Tree _stree;

  int _num_columns;
};

inline int
Static_Tree_Model::num_columns () const
{
  return _num_columns;
}

inline Static_Tree &
Static_Tree_Model::stree ()
{
  return _stree;
}

inline const Static_Tree &
Static_Tree_Model::stree () const
{
  return _stree;
}

#endif
