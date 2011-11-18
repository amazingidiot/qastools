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

#ifndef __INC_scroll_area_vertical_hpp__
#define __INC_scroll_area_vertical_hpp__

#include <QScrollArea>


namespace Wdg
{


/// @brief Scroll_Area_Vertical
///
class Scroll_Area_Vertical :
	public QScrollArea
{
	// Public methods
	public:

	Scroll_Area_Vertical (
		QWidget * parent_n = 0 );


	QSize
	minimumSizeHint ( ) const;

	void
	set_widget (
		QWidget * wdg_n );

	QWidget *
	take_widget ( );


	// Protected methods
	protected:

	bool
	eventFilter (
		QObject * watched_n,
		QEvent * event_n );
};


} // End of namespace


#endif
