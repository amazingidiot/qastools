//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
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
