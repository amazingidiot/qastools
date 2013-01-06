//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_wdg2_gw_qsnd2_slider_hpp__
#define __INC_wdg2_gw_qsnd2_slider_hpp__

#include "gw_slider.hpp"
#include "qsnd2/controls_proxies.hpp"

namespace Wdg2
{


/// @brief GW_QSnd2_Slider
///
class GW_QSnd2_Slider :
	public ::Wdg2::GW_Slider
{
	// Public methods
	public:

	GW_QSnd2_Slider (
		::QSnd2::Proxy_Slider & slider_proxy_n,
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_QSnd2_Slider ( );


	void
	read_value_from_proxy ( );

	void
	write_value_to_proxy ( );


	static
	void
	read_value_from_proxy_cb (
		void * context_n );

	static
	void
	write_value_to_proxy_cb (
		void * context_n );


	// Private attributes
	private:

	::QSnd2::Proxy_Slider & _slider_proxy;
};


} // End of namespace

#endif
