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

#ifndef __INC_dev_select_view_setup_hpp__
#define __INC_dev_select_view_setup_hpp__

#include <QKeySequence>
#include <QList>
#include <QStringList>


// Forward declaration
namespace QSnd {
	class CTL_Address;
}


namespace Views
{


/// @brief Dev_Select_View_Setup
///
class Dev_Select_View_Setup
{
	// Public methods
	public:

	Dev_Select_View_Setup ( );

	~Dev_Select_View_Setup ( );

	void
	sel_db_clear ( );

	QStringList
	sel_db_get ( ) const;

	void
	sel_db_set (
		const QStringList & sel_db_n );


	// Public attributes
	public:

	QKeySequence kseq_toggle_vis;
	QList < const ::QSnd::CTL_Address * > selection_db;
};


} // End of namespace

#endif
