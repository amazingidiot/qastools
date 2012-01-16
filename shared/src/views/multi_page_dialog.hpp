//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_views_multi_page_dialog_hpp__
#define __INC_views_multi_page_dialog_hpp__

#include <QList>
#include <QStackedLayout>
#include <QModelIndex>
#include <QListView>
#include <QStandardItem>
#include <QStandardItemModel>

#include "basic_dialog.hpp"

namespace Views
{


/// @brief Multi_Page_Dialog
///
class Multi_Page_Dialog :
	public ::Views::Basic_Dialog
{
	Q_OBJECT

	// Public methods
	public:

	Multi_Page_Dialog (
		QWidget * parent_n = 0,
		Qt::WindowFlags flags_n = 0 );

	~Multi_Page_Dialog ( );

	void
	add_page_vscroll (
		const QString & name_n,
		QWidget * wdg_n );

	void
	add_page (
		const QString & name_n,
		QWidget * wdg_n );

	unsigned int
	num_pages ( ) const;

	int
	current_page_idx ( ) const;

	void
	set_current_page_idx (
		int idx_n );


	// Protected slots
	protected slots:

	void
	page_changed (
		const QModelIndex & cur_n,
		const QModelIndex & prev_n );

	void
	page_selected (
		const QModelIndex & index_n );


	// Protected methods
	protected:


	// Private attributes
	private:

	// Pages
	QListView * _page_selection;
	QStackedLayout * _lay_pages_stack;
	QStandardItemModel * _pages_model;
};


} // End of namespace

#endif
