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

#ifndef __INC_qsnd_event_types_hpp__
#define __INC_qsnd_event_types_hpp__

#include <QEvent>


namespace QSnd
{


extern QEvent::Type evt_update_values_request;
extern QEvent::Type evt_update_values;

extern QEvent::Type evt_reload_request;
extern QEvent::Type evt_reload;


void
init_event_types ( );


} // End of namespace


#endif
