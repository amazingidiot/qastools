//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_groups_hpp__
#define __INC_wdg2_gw_groups_hpp__

#include <QGraphicsItem>
#include <QGraphicsSimpleTextItem>
#include "gw_multi_slider.hpp"
#include "gw_multi_switch.hpp"
#include "qsnd2/controls_proxies.hpp"

namespace Wdg2
{



/// @brief GW_Group2_Sizes
///
struct GW_Group2_Sizes
{
	GW_Group2_Sizes ( );

	unsigned int height;
	unsigned int channels_hgap;
	unsigned int slider_width;
	unsigned int levels_height;
	unsigned int switches_vgap;
	unsigned int switches_height;
	unsigned int label_fnt_height; /// @brief label font height
	unsigned int label_hpadi; /// @brief horizontal padding towards sliders
	unsigned int label_hpado; /// @brief horizontal padding towards outside
};

inline
GW_Group2_Sizes::GW_Group2_Sizes ( ) :
height ( 0 ),
channels_hgap ( 0 ),
slider_width ( 0 ),
levels_height ( 0 ),
switches_vgap ( 0 ),
switches_height ( 0 ),
label_fnt_height ( 16 ),
label_hpadi ( 2 ),
label_hpado ( 2 )
{
}



/// @brief GW_Group2
///
class GW_Group2 :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Group2 (
		::QSnd2::Proxies_Group2 & proxies_group_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Group2 ( );


	QRectF
	boundingRect ( ) const;

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );


	const ::Wdg2::GW_Group2_Sizes &
	sizes ( ) const;

	void
	set_sizes (
		const ::Wdg2::GW_Group2_Sizes & sizes_n );


	unsigned int
	int_width ( ) const;

	/// @brief Used to probe a sizes set
	///
	unsigned int
	int_width_probe (
		const ::Wdg2::GW_Group2_Sizes & sizes_n ) const;


	// Private methods
	private:

	::Wdg2::GW_Multi_Slider_Sizes
	gw_levels_sizes (
		const ::Wdg2::GW_Group2_Sizes & sizes_n ) const;

	::Wdg2::GW_Multi_Switch_Sizes
	gw_switches_sizes (
		const ::Wdg2::GW_Group2_Sizes & sizes_n ) const;

	void
	update_geometries ( );


	// Private attributes
	private:

	::QSnd2::Proxies_Group2 & _proxies_group;
	::Wdg2::GW_Multi_Slider * _gw_levels;
	::Wdg2::GW_Multi_Switch * _gw_switches;
	QGraphicsSimpleTextItem * _label_item;

	QString _str_label;

	::Wdg2::GW_Group2_Sizes _sizes;
	QRectF _brect;
};

inline
const ::Wdg2::GW_Group2_Sizes &
GW_Group2::sizes ( ) const
{
	return _sizes;
}



/// @brief GW_Group3_Sizes
///
struct GW_Group3_Sizes
{
	GW_Group3_Sizes ( );

	unsigned int height;
	unsigned int slider_width;
	unsigned int channels_hgap;
	unsigned int group2_hgap;
};

inline
GW_Group3_Sizes::GW_Group3_Sizes ( ) :
height ( 0 ),
slider_width ( 0 ),
channels_hgap ( 0 ),
group2_hgap ( 0 )
{
}

/// @brief GW_Group3
///
class GW_Group3 :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Group3 (
		::QSnd2::Proxies_Group3 & proxies_group_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Group3 ( );

	QRectF
	boundingRect ( ) const;

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );


	const ::Wdg2::GW_Group3_Sizes &
	sizes ( ) const;

	void
	set_sizes (
		const ::Wdg2::GW_Group3_Sizes & sizes_n );

	unsigned int
	int_width ( ) const;

	/// @brief Used to probe a sizes set
	///
	unsigned int
	int_width_probe (
		const ::Wdg2::GW_Group3_Sizes & sizes_n ) const;


	// Private methods
	private:

	::Wdg2::GW_Group2_Sizes
	gw_group2_sizes (
		const ::Wdg2::GW_Group3_Sizes & sizes_n ) const;

	void
	update_geometries ( );


	// Private attributes
	private:

	::QSnd2::Proxies_Group3 & _proxies_group;
	QList < ::Wdg2::GW_Group2 * > _gw_groups;

	::Wdg2::GW_Group3_Sizes _sizes;
	QRectF _brect;
};

inline
const ::Wdg2::GW_Group3_Sizes &
GW_Group3::sizes ( ) const
{
	return _sizes;
}



/// @brief GW_Group4_Sizes
///
struct GW_Group4_Sizes
{
	GW_Group4_Sizes ( );

	unsigned int height;
	unsigned int slider_width;
	unsigned int channels_hgap;
	unsigned int group2_hgap;
	unsigned int group3_hgap;
};

inline
GW_Group4_Sizes::GW_Group4_Sizes ( ) :
height ( 0 ),
slider_width ( 0 ),
channels_hgap ( 0 ),
group2_hgap ( 0 ),
group3_hgap ( 0 )
{
}


/// @brief GW_Group4
///
class GW_Group4 :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Group4 (
		::QSnd2::Proxies_Group4 & proxies_group_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Group4 ( );

	QRectF
	boundingRect ( ) const;

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );


	const ::Wdg2::GW_Group4_Sizes &
	sizes ( ) const;

	void
	set_sizes (
		const ::Wdg2::GW_Group4_Sizes & sizes_n );

	unsigned int
	int_width ( ) const;

	/// @brief Used to probe a sizes set
	///
	unsigned int
	int_width_probe (
		const ::Wdg2::GW_Group4_Sizes & sizes_n ) const;


	// Private methods
	private:

	::Wdg2::GW_Group3_Sizes
	gw_group3_sizes (
		const ::Wdg2::GW_Group4_Sizes & sizes_n ) const;

	void
	update_geometries ( );


	// Private attributes
	private:

	::QSnd2::Proxies_Group4 & _proxies_group;
	QList < ::Wdg2::GW_Group3 * > _gw_groups;

	GW_Group4_Sizes _sizes;
	QRectF _brect;
};


} // End of namespace

#endif
