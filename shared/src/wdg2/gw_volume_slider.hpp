//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_volume_slider_hpp__
#define __INC_wdg2_gw_volume_slider_hpp__

#include "gw_slider.hpp"
#include "qsnd2/controls_proxies.hpp"

namespace Wdg2
{


/// @brief GW_Volume_Slider
///
class GW_Volume_Slider :
	public ::Wdg2::GW_Slider
{
	// Public methods
	public:

	GW_Volume_Slider (
		::QSnd2::Proxy_Slider & slider_proxy_n,
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Volume_Slider ( );


	void
	read_value_from_proxy ( );

	void
	write_value_to_proxy ( ) const;


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
