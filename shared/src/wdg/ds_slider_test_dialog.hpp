/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.


#ifndef __INC_ds_slider_test_dialog_hpp__
#define __INC_ds_slider_test_dialog_hpp__

#include <QLabel>
#include <QDialog>

#include <iostream>

#include "namespace_wdg.hpp"
#include "slider_test.hpp"
#include "mixer_style.hpp"


namespace Wdg
{


class DS_Slider_Test_Dialog :
	public QDialog
{
	Q_OBJECT

	public:

	DS_Slider_Test_Dialog (
		QWidget * parent_n = 0 );

	~DS_Slider_Test_Dialog ( );

	QWidget *
	new_vline ( );

	// Protected slots
	protected slots:


	// Private attributes
	private:

	Mixer_Style _widget_hub;
};


} // End of namespace


#endif
