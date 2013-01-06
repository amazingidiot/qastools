//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_wdg2_gw_switch_hpp__
#define __INC_wdg2_gw_switch_hpp__

#include "flags.hpp"
#include "callbacks.hpp"
#include "gw_widget.hpp"
#include "gw_pixmaps.hpp"
#include <QScopedPointer>

// Forward declaration
namespace Wdg2
{
	class GW_Switch_Ground;
	class GW_Switch_Handle;
};


namespace Wdg2
{


/// @brief GW_Switch
///
class GW_Switch :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_Switch (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Switch ( );

	void
	set_size (
		const QSize & size_n );

	bool
	switch_state ( ) const;

	void
	set_switch_state (
		bool state_n );

	void
	toggle_switch_state ( );


	/// @brief Gets called when the switch state changed
	///
	const ::Context_Callback &
	val_change_callback ( ) const;

	void
	set_val_change_callback (
		const ::Context_Callback & cb_n );


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

	QScopedPointer < ::Wdg2::GW_Switch_Ground > _ground;
	QScopedPointer < ::Wdg2::GW_Switch_Handle > _handle;

	::Flags _state_flags;
	::Context_Callback _val_change_cb;
};

inline
bool
GW_Switch::switch_state ( ) const
{
	return _state_flags.has_any ( ::Wdg2::GW_IS_ON );
}

inline
const ::Context_Callback &
GW_Switch::val_change_callback ( ) const
{
	return _val_change_cb;
}


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
};


} // End of namespace

#endif
