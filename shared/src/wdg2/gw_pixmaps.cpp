//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_pixmaps.hpp"
#include <QPainter>
#include <cassert>
#include <iostream>

namespace Wdg2
{


GW_Pixmaps::GW_Pixmaps (
	::Wdg2::Scene_Database * scene_db_n,
	unsigned int num_pxmaps_n,
	QGraphicsItem * parent_n ) :
::Wdg2::GW_Widget ( scene_db_n, parent_n ),
_pxm_idx ( 0 ),
_pxmaps ( num_pxmaps_n ),
_pxm_requests ( num_pxmaps_n, 0 )
{
	for ( unsigned int ii=0; ii < _pxm_requests.size(); ++ii ) {
		::dpe2::Pixmap_Request * req (
			scene_db()->pxm_server()->acquire_request() );
		req->key = ii;
		req->cb_context = this;
		req->cb_func = &pxm_request_finished_cb;
		_pxm_requests[ii] = req;
	}
}

GW_Pixmaps::~GW_Pixmaps ( )
{
	for ( unsigned int ii=0; ii < num_pixmaps(); ++ii ) {
		scene_db()->pxm_server()->release_request ( _pxm_requests[ii] );
		_pxm_requests[ii] = 0;
	}
	for ( unsigned int ii=0; ii < num_pixmaps(); ++ii ) {
		if ( _pxmaps[ii].is_valid() ) {
			scene_db()->pxm_server()->return_pixmap ( _pxmaps[ii] );
			_pxmaps[ii].clear();
		}
	}
}

void
GW_Pixmaps::paint (
	QPainter * painter_n,
	const QStyleOptionGraphicsItem * option_n,
	QWidget * widget_n )
{
	(void) option_n;
	(void) widget_n;

	if ( pxm_idx() < num_pixmaps() ) {
		QPixmap * qpxm ( 0 );
		{
			const ::dpe2::Pixmap_Ref & pxm_ref ( _pxmaps[_pxm_idx] );
			const ::dpe2::Pixmap * pxm ( pxm_ref.pixmap() );
			if ( pxm != 0 ) {
				qpxm = pxm->qpixmap();
			}
		}
		if ( qpxm != 0 ) {
			painter_n->drawPixmap (
				bounding_rect(),
				*qpxm,
				QRectF ( 0.0, 0.0, qpxm->width(), qpxm->height() ) );
		}
	}
}

void
GW_Pixmaps::set_pxm_idx (
	unsigned int idx_n )
{
	if ( _pxm_idx != idx_n ) {
		_pxm_idx = idx_n;
		update();
	}
}

void
GW_Pixmaps::set_pxm_size (
	const QSize & size_n )
{
	if ( size_n != _pxm_size ) {
		_pxm_size = size_n;
		set_bounding_rect ( _pxm_size );
		repaint_pixmaps();
	}
}

void
GW_Pixmaps::set_state_flags (
	unsigned int flags_n,
	bool flag_n )
{
	const ::Flags ftmp ( _state_flags );
	_state_flags.set ( flags_n, flag_n );
	if ( _state_flags != ftmp ) {
		this->update_pxm_idx();
	}
}


void
GW_Pixmaps::pxm_request_finished_cb (
	void * context_n,
	::dpe2::Pixmap_Request * request_n )
{
	::Wdg2::GW_Pixmaps & cref (
		*reinterpret_cast < ::Wdg2::GW_Pixmaps * > ( context_n ) );
	cref.pxm_request_finished ( request_n );
}

void
GW_Pixmaps::pxm_request_finished (
	::dpe2::Pixmap_Request * request_n )
{
	if ( request_n->key < num_pixmaps() ) {
		::dpe2::Pixmap_Ref & pxm_ref ( _pxmaps[request_n->key] );
		// Return old pixmap on demand
		if ( pxm_ref.is_valid() ) {
			scene_db()->pxm_server()->return_pixmap ( pxm_ref );
		}
		// Accept new pixmap
		if ( request_n->pxm_ref.is_valid() ) {
			pxm_ref = request_n->pxm_ref;
			request_n->pxm_ref.clear();
			pxm_ref.pixmap()->convert_to_qpixmap();
			if ( request_n->key == pxm_idx() ) {
				update();
			}
		}

		// Repaint on demand
		if ( request_n->state.has_any ( ::dpe2::RS_NEEDS_UPDATE ) ) {
			repaint_pixmap ( request_n->key );
		}
	}
}

void
GW_Pixmaps::repaint_pixmap (
	unsigned int idx_n )
{
	assert ( idx_n < num_pixmaps() );

	::dpe2::Pixmap_Request * req ( _pxm_requests[idx_n] );
	if ( !req->state.has_any ( ::dpe2::RS_PROCESSING ) ) {
		//::std::cout << "GW_Pixmaps::repaint_pixmap " << "Sending request" << "\n";
		req->state.unset ( ::dpe2::RS_NEEDS_UPDATE );
		if ( this->setup_request ( idx_n, req->kvals ) ) {
			scene_db()->pxm_server()->send_request ( req );
		}
	} else {
		//::std::cout << "GW_Pixmaps::repaint_pixmap " << "Requesting update" << "\n";
		req->state.set ( ::dpe2::RS_NEEDS_UPDATE );
	}
}

void
GW_Pixmaps::repaint_pixmaps ( )
{
	for ( unsigned int ii=0; ii < num_pixmaps(); ++ii ) {
		repaint_pixmap ( ii );
	}
}

bool
GW_Pixmaps::setup_request (
	unsigned int idx_n,
	::dpe2::Key_Values & kvals_n )
{
	// Simple default implementation
	(void) idx_n;
	kvals_n.set_uint ( ::dpe2::PMK_WIDTH, bounding_rect().width() );
	kvals_n.set_uint ( ::dpe2::PMK_HEIGHT, bounding_rect().height() );
	return true;
}


} // End of namespace
