//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_switch_hpp__
#define __INC_wdg2_gw_switch_hpp__

#include "flags.hpp"
#include "gw_widget.hpp"
#include "gw_pixmaps.hpp"
#include "qsnd2/controls_proxies.hpp"


namespace Wdg2
{


/// @brief GW_Switch_Ground
///
class GW_Switch_Ground :
	public ::Wdg2::GW_Pixmaps
{
	// Public methods
	public:

	GW_Switch_Ground (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );


	// Protected methods
	protected:

	void
	update_pxm_idx ( );

	bool
	setup_pxm_request (
		unsigned int idx_n,
		::dpe2::Key_Values & kvals_n );
};


/// @brief GW_Switch_Handle
///
class GW_Switch_Handle :
	public ::Wdg2::GW_Pixmaps
{
	// Public methods
	public:

	GW_Switch_Handle (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );


	// Protected methods
	protected:

	void
	update_pxm_idx ( );

	bool
	setup_pxm_request (
		unsigned int idx_n,
		::dpe2::Key_Values & kvals_n );
};


/// @brief GW_Switch
///
class GW_Switch :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_Switch (
		::QSnd2::Proxy_Switch & switch_proxy_n,
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Switch ( );


	const QSize &
	switch_size ( ) const;

	void
	set_switch_size (
		const QSize & size_n );


	void
	read_proxy_value ( );

	/// @brief Callback version
	static
	void
	read_proxy_value_cb (
		void * context_n );


	// Protected methods
	protected:

	void
	focusInEvent (
		QFocusEvent * event_n );

	void
	focusOutEvent (
		QFocusEvent * event_n );

	void
	mousePressEvent (
		QGraphicsSceneMouseEvent * event_n );

	void
	mouseReleaseEvent (
		QGraphicsSceneMouseEvent * event_n );


	// Private attributes
	private:

	::QSnd2::Proxy_Switch & _switch_proxy;
	QSize _switch_size;

	::Flags _state_flags;

	::Wdg2::GW_Switch_Ground _ground;
	::Wdg2::GW_Switch_Handle _handle;
};


inline
const QSize &
GW_Switch::switch_size ( ) const
{
	return _switch_size;
}


} // End of namespace

#endif
