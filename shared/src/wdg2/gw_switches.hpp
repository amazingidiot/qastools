//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_switches_hpp__
#define __INC_wdg2_gw_switches_hpp__

#include <QGraphicsItem>
#include "qsnd2/controls_proxies.hpp"


namespace Wdg2
{


// Forward declaration
class GW_Switch;


struct GW_Switches_Sizes
{
	GW_Switches_Sizes ( );

	unsigned int area_height;
	unsigned int switch_width;
	unsigned int channels_hgap;
};

inline
GW_Switches_Sizes::GW_Switches_Sizes ( ) :
area_height ( 0 ),
switch_width ( 0 ),
channels_hgap ( 0 )
{
}


/// @brief GW_Switches
///
class GW_Switches :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Switches (
		::QSnd2::Proxies_Group1_Switch & snd_proxies_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Switches ( );


	::QSnd2::Proxies_Group1_Switch &
	snd_proxies ( );

	const ::QSnd2::Proxies_Group1_Switch &
	snd_proxies ( ) const;


	QRectF
	boundingRect ( ) const;

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );


	const ::Wdg2::GW_Switches_Sizes &
	sizes ( ) const;

	void
	set_sizes (
		const ::Wdg2::GW_Switches_Sizes & sizes_n );

	unsigned int
	int_width ( ) const;


	// Private methods
	private:

	void
	update_geometries ( );


	// Private attributes
	private:

	::QSnd2::Proxies_Group1_Switch & _snd_proxies;
	::Wdg2::GW_Switches_Sizes _sizes;

	QList < ::Wdg2::GW_Switch * > _switches;
};

inline
::QSnd2::Proxies_Group1_Switch &
GW_Switches::snd_proxies ( )
{
	return _snd_proxies;
}

inline
const ::QSnd2::Proxies_Group1_Switch &
GW_Switches::snd_proxies ( ) const
{
	return _snd_proxies;
}

inline
const ::Wdg2::GW_Switches_Sizes &
GW_Switches::sizes ( ) const
{
	return _sizes;
}



/// @brief GW_Switch
///
class GW_Switch :
	public QGraphicsItem
{
	// Public methods
	public:

	GW_Switch (
		QGraphicsItem * parent_n = 0 );

	~GW_Switch ( );


	QRectF
	boundingRect ( ) const;

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );


	const QSize &
	switch_size ( ) const;

	void
	set_switch_size (
		const QSize & size_n );


	// Private methods
	private:

	// Private attributes
	private:

	QSize _switch_size;
};

inline
const QSize &
GW_Switch::switch_size ( ) const
{
	return _switch_size;
}


} // End of namespace

#endif
