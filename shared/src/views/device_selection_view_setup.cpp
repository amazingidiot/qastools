//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "device_selection_view_setup.hpp"

#include "qsnd/ctl_address.hpp"


namespace Views
{


Device_Selection_View_Setup::Device_Selection_View_Setup ( ) :
kseq_toggle_vis ( Qt::Key_F6 )
{
}


Device_Selection_View_Setup::~Device_Selection_View_Setup ( )
{
	sel_db_clear();
}


void
Device_Selection_View_Setup::sel_db_clear ( )
{
	if ( selection_db.size() > 0 ) {
		for ( int ii=0; ii < selection_db.size(); ++ii ) {
			delete selection_db[ii];
		}
		selection_db.clear();
	}
}


QStringList
Device_Selection_View_Setup::sel_db_get ( ) const
{
	QStringList lst;
	for ( int ii=0; ii < selection_db.size(); ++ii ) {
		lst.append ( selection_db[ii]->addr_str() );
	}
	return lst;
}


void
Device_Selection_View_Setup::sel_db_set (
	const QStringList & sel_db_n )
{
	sel_db_clear();
	for ( int ii=0; ii < sel_db_n.size(); ++ii ) {
		if ( !sel_db_n[ii].isEmpty() ) {
			selection_db.append ( new ::QSnd::CTL_Address ( sel_db_n[ii] ) );
		}
	}
}


} // End of namespace