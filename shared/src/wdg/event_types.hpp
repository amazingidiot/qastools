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

#ifndef __INC_wdg_event_types_hpp__
#define __INC_wdg_event_types_hpp__

#include <QEvent>


namespace Wdg
{

extern ::QEvent::Type evt_pass_event_focus;
extern ::QEvent::Type evt_pass_event_key;


void
init_event_types ( );


} // End of namespace


#endif
