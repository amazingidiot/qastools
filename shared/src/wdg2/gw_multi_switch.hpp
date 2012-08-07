//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_multi_switch_hpp__
#define __INC_wdg2_gw_multi_switch_hpp__

#include "gw_switch.hpp"
#include "qsnd2/controls_proxies.hpp"
#include <QGraphicsItem>


namespace Wdg2
{


struct GW_Multi_Switch_Sizes
{
	GW_Multi_Switch_Sizes ( );

	unsigned int area_height;
	unsigned int switch_width;
	unsigned int channels_hgap;
};

inline
GW_Multi_Switch_Sizes::GW_Multi_Switch_Sizes ( ) :
area_height ( 0 ),
switch_width ( 0 ),
channels_hgap ( 0 )
{
}


/// @brief GW_Multi_Switch
///
class GW_Multi_Switch :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_Multi_Switch (
		::QSnd2::Proxies_Group1_Switch & snd_proxies_n,
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Multi_Switch ( );


	::QSnd2::Proxies_Group1_Switch &
	proxies_grp ( );

	const ::QSnd2::Proxies_Group1_Switch &
	proxies_grp ( ) const;


	const ::Wdg2::GW_Multi_Switch_Sizes &
	sizes ( ) const;

	void
	set_sizes (
		const ::Wdg2::GW_Multi_Switch_Sizes & sizes_n );

	unsigned int
	int_width ( ) const;

	/// @brief Used to probe a sizes set
	///
	unsigned int
	int_width_probe (
		const ::Wdg2::GW_Multi_Switch_Sizes & sizes_n ) const;


	// Private methods
	private:

	void
	update_geometries ( );


	// Private attributes
	private:

	::QSnd2::Proxies_Group1_Switch & _snd_proxies;
	::Wdg2::GW_Multi_Switch_Sizes _sizes;
	QRectF _brect;

	QList < ::Wdg2::GW_Switch * > _switches;
};

inline
::QSnd2::Proxies_Group1_Switch &
GW_Multi_Switch::proxies_grp ( )
{
	return _snd_proxies;
}

inline
const ::QSnd2::Proxies_Group1_Switch &
GW_Multi_Switch::proxies_grp ( ) const
{
	return _snd_proxies;
}

inline
const ::Wdg2::GW_Multi_Switch_Sizes &
GW_Multi_Switch::sizes ( ) const
{
	return _sizes;
}


} // End of namespace

#endif
