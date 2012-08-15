//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "gw_pixmaps.hpp"
#include "scene_database.hpp"
#include "theme_painters.hpp"
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
_qpixmap ( 0 ),
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

	if ( _qpixmap != 0 ) {
		const QRectF pxm_rect ( 0.0, 0.0, _qpixmap->width(), _qpixmap->height() );
		painter_n->drawPixmap (
			bounding_rect(),
			*_qpixmap,
			pxm_rect );
	}
}

void
GW_Pixmaps::update_qpixmap ( )
{
	QPixmap * qpxm ( 0 );
	if ( pxm_idx() < num_pixmaps() ) {
		const ::dpe2::Pixmap_Ref & pxm_ref ( _pxmaps[_pxm_idx] );
		const ::dpe2::Pixmap * pxm ( pxm_ref.pixmap() );
		if ( pxm != 0 ) {
			qpxm = pxm->qpixmap();
		}
	}
	if ( qpxm != _qpixmap ) {
		_qpixmap = qpxm;
		update();
	}
}

void
GW_Pixmaps::set_pxm_idx (
	unsigned int idx_n )
{
	if ( _pxm_idx != idx_n ) {
		_pxm_idx = idx_n;
		update_qpixmap();
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
GW_Pixmaps::update_pxm_idx ( )
{
	// Default implementation
	set_pxm_idx ( 0 );
}

void
GW_Pixmaps::set_pxm_keys (
	const ::Wdg2::GW_Pixmaps_Keys & keys_n )
{
	if ( _pxm_keys != keys_n ) {
		_pxm_keys = keys_n;
		repaint_pixmaps();
	}
}

void
GW_Pixmaps::set_pxm_type_part (
	unsigned char type_n,
	unsigned char part_n )
{
	{
		::Wdg2::GW_Pixmaps_Keys pkeys ( pxm_keys() );
		pkeys.wdg_type = type_n;
		pkeys.wdg_part = part_n;
		set_pxm_keys ( pkeys );
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
		if ( request_n->state.has_any ( ::dpe2::RS_NEEDS_UPDATE ) ) {
			// Repaint on demand
			repaint_pixmap ( request_n->key );
		} else {
			// Return old pixmap on demand
			if ( pxm_ref.is_valid() ) {
				scene_db()->pxm_server()->return_pixmap ( pxm_ref );
			}
			// Accept new pixmap
			if ( request_n->pxm_ref.is_valid() ) {
				pxm_ref.swap ( request_n->pxm_ref );
				pxm_ref.pixmap()->convert_to_qpixmap();
				if ( pxm_idx() == request_n->key ) {
					update_qpixmap();
				}
			}
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
		// Update now
		req->state.unset ( ::dpe2::RS_NEEDS_UPDATE );
		if ( pxm_size_valid() && this->setup_request ( idx_n, req->kvals ) ) {
			scene_db()->pxm_server()->send_request ( req );
		} else {
			scene_db()->pxm_server()->return_pixmap ( _pxmaps[idx_n] );
			if ( pxm_idx() == idx_n ) {
				update_qpixmap();
			}
		}
	} else {
		// Request a later update
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
	kvals_n.set_uint ( ::Wdg2::PRK_WIDTH, pxm_size().width() );
	kvals_n.set_uint ( ::Wdg2::PRK_HEIGHT, pxm_size().height() );
	kvals_n.set_uint ( ::Wdg2::PRK_WIDGET_TYPE, pxm_keys().wdg_type );
	kvals_n.set_uint ( ::Wdg2::PRK_WIDGET_PART, pxm_keys().wdg_part );
	return true;
}


} // End of namespace
