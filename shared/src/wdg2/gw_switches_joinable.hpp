//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_wdg2_gw_switches_joinable_hpp__
#define __INC_wdg2_gw_switches_joinable_hpp__

#include "gw_switch.hpp"

// Forward declaration
namespace Wdg2
{
	class GW_Switch;
	class GW_Switch_Multi;
}

namespace Wdg2
{


struct GW_Switches_Joinable_Settings
{
	GW_Switches_Joinable_Settings ( );

	unsigned int area_height;
	unsigned int switch_width;
	unsigned int channels_gap;
};

inline
GW_Switches_Joinable_Settings::GW_Switches_Joinable_Settings ( ) :
area_height ( 0 ),
switch_width ( 0 ),
channels_gap ( 0 )
{
}


/// @brief GW_Switches_Joinable
///
class GW_Switches_Joinable :
	public ::Wdg2::GW_Widget
{
	// Public typedefs
	public:

	enum State_Flags {
		SF_NULL = 0,
		SF_SEPARATE = ( 1 << 0 ),
		SF_JOINED   = ( 1 << 1 )
	};


	// Public methods
	public:

	GW_Switches_Joinable (
		::Wdg2::Scene_Database * scene_db_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Switches_Joinable ( );


	unsigned int
	num_switches ( ) const;

	void
	set_num_switches (
		unsigned int num_n );


	const ::Wdg2::GW_Switches_Joinable_Settings &
	settings ( ) const;

	void
	load_settings (
		const ::Wdg2::GW_Switches_Joinable_Settings & settings_n );

	unsigned int
	int_width ( ) const;

	/// @brief Used to probe a sizes set
	///
	unsigned int
	int_width_probe (
		const ::Wdg2::GW_Switches_Joinable_Settings & settings_n ) const;


	bool
	is_separate ( ) const;

	bool
	is_joined ( ) const;

	void
	select_separate ( );

	void
	select_joined ( );

	void
	select_joined (
		bool flag_n );

	void
	toggle_joined ( );


	bool
	has_focus ( );

	void
	set_focus (
		Qt::FocusReason focusReason_n = Qt::OtherFocusReason );


	// Protected methods
	protected:

	bool
	sceneEventFilter (
		QGraphicsItem * watched_n,
		QEvent * event_n );


	// Private methods
	private:

	void
	update_geometries ( );

	void
	destroy_switches ( );

	void
	init_switches ( );

	virtual
	::Wdg2::GW_Switch *
	create_switch_single (
		unsigned int idx_n ) = 0;

	virtual
	::Wdg2::GW_Switch_Multi *
	create_switch_multi ( ) = 0;


	// Private attributes
	private:

	QList < ::Wdg2::GW_Switch * > _switches;
	::Wdg2::GW_Switch_Multi * _switch_multi;

	unsigned int _num_switches;
	::Flags _state_flags;
	::Wdg2::GW_Switches_Joinable_Settings _settings;
};

inline
unsigned int
GW_Switches_Joinable::num_switches ( ) const
{
	return _num_switches;
}

inline
const ::Wdg2::GW_Switches_Joinable_Settings &
GW_Switches_Joinable::settings ( ) const
{
	return _settings;
}

inline
bool
GW_Switches_Joinable::is_separate ( ) const
{
	return _state_flags.has_any ( SF_SEPARATE );
}

inline
bool
GW_Switches_Joinable::is_joined ( ) const
{
	return _state_flags.has_any ( SF_JOINED );
}


} // End of namespace

#endif
