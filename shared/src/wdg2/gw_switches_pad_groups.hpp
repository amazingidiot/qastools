//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_switches_pad_groups_hpp__
#define __INC_wdg2_gw_switches_pad_groups_hpp__

#include "gw_widget.hpp"
#include "gw_qsnd2_switches_joinable.hpp"
#include "gw_label.hpp"
#include "qsnd2/controls_proxies.hpp"

namespace Wdg2
{


/// @brief GW_SwPad_Group2_Sizes
///
struct GW_SwPad_Group2_Sizes
{
	GW_SwPad_Group2_Sizes ( );

	unsigned int width;
	unsigned int height;
	unsigned int label_gap;
	unsigned int channels_gap;
	unsigned int switch_height;
};

inline
GW_SwPad_Group2_Sizes::GW_SwPad_Group2_Sizes ( ) :
width ( 0 ),
height ( 0 ),
label_gap ( 0 ),
channels_gap ( 0 ),
switch_height ( 0 )
{
}



/// @brief GW_SwPad_Group2
///
class GW_SwPad_Group2 :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_SwPad_Group2 (
		::QSnd2::Proxies_Group2 & proxies_group_n,
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_SwPad_Group2 ( );


	const ::Wdg2::GW_SwPad_Group2_Sizes &
	sizes ( ) const;

	void
	set_sizes (
		const ::Wdg2::GW_SwPad_Group2_Sizes & sizes_n );


	unsigned int
	int_width ( ) const;

	/// @brief Used to probe a sizes set
	///
	unsigned int
	int_width_probe (
		const ::Wdg2::GW_SwPad_Group2_Sizes & sizes_n ) const;


	// Private methods
	private:

	::Wdg2::GW_Switches_Joinable_Settings
	gw_switches_settings (
		const ::Wdg2::GW_SwPad_Group2_Sizes & sizes_n ) const;

	void
	update_geometries ( );


	// Private attributes
	private:

	::QSnd2::Proxies_Group2 & _proxies_group;
	::Wdg2::GW_Switches_Joinable * _gw_switches;
	::Wdg2::GW_Label * _label_item;

	QString _str_label;

	::Wdg2::GW_SwPad_Group2_Sizes _sizes;
};

inline
const ::Wdg2::GW_SwPad_Group2_Sizes &
GW_SwPad_Group2::sizes ( ) const
{
	return _sizes;
}



/// @brief GW_SwPad_Group3_Sizes
///
struct GW_SwPad_Group3_Sizes
{
	GW_SwPad_Group3_Sizes ( );

	unsigned int width;
	unsigned int height;
	unsigned int channels_gap;
	unsigned int group2_gap;
};

inline
GW_SwPad_Group3_Sizes::GW_SwPad_Group3_Sizes ( ) :
width ( 0 ),
height ( 0 ),
channels_gap ( 0 ),
group2_gap ( 0 )
{
}

/// @brief GW_SwPad_Group3
///
class GW_SwPad_Group3 :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_SwPad_Group3 (
		::QSnd2::Proxies_Group3 & proxies_group_n,
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_SwPad_Group3 ( );


	const ::Wdg2::GW_SwPad_Group3_Sizes &
	sizes ( ) const;

	void
	set_sizes (
		const ::Wdg2::GW_SwPad_Group3_Sizes & sizes_n );

	unsigned int
	int_width ( ) const;

	/// @brief Used to probe a sizes set
	///
	unsigned int
	int_width_probe (
		const ::Wdg2::GW_SwPad_Group3_Sizes & sizes_n ) const;


	// Private methods
	private:

	::Wdg2::GW_SwPad_Group2_Sizes
	gw_group2_sizes (
		const ::Wdg2::GW_SwPad_Group3_Sizes & sizes_n ) const;

	void
	update_geometries ( );


	// Private attributes
	private:

	::QSnd2::Proxies_Group3 & _proxies_group;
	QList < ::Wdg2::GW_SwPad_Group2 * > _gw_groups;

	::Wdg2::GW_SwPad_Group3_Sizes _sizes;
};

inline
const ::Wdg2::GW_SwPad_Group3_Sizes &
GW_SwPad_Group3::sizes ( ) const
{
	return _sizes;
}



/// @brief GW_SwPad_Group4_Sizes
///
struct GW_SwPad_Group4_Sizes
{
	GW_SwPad_Group4_Sizes ( );

	unsigned int width;
	unsigned int height;
	unsigned int channels_gap;
	unsigned int group2_gap;
	unsigned int group3_gap;
};

inline
GW_SwPad_Group4_Sizes::GW_SwPad_Group4_Sizes ( ) :
width ( 0 ),
height ( 0 ),
channels_gap ( 0 ),
group2_gap ( 0 ),
group3_gap ( 0 )
{
}


/// @brief GW_SwPad_Group4
///
class GW_SwPad_Group4 :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_SwPad_Group4 (
		::QSnd2::Proxies_Group4 & proxies_group_n,
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_SwPad_Group4 ( );


	const ::Wdg2::GW_SwPad_Group4_Sizes &
	sizes ( ) const;

	void
	set_sizes (
		const ::Wdg2::GW_SwPad_Group4_Sizes & sizes_n );

	unsigned int
	int_width ( ) const;

	/// @brief Used to probe a sizes set
	///
	unsigned int
	int_width_probe (
		const ::Wdg2::GW_SwPad_Group4_Sizes & sizes_n ) const;


	// Private methods
	private:

	::Wdg2::GW_SwPad_Group3_Sizes
	gw_group3_sizes (
		const ::Wdg2::GW_SwPad_Group4_Sizes & sizes_n ) const;

	void
	update_geometries ( );


	// Private attributes
	private:

	::QSnd2::Proxies_Group4 & _proxies_group;
	QList < ::Wdg2::GW_SwPad_Group3 * > _gw_groups;

	GW_SwPad_Group4_Sizes _sizes;
};


} // End of namespace

#endif
