//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#include "ctl_arg_view_card.hpp"

#include "qsnd/card_info.hpp"
#include "qsnd/cards_model.hpp"
#include "mwdg/controls_view.hpp"
#include <QLayout>


namespace MWdg
{


CTL_Arg_View_Card::CTL_Arg_View_Card (
	QWidget * parent_n ) :
::MWdg::CTL_Arg_View ( parent_n )
{
	_cards_model = new ::QSnd::Cards_Model ( this );
	_ctl_view = new ::MWdg::Controls_View ( this );
	_ctl_view->setModel ( _cards_model );

	connect ( _ctl_view->selectionModel(),
		SIGNAL ( currentChanged ( const QModelIndex &, const QModelIndex & ) ),
		this, SLOT ( view_selection_changed ( const QModelIndex &, const QModelIndex & ) ) );

	lay_content()->addWidget ( _ctl_view );
}

void
CTL_Arg_View_Card::set_arg_string (
	const QString & str_n )
{
	if ( set_arg_string_private ( str_n ) ) {
		// Update view
		{
			QModelIndex midx (
				_cards_model->card_info_index ( arg_string() ) );
			if ( midx.isValid() ) {
				_ctl_view->setCurrentIndex ( midx );
			}
		}
		emit sig_arg_changed();
	}
}

void
CTL_Arg_View_Card::view_selection_changed (
	const QModelIndex & index_to_n,
	const QModelIndex & index_from_n )
{
	(void) index_from_n;
	// Update arg string if selected index is valid
	{
		const ::QSnd::Card_Info * cinfo (
			_cards_model->card_info ( index_to_n ) );
		if ( cinfo != 0 ) {
			QString str;
			str.setNum ( cinfo->card_index() );
			set_arg_string ( str );
		}
	}
}

void
CTL_Arg_View_Card::ctl_db_changed ( )
{
	_cards_model->set_controls_db ( ctl_db() );
}


} // End of namespace
