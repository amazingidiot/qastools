/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "ctl_arg_view_card.hpp"
#include "mwdg/controls_view.hpp"
#include "qsnd/card_info.hpp"
#include "qsnd/cards_model.hpp"
#include <QLayout>
#include <iostream>

namespace MWdg
{

CTL_Arg_View_Card::CTL_Arg_View_Card ( QWidget * parent_n )
: ::MWdg::CTL_Arg_View ( parent_n )
{
  _ctl_view = new ::MWdg::Controls_View ( this );
  lay_content ()->addWidget ( _ctl_view );
}

void
CTL_Arg_View_Card::set_arg_string ( const QString & str_n )
{
  if ( set_arg_string_private ( str_n ) ) {
    // Update view
    if ( cards_model () != 0 ) {
      QModelIndex midx (
          cards_model ()->model_index_by_card_id ( arg_string () ) );
      if ( midx.isValid () ) {
        _ctl_view->setCurrentIndex ( midx );
      }
    }
    emit sig_arg_changed ();
  }
}

void
CTL_Arg_View_Card::read_view_selection ()
{
  if ( cards_model () == nullptr ) {
    return;
  }
  // Update arg string if selected index is valid
  auto cinfo =
      cards_model ()->card_info_by_model_index ( _ctl_view->currentIndex () );
  if ( !cinfo ) {
    return;
  }
  set_arg_string ( QString::number ( cinfo->index () ) );
}

void
CTL_Arg_View_Card::cards_model_changed ()
{
  if ( _ctl_view->model () != 0 ) {
    disconnect ( _ctl_view, 0, this, 0 );
  }

  _ctl_view->setModel ( cards_model () );

  if ( cards_model () != 0 ) {
    connect (
        _ctl_view->selectionModel (),
        SIGNAL ( currentChanged ( const QModelIndex &, const QModelIndex & ) ),
        this,
        SLOT ( read_view_selection () ) );

    connect ( cards_model (),
              SIGNAL ( layoutChanged () ),
              this,
              SLOT ( read_view_selection () ) );
    connect ( cards_model (),
              SIGNAL ( rowsRemoved ( const QModelIndex &, int, int ) ),
              this,
              SLOT ( read_view_selection () ) );
    connect ( cards_model (),
              SIGNAL ( rowsInserted ( const QModelIndex &, int, int ) ),
              this,
              SLOT ( read_view_selection () ) );
    connect ( cards_model (),
              SIGNAL ( rowsMoved (
                  const QModelIndex &, int, int, const QModelIndex &, int ) ),
              this,
              SLOT ( read_view_selection () ) );
  }
}

} // namespace MWdg
