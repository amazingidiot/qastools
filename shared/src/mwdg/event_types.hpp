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

#ifndef __INC_mwdg_event_types_hpp__
#define __INC_mwdg_event_types_hpp__

#include <QEvent>


namespace MWdg
{

extern ::QEvent::Type evt_separation_request;
extern ::QEvent::Type evt_separation;

extern ::QEvent::Type evt_values_changed;
extern ::QEvent::Type evt_refresh_data;


void
init_event_types ( );


} // End of namespace


#endif
