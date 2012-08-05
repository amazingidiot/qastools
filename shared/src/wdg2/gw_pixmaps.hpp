//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_wdg2_gw_pixmaps_hpp__
#define __INC_wdg2_gw_pixmaps_hpp__

#include "graphical_widget.hpp"
#include "dpe2/pixmap_request.hpp"

namespace Wdg2
{


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


	::dpe2::Pixmap_Ref &
	pxmap (
		unsigned int idx_n );

	::dpe2::Pixmap_Request *
	pxm_request (
		unsigned int idx_n );


	static
	void
	pxm_request_finished_cb (
		void * context_n,
		::dpe2::Pixmap_Request * request_n );


	// Protected methods
	protected:

	virtual
	bool
	setup_request (
		::dpe2::Key_Values & kvals_n );

	void
	repaint_pixmap (
		unsigned int idx_n );

	void
	repaint_pixmaps ( );


	// Private attributes
	private:

	unsigned int _pxm_idx;
	::std::vector < ::dpe2::Pixmap_Ref > _pxmaps;
	::std::vector < ::dpe2::Pixmap_Request * > _pxm_requests;
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
::dpe2::Pixmap_Ref &
GW_Pixmaps::pxmap (
	unsigned int idx_n )
{
	return _pxmaps[idx_n];
}

inline
::dpe2::Pixmap_Request *
GW_Pixmaps::pxm_request (
	unsigned int idx_n )
{
	return _pxm_requests[idx_n];
}


} // End of namespace

#endif
