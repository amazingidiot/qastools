//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_views_view_utility_hpp__
#define __INC_views_view_utility_hpp__

#include <QMainWindow>
#include <QWidget>

namespace Views
{


void
load_translators (
	QApplication * app_n );

void
load_application_icon (
	QApplication * app_n,
	const QString & fallback_n );

/// @return true on successful size determination
bool
win_default_geometry (
	QRect & rect_n );

/// @return true on successful size determination
bool
win_default_size (
	QSize & size_n );

void
resize_to_default (
	QMainWindow * mwin_n );


} // End of namespace

#endif
