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

#ifndef __INC_views_info_view_hpp__
#define __INC_views_info_view_hpp__

#include <QWidget>
#include <QLabel>
#include <QTabWidget>


// Forward declaration
namespace Wdg { class Text_Browser; }


namespace Views
{


/// @brief Info_View
///
class Info_View :
	public QWidget
{
	Q_OBJECT

	// Public methods
	public:

	Info_View (
		QWidget * parent_n = 0 );


	// Signals
	signals:

	void
	sig_close ( );


	// Protected methods
	protected:

	bool
	read_utf8_file (
		const QString & filename_n,
		QString & txt_n ) const;


	// Private attributes
	private:

	QLabel _title;
	QTabWidget _tabs;
	::Wdg::Text_Browser * _txt_info;
	::Wdg::Text_Browser * _txt_people;
	::Wdg::Text_Browser * _txt_license;
};


} // End of namespace

#endif
