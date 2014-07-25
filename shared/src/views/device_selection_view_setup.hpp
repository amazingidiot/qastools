//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_device_selection_view_setup_hpp__
#define __INC_device_selection_view_setup_hpp__

#include <QKeySequence>
#include <QList>
#include <QStringList>


// Forward declaration
namespace QSnd {
	class CTL_Address;
}


namespace Views
{


/// @brief Device_Selection_View_Setup
///
class Device_Selection_View_Setup
{
	// Public methods
	public:

	Device_Selection_View_Setup ( );

	~Device_Selection_View_Setup ( );

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
