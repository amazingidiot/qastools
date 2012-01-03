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

#ifndef __INC_views_basic_dialog_hpp__
#define __INC_views_basic_dialog_hpp__

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>


namespace Views
{


/// @brief Basic_Dialog
///
class Basic_Dialog :
	public QDialog
{
	Q_OBJECT

	// Public methods
	public:

	Basic_Dialog (
		QWidget * parent_n = 0,
		Qt::WindowFlags flags_n = 0 );

	~Basic_Dialog ( );

	void
	set_title_str (
		const QString & str_n );

	void
	set_central_widget (
		QWidget * wdg_n );

	static
	QLabel *
	create_title_widget (
		const QString & txt_n );

	QPushButton *
	create_close_button ( );


	// Private attributes
	private:

	// Pages
	QLabel * _lbl_title;
	QVBoxLayout * _lay_main;
};


} // End of namespace


#endif
