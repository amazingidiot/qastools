//
// C++ Implementation:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "ctl_arg_view.hpp"

#include "qsnd/ctl_def_arg.hpp"
#include <QCoreApplication>


namespace MWdg
{


CTL_Arg_View::CTL_Arg_View (
	QWidget * parent_n ) :
QWidget ( parent_n ),
_ctl_db ( 0 )
{
	_wdg_title = new QLabel;
	{
		QFont fnt ( _wdg_title->font() );
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
CTL_Arg_View::set_ctl_db (
	const ::QSnd::Controls_DB * ctl_db_n )
{
	if ( _ctl_db != ctl_db_n ) {
		_ctl_db = ctl_db_n;
		this->ctl_db_changed();
	}
}


void
CTL_Arg_View::set_ctl_arg (
	const ::QSnd::CTL_Def_Arg * ctl_arg_n )
{
	if ( _ctl_arg != ctl_arg_n ) {
		_ctl_arg = ctl_arg_n;
		this->ctl_arg_changed();
	}
}


QString
CTL_Arg_View::arg_string ( ) const
{
	return QString();
}


void
CTL_Arg_View::set_arg_string (
	const QString & )
{
	// Does nothing by default
}


void
CTL_Arg_View::ctl_db_changed ( )
{
	// Does nothing by default
}


void
CTL_Arg_View::ctl_arg_changed ( )
{
	if ( _ctl_arg != 0 ) {
		// Text
		{
			const QString name_l10n ( QCoreApplication::translate (
				"ALSA::CTL_Arg_Name",
				_ctl_arg->arg_name.toLatin1().constData() ) );
			QString txt;
			//txt.append ( QChar ( 0x2192 ) );
			txt.append ( name_l10n );
			_wdg_title->setText ( txt );
		}
		// Tooltip
		{
			QString ttip ( _ctl_arg->arg_name );
			ttip.append ( " (" );
			ttip.append ( _ctl_arg->arg_type );
			ttip.append ( ")" );
			_wdg_title->setToolTip ( ttip );
		}
	}
}


} // End of namespace
