//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_mixer_hctl_slider_status_widget_hpp__
#define __INC_mixer_hctl_slider_status_widget_hpp__

#include "mixer_hctl_int_proxy_slider.hpp"
#include "mwdg/slider_status_widget.hpp"


namespace MWdg
{


///
/// @brief Mixer_HCTL_Slider_Status_Widget
///
class Mixer_HCTL_Slider_Status_Widget :
	public Slider_Status_Widget
{
	Q_OBJECT

	// Public methods
	public:

	Mixer_HCTL_Slider_Status_Widget (
		QWidget * parent_n = 0 );

	~Mixer_HCTL_Slider_Status_Widget ( );


	Mixer_HCTL_Int_Proxy_Slider *
	proxy_slider ( ) const;



	// Element info

	QString
	elem_name ( ) const;

	bool
	elem_has_volume ( ) const;

	bool
	elem_has_dB ( ) const;


	long
	elem_volume_value ( ) const;

	void
	elem_set_volume (
		long value_n ) const;

	long
	elem_volume_min ( ) const;

	long
	elem_volume_max ( ) const;


	long
	elem_dB_value ( ) const;

	void
	elem_set_nearest_dB (
		long dB_value_n ) const;

	long
	elem_dB_min ( ) const;

	long
	elem_dB_max ( ) const;


	// Slider selection

	void
	select_slider (
		unsigned int grp_idx_n,
		unsigned int col_idx_n );


	// Protected slots
	protected slots:

	void
	proxy_destroyed ( );


	// Protected methods
	protected:

	void
	set_slider_proxy (
		Mixer_HCTL_Int_Proxy_Slider * proxy_n );


	// Private attributes
	private:

	Mixer_HCTL_Int_Proxy_Slider * _proxy_slider;
};


inline
Mixer_HCTL_Int_Proxy_Slider *
Mixer_HCTL_Slider_Status_Widget::proxy_slider ( ) const
{
	return _proxy_slider;
}


} // End of namespace


#endif
