//
// C++ Interface:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//


#ifndef __INC_tree_view_kv_hpp__
#define __INC_tree_view_kv_hpp__

#include <QTreeView>

namespace Wdg
{


///
/// @brief Tree_View_KV
///
class Tree_View_KV :
	public QTreeView
{
	Q_OBJECT

	// Public methods
	public:

	Tree_View_KV (
		QWidget * parent = 0 );

	void
	setModel (
		QAbstractItemModel * model_n );


	// Activate current

	bool
	activate_current ( ) const;

	void
	set_activate_current (
		bool flag_n );


	// Public slots
	public slots:

	void
	set_expanded_recursive (
		const QModelIndex & index_n,
		int depth_n,
		bool expanded_n );

	void
	expand_recursive (
		const QModelIndex & index_n,
		int depth_n );

	void
	collapse_recursive (
		const QModelIndex & index_n,
		int depth_n );

	void
	adjust_first_column_width ( );


	// Protected methods
	protected:

	void
	currentChanged (
		const QModelIndex & current,
		const QModelIndex & previous );


	// Private attributes
	private:

	bool _activate_current;

};


inline
bool
Tree_View_KV::activate_current ( ) const
{
	return _activate_current;
}


} // End of namespace

#endif
