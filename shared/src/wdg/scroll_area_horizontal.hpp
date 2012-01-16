//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_scroll_area_horizontal_hpp__
#define __INC_scroll_area_horizontal_hpp__

#include <QScrollArea>


namespace Wdg
{


/// @brief Scroll_Area_Horizontal
///
class Scroll_Area_Horizontal :
	public QScrollArea
{
	// Public methods
	public:

	Scroll_Area_Horizontal (
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
