//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_qsnd2_switch_multi_hpp__
#define __INC_wdg2_gw_qsnd2_switch_multi_hpp__

#include "wdg2/gw_switch_multi.hpp"
#include "qsnd2/controls_proxies.hpp"

namespace Wdg2
{


/// @brief GW_QSnd2_Switch_Multi
///
class GW_QSnd2_Switch_Multi :
	public ::Wdg2::GW_Switch_Multi
{
	// Public methods
	public:

	GW_QSnd2_Switch_Multi (
		::QSnd2::Proxies_Group1_Switch & proxies_group_n,
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_QSnd2_Switch_Multi ( );


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

	::QSnd2::Proxies_Group1_Switch & _proxies_group;
};


} // End of namespace

#endif
