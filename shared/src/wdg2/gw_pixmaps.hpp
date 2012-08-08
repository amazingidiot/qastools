//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_pixmaps_hpp__
#define __INC_wdg2_gw_pixmaps_hpp__

#include "gw_widget.hpp"
#include "dpe2/pixmap_request.hpp"

namespace Wdg2
{


struct GW_Pixmaps_Keys
{
	GW_Pixmaps_Keys ( );

	unsigned char wdg_type;
	unsigned char wdg_part;

	bool
	operator == (
		const ::Wdg2::GW_Pixmaps_Keys & keys_n );

	bool
	operator != (
		const ::Wdg2::GW_Pixmaps_Keys & keys_n );
};

inline
GW_Pixmaps_Keys::GW_Pixmaps_Keys ( ) :
wdg_type ( 0 ),
wdg_part ( 0 )
{
}

inline
bool
GW_Pixmaps_Keys::operator == (
	const ::Wdg2::GW_Pixmaps_Keys & keys_n )
{
	return ( wdg_type == keys_n.wdg_type ) &&
		( wdg_part == keys_n.wdg_part );
}

inline
bool
GW_Pixmaps_Keys::operator != (
	const ::Wdg2::GW_Pixmaps_Keys & keys_n )
{
	return !operator== ( keys_n );
}


/// @brief GW_Pixmaps
///
class GW_Pixmaps :
	public ::Wdg2::GW_Widget
{
	// Public methods
	public:

	GW_Pixmaps (
		::Wdg2::Scene_Database * scene_db_n,
		unsigned int num_pxmaps_n,
		QGraphicsItem * parent_n = 0 );

	~GW_Pixmaps ( );

	void
	paint (
		QPainter * painter_n,
		const QStyleOptionGraphicsItem * option_n,
		QWidget * widget_n = 0 );


	unsigned int
	num_pixmaps ( ) const;

	/// @brief Current pixmap index
	unsigned int
	pxm_idx ( ) const;

	void
	set_pxm_idx (
		unsigned int idx_n );


	const QSize &
	pxm_size ( ) const;

	void
	set_pxm_size (
		const QSize & size_n );

	bool
	pxm_size_valid ( ) const;


	const ::Flags &
	state_flags ( ) const;

	void
	set_state_flags (
		unsigned int flags_n,
		bool flag_n = true );


	void
	repaint_pixmap (
		unsigned int idx_n );

	void
	repaint_pixmaps ( );

	const ::Wdg2::GW_Pixmaps_Keys &
	pxm_keys ( ) const;

	void
	set_pxm_keys (
		const ::Wdg2::GW_Pixmaps_Keys & keys_n );

	void
	set_pxm_type_part (
		unsigned char type_n,
		unsigned char part_n );


	static
	void
	pxm_request_finished_cb (
		void * context_n,
		::dpe2::Pixmap_Request * request_n );

	void
	pxm_request_finished (
		::dpe2::Pixmap_Request * request_n );


	// Protected methods
	protected:

	virtual
	void
	update_pxm_idx ( );

	virtual
	bool
	setup_request (
		unsigned int idx_n,
		::dpe2::Key_Values & kvals_n );


	// Private attributes
	private:

	typedef ::std::vector < ::dpe2::Pixmap_Ref > Pixmap_List;
	typedef ::std::vector < ::dpe2::Pixmap_Request * > Pixmap_Req_List;

	unsigned int _pxm_idx;
	Pixmap_List _pxmaps;
	Pixmap_Req_List _pxm_requests;
	::Flags _state_flags;
	QSize _pxm_size;
	::Wdg2::GW_Pixmaps_Keys _pxm_keys;
};

inline
unsigned int
GW_Pixmaps::num_pixmaps ( ) const
{
	return _pxm_requests.size();
}

inline
unsigned int
GW_Pixmaps::pxm_idx ( ) const
{
	return _pxm_idx;
}

inline
const QSize &
GW_Pixmaps::pxm_size ( ) const
{
	return _pxm_size;
}

inline
bool
GW_Pixmaps::pxm_size_valid ( ) const
{
	return ( ( pxm_size().width() >= 0 ) && (  pxm_size().height() >= 0 ) );
}

inline
const ::Wdg2::GW_Pixmaps_Keys &
GW_Pixmaps::pxm_keys ( ) const
{
	return _pxm_keys;
}

inline
const ::Flags &
GW_Pixmaps::state_flags ( ) const
{
	return _state_flags;
}

} // End of namespace

#endif
