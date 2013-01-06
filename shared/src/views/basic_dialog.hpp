//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
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
	set_top_widget (
		QWidget * wdg_n );

	void
	set_central_widget (
		QWidget * wdg_n );

	void
	set_bottom_widget (
		QWidget * wdg_n );

	QLabel *
	create_title_widget (
		const QString & txt_n );

	QPushButton *
	create_close_button ( );


	// Private attributes
	private:

	QVBoxLayout * _lay_main;
};


} // End of namespace

#endif
