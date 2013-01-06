//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_wdg2_gw_pixmaps_hpp__
#define __INC_wdg2_gw_pixmaps_hpp__

#include "gw_widget.hpp"
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


	/// @brief Number of pixmaps / pixmap indices
	///
	unsigned int
	num_pixmaps ( ) const;

	/// @brief Current pixmap index
	///
	unsigned int
	pxm_idx ( ) const;

	/// @brief Change the current pixmap index
	///
	void
	set_pxm_idx (
		unsigned int idx_n );


	void
	set_size (
		const QSize & size_n );

	bool
	size_valid ( ) const;


	const ::Flags &
	state_flags ( ) const;

	void
	set_state_flags (
		unsigned int flags_n,
		bool flag_n = true );


	const ::dpe2::Key_Values &
	pxm_kvals ( ) const;

	::dpe2::Key_Values &
	pxm_kvals ( );


	void
	repaint_pixmap (
		unsigned int idx_n );

	void
	repaint_pixmaps ( );


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

	/// @brief Returns true if the new QPixmap isn't the old one
	void
	update_qpixmap ( );

	virtual
	void
	update_pxm_idx ( );

	virtual
	bool
	setup_pxm_request (
		unsigned int idx_n,
		::dpe2::Key_Values & kvals_n );


	// Private attributes
	private:

	typedef ::std::vector < ::dpe2::Pixmap_Ref > Pixmap_List;
	typedef ::std::vector < ::dpe2::Pixmap_Request * > Pixmap_Req_List;

	QPixmap * _qpixmap;
	unsigned int _pxm_idx;
	Pixmap_List _pxmaps;
	Pixmap_Req_List _pxm_requests;

	::Flags _state_flags;
	::dpe2::Key_Values _pxm_kvals;
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
bool
GW_Pixmaps::size_valid ( ) const
{
	return ( ( size().width() > 0 ) && ( size().height() > 0 ) );
}

inline
const ::Flags &
GW_Pixmaps::state_flags ( ) const
{
	return _state_flags;
}

inline
const ::dpe2::Key_Values &
GW_Pixmaps::pxm_kvals ( ) const
{
	return _pxm_kvals;
}

inline
::dpe2::Key_Values &
GW_Pixmaps::pxm_kvals ( )
{
	return _pxm_kvals;
}


} // End of namespace

#endif
