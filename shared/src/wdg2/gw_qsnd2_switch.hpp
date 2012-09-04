//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_qsnd2_switch_hpp__
#define __INC_wdg2_gw_qsnd2_switch_hpp__

#include "gw_switch.hpp"
#include "qsnd2/controls_proxies.hpp"

namespace Wdg2
{


/// @brief GW_QSnd2_Switch
///
class GW_QSnd2_Switch :
	public ::Wdg2::GW_Switch
{
	// Public methods
	public:

	GW_QSnd2_Switch (
		::QSnd2::Proxy_Switch & switch_proxy_n,
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_QSnd2_Switch ( );


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

	::QSnd2::Proxy_Switch & _switch_proxy;
};


} // End of namespace

#endif
