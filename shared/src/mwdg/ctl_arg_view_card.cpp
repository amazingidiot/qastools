//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
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
		this, SIGNAL ( sig_arg_changed() ) );

	lay_content()->addWidget ( _ctl_view );
}


QString
CTL_Arg_View_Card::arg_string ( ) const
{
	QString res;
	const ::QSnd::Card_Info * cinfo (
		_cards_model->card_info ( _ctl_view->currentIndex() ) );
	if ( cinfo != 0 ) {
		res.setNum ( cinfo->card_index() );
	}
	return res;
}


void
CTL_Arg_View_Card::set_arg_string (
	const QString & str_n )
{
	QModelIndex midx (
		_cards_model->card_info_index ( str_n ) );
	if ( midx.isValid() ) {
		_ctl_view->setCurrentIndex ( midx );
	}
}


void
CTL_Arg_View_Card::ctl_db_changed ( )
{
	_cards_model->set_controls_db ( ctl_db() );
}


} // End of namespace
