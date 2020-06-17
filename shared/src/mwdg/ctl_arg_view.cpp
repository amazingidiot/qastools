/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "ctl_arg_view.hpp"
#include "qsnd/ctl_format_argument.hpp"
#include <QCoreApplication>

namespace MWdg
{

CTL_Arg_View::CTL_Arg_View ( QWidget * parent_n )
: QWidget ( parent_n )
, _ctl_db ( 0 )
, _cards_model ( 0 )
, _ctl_arg ( 0 )
{
  _wdg_title = new QLabel;
  {
    QFont fnt ( _wdg_title->font () );
    fnt.setBold ( true );
    _wdg_title->setFont ( fnt );
  }

  QVBoxLayout * lay_vbox ( new QVBoxLayout );
  lay_vbox->setContentsMargins ( 0, 0, 0, 0 );
  _lay_content = new QVBoxLayout;
  lay_vbox->addWidget ( _wdg_title, 0 );
  lay_vbox->addLayout ( _lay_content, 1 );
  setLayout ( lay_vbox );
}

void
CTL_Arg_View::set_ctl_db ( const ::QSnd::Controls_Database * ctl_db_n )
{
  if ( _ctl_db != ctl_db_n ) {
    _ctl_db = ctl_db_n;
    this->ctl_db_changed ();
  }
}

void
CTL_Arg_View::set_cards_model ( ::QSnd::Cards_Model * cards_model_n )
{
  if ( _cards_model != cards_model_n ) {
    _cards_model = cards_model_n;
    this->cards_model_changed ();
  }
}

void
CTL_Arg_View::set_ctl_arg ( const ::QSnd::CTL_Format_Argument & ctl_arg_n )
{
  if ( _ctl_arg != ctl_arg_n ) {
    _ctl_arg = ctl_arg_n;
    this->ctl_arg_changed ();
  }
}

void
CTL_Arg_View::set_arg_string ( const QString & str_n )
{
  if ( _arg_string != str_n ) {
    _arg_string = str_n;
    emit sig_arg_changed ();
  }
}

bool
CTL_Arg_View::set_arg_string_private ( const QString & str_n )
{
  if ( _arg_string != str_n ) {
    _arg_string = str_n;
    return true;
  }
  return false;
}

void
CTL_Arg_View::ctl_db_changed ()
{
  // Does nothing by default
}

void
CTL_Arg_View::cards_model_changed ()
{
  // Does nothing by default
}

void
CTL_Arg_View::ctl_arg_changed ()
{
  if ( _ctl_arg.is_valid () ) {
    // Text
    {
      const QString name_l10n ( QCoreApplication::translate (
          "ALSA::CTL_Arg_Name", _ctl_arg.arg_name.toUtf8 ().constData () ) );
      QString txt;
      // txt.append ( QChar ( 0x2192 ) );
      txt.append ( name_l10n );
      _wdg_title->setText ( txt );
    }
    // Tooltip
    {
      QString ttip ( _ctl_arg.arg_name );
      ttip.append ( " (" );
      ttip.append ( _ctl_arg.arg_type );
      ttip.append ( ")" );
      _wdg_title->setToolTip ( ttip );
    }
  }
}

} // namespace MWdg
