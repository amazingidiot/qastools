//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_wdg2_gw_widget_element_hpp__
#define __INC_wdg2_gw_widget_element_hpp__

#include "gw_pixmaps.hpp"

namespace Wdg2
{


/// @brief GW_Widget_Element
///
class GW_Widget_Element
{
	// Public methods
	public:

	GW_Widget_Element ( );

	virtual
	~GW_Widget_Element ( );


	virtual
	::Wdg2::GW_Widget *
	gw_widget ( ) = 0;


	const QSize &
	size ( );

	void
	set_size (
		const QSize & size_n );


	const QPoint &
	pos ( );

	void
	set_pos (
		const QPoint & pos_n );


	const ::Flags &
	state_flags ( ) const;

	void
	set_state_flags (
		unsigned int flags_n,
		bool on_n );

	void
	set_state_flags (
		unsigned int flags_n );

	void
	unset_state_flags (
		unsigned int flags_n );



	// Protected methods
	protected:

	virtual
	void
	size_changed ( );

	virtual
	void
	pos_changed ( );

	virtual
	void
	state_flags_changed ( );


	// Private attributes
	protected:

	QSize _size;
	QPoint _pos;
	::Flags _state_flags;
};

inline
const QSize &
GW_Widget_Element::size ( )
{
	return _size;
}

inline
const QPoint &
GW_Widget_Element::pos ( )
{
	return _pos;
}

inline
const ::Flags &
GW_Widget_Element::state_flags ( ) const
{
	return _state_flags;
}

inline
void
GW_Widget_Element::set_state_flags (
	unsigned int flags_n )
{
	set_state_flags ( flags_n, true );
}

inline
void
GW_Widget_Element::unset_state_flags (
	unsigned int flags_n )
{
	set_state_flags ( flags_n, false );
}


/// @brief GW_Widget_Element_Pixmaps
///
class GW_Widget_Element_Pixmaps :
	public ::Wdg2::GW_Widget_Element
{
	// Public methods
	public:

	GW_Widget_Element_Pixmaps (
		::Wdg2::Scene_Database * scene_db_n,
		unsigned int num_pixmaps_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Widget_Element_Pixmaps ( );

	::Wdg2::GW_Widget *
	gw_widget ( );

	const ::Wdg2::GW_Pixmaps *
	gw_pixmaps ( ) const;

	::Wdg2::GW_Pixmaps *
	gw_pixmaps ( );


	// Protected methods
	protected:

	void
	size_changed ( );

	void
	pos_changed ( );


	// Private attributes
	protected:

	::Wdg2::GW_Pixmaps _gw_pixmaps;
};

inline
::Wdg2::GW_Widget *
GW_Widget_Element_Pixmaps::gw_widget ( )
{
	return &_gw_pixmaps;
}

inline
const ::Wdg2::GW_Pixmaps *
GW_Widget_Element_Pixmaps::gw_pixmaps ( ) const
{
	return &_gw_pixmaps;
}

inline
::Wdg2::GW_Pixmaps *
GW_Widget_Element_Pixmaps::gw_pixmaps ( )
{
	return &_gw_pixmaps;
}


} // End of namespace

#endif
