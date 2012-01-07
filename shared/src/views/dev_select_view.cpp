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

#include "dev_select_view.hpp"

#include "qsnd/ctl_address.hpp"
#include "qsnd/ctl_def.hpp"
#include "qsnd/controls_model.hpp"
#include "mwdg/controls_view.hpp"
#include "mwdg/ctl_arg_view.hpp"
#include "mwdg/ctl_arg_view_string.hpp"
#include "mwdg/ctl_arg_view_integer.hpp"
#include "mwdg/ctl_arg_view_card.hpp"
#include "views/dev_select_view_setup.hpp"

#include <QLabel>
#include <QMenu>
#include <QContextMenuEvent>

#include <iostream>


namespace Views
{


Dev_Select_View::Dev_Select_View (
	QWidget * parent_n ) :
QWidget ( parent_n ),
_view_setup ( 0 ),
_lay_arg_views ( 0 ),
_current_ctl_def ( 0 ),
_silent_ctl_change ( false )
{
	// Strings
	_str_type_card = "card";
	_str_type_string = "string";
	_str_type_integer = "integer";

	// View close action
	_act_close = new QAction ( this );
	_act_close->setText ( tr ( "&Close device selection" ) );
	if ( QIcon::hasThemeIcon ( "window-close" ) ) {
		_act_close->setIcon ( QIcon::fromTheme ( "window-close" ) );
	}
	connect ( _act_close, SIGNAL ( triggered ( bool ) ),
		this, SIGNAL ( sig_close() ) );

	// Control selection model and view
	{
		_controls_model = new ::QSnd::Controls_Model ( this );
		_controls_model->set_controls_db ( &_controls_db );

		_controls_view = new ::MWdg::Controls_View ( this );
		_controls_view->setModel ( _controls_model );

		connect ( _controls_view->selectionModel(),
			SIGNAL ( currentChanged ( const QModelIndex &, const QModelIndex & ) ),
			this, SLOT ( control_changed ( const QModelIndex &, const QModelIndex & ) ) );
	}

	{
		QSizePolicy policy ( sizePolicy() );
		policy.setHorizontalPolicy ( QSizePolicy::Preferred );
		policy.setVerticalPolicy ( QSizePolicy::Expanding );
		setSizePolicy ( policy );
	}

	QLabel * controls_label = new QLabel;
	controls_label->setText ( tr ( "Mixer device" ) );
	controls_label->setToolTip ( tr ( "CTL plugin selection" ) );
	{
		QFont fnt ( controls_label->font() );
		fnt.setBold ( true );
		controls_label->setFont ( fnt );
	}

	_lay_arg_views = new QVBoxLayout;
	{
		QVBoxLayout * lay_args ( new QVBoxLayout );
		int vspace ( fontMetrics().height() * 2 / 3 );
		lay_args->setContentsMargins ( 0, vspace, 0, 0 );
		lay_args->addLayout ( _lay_arg_views, 0 );
		lay_args->addStretch ( 1 );

		QVBoxLayout * lay_vbox ( new QVBoxLayout );
		lay_vbox->addWidget ( controls_label, 0 );
		lay_vbox->addWidget ( _controls_view, 1 );
		lay_vbox->addLayout ( lay_args, 0 );
		setLayout ( lay_vbox );
	}
}


Dev_Select_View::~Dev_Select_View ( )
{
	clear_arg_views();
}


void
Dev_Select_View::set_view_setup (
	::Views::Dev_Select_View_Setup * setup_n )
{
	_view_setup = setup_n;

	if ( _view_setup != 0 ) {
		sel_db_clean();
	}
}


void
Dev_Select_View::compile_ctl_addr (
	::QSnd::CTL_Address & ctl_addr_n )
{
	//::std::cout << "Dev_Select_View::compile_ctl_addr: " << ctl_addr_n.addr_str().toLocal8Bit().data() << "\n";

	const QModelIndex & cidx ( _controls_view->currentIndex() );
	const ::QSnd::CTL_Def * ctl_def ( _controls_model->ctl_def ( cidx ) );
	if ( ctl_def != 0 ) {
		ctl_addr_n.set_ctl_name ( ctl_def->ctl_name() );

		for ( int ii=0; ii < _arg_views.size(); ++ii ) {
			const QString & arg_name ( _arg_views[ii]->ctl_arg()->arg_name );
			const QString & arg_str ( _arg_views[ii]->arg_string() );
			if ( !arg_str.isEmpty() ) {
				ctl_addr_n.append_arg (
					new ::QSnd::CTL_Address_Arg ( arg_name, arg_str ) );
			}
		}
	}
}


void
Dev_Select_View::silent_select_ctl (
	const ::QSnd::CTL_Address & ctl_addr_n )
{
	if ( _selected_ctl.match ( ctl_addr_n ) ) {
		return;
	}

	_silent_ctl_change = true;

	// Clear previous selection
	clear_arg_views();
	_current_ctl_def = 0;
	_selected_ctl.clear();
	_controls_view->clearSelection();

	// Find model index from name
	const QModelIndex midx (
		_controls_model->ctl_def_index ( ctl_addr_n.ctl_name() ) );

	// It's safer to set a current index even if it is invalid.
	// Otherwise Qt may pick the first index just to have one selected.
	_controls_view->setCurrentIndex ( midx );
	if ( midx.isValid() ) {
		_current_ctl_def = _controls_model->ctl_def ( midx );

		// The address in the datbase will be used by the arg view restore
		sel_db_commit ( new ::QSnd::CTL_Address ( ctl_addr_n ) );

		_silent_arg_change = true;
		create_arg_views();
		restore_arg_views();
		_silent_arg_change = false;

		compile_ctl_addr ( _selected_ctl );
	}

	_silent_ctl_change = false;
}


void
Dev_Select_View::reload_database ( )
{
	//::std::cout << "Dev_Select_View::reload_database" << "\n";

	if ( _view_setup != 0 ) {
		// Remember and clear selection
		::QSnd::CTL_Address ctl_addr ( _selected_ctl );
		_selected_ctl.clear();
		_current_ctl_def = 0;

		_silent_ctl_change = true;
		_controls_db.reload();
		sel_db_clean();
		_silent_ctl_change = false;

		// Restore selection
		silent_select_ctl ( ctl_addr );
	}
}


void
Dev_Select_View::control_changed (
	const QModelIndex & cur_idx_n,
	const QModelIndex & )
{
	control_changed ( cur_idx_n );
}


void
Dev_Select_View::control_changed (
	const QModelIndex & idx_n )
{
	//::std::cout << "Dev_Select_View::control_changed " << "\n";

	if ( idx_n.isValid() && !_silent_ctl_change ) {
		const ::QSnd::CTL_Def * ctl_def (
			_controls_model->ctl_def ( idx_n ) );

		if ( ctl_def != _current_ctl_def ) {
			// Clear
			clear_arg_views();
			_current_ctl_def = ctl_def;

			if ( _current_ctl_def != 0 ) {

				_silent_arg_change = true;
				create_arg_views();
				restore_arg_views();
				_silent_arg_change = false;

				update_selected_ctl();
			}
		}
	}
}


void
Dev_Select_View::control_arg_changed ( )
{
	//::std::cout << "Dev_Select_View::control_arg_changed " << "\n";

	if ( !( _silent_ctl_change || _silent_arg_change ) ) {
		if ( update_selected_ctl() ) {
			// Remember _selected_ctl for a later visit
			sel_db_commit ( new ::QSnd::CTL_Address ( _selected_ctl ) );
		}
	}
}


bool
Dev_Select_View::update_selected_ctl ( )
{
	//::std::cout << "Dev_Select_View::update_selected_ctl " << "\n";

	::QSnd::CTL_Address ctl_addr;
	compile_ctl_addr ( ctl_addr );
	if ( !_selected_ctl.match ( ctl_addr ) ) {
		_selected_ctl = ctl_addr;
		emit sig_control_changed();
		return true;
	}
	return false;
}


const ::QSnd::CTL_Address *
Dev_Select_View::sel_db_find (
	const QString & ctl_name_n ) const
{
	const ::QSnd::CTL_Address * res ( 0 );
	if ( _view_setup != 0 ) {
		for ( int ii=0; ii < _view_setup->selection_db.size(); ++ii ) {
			const ::QSnd::CTL_Address * addr (
				_view_setup->selection_db[ii] );
			if ( addr->ctl_name() == ctl_name_n ) {
				res = addr;
				break;
			}
		}
	}
	return res;
}


void
Dev_Select_View::sel_db_commit (
	const ::QSnd::CTL_Address * ctl_addr_n )
{
	if ( ctl_addr_n == 0 ) {
		return;
	}
	if ( _view_setup == 0 ) {
		return;
	}

	for ( int ii=0; ii < _view_setup->selection_db.size(); ++ii ) {
		const ::QSnd::CTL_Address * addr ( _view_setup->selection_db[ii] );
		if ( addr->ctl_name() == ctl_addr_n->ctl_name() ) {
			delete addr;
			_view_setup->selection_db[ii] = ctl_addr_n;
			return;
		}
	}
	_view_setup->selection_db.append ( ctl_addr_n );
}


void
Dev_Select_View::sel_db_clean ( )
{
	if ( _view_setup == 0 ) {
		return;
	}

	// Remove entries from selection db that don't exist in the
	// available controls database
	for ( int ii=0; ii < _view_setup->selection_db.size(); ) {
		const ::QSnd::CTL_Address * addr (
			_view_setup->selection_db[ii] );
		if ( _controls_db.find_control_def ( addr->ctl_name() ) == 0 ) {
			delete addr;
			_view_setup->selection_db.removeAt ( ii );
		} else {
			++ii;
		}
	}
}


void
Dev_Select_View::clear_arg_views ( )
{
	for ( int ii=0; ii < _arg_views.size(); ++ii ) {
		delete _arg_views[ii];
	}
	_arg_views.clear();
}


void
Dev_Select_View::create_arg_views ( )
{
	//::std::cout << "Dev_Select_View::create_arg_views"\n";
	if ( _current_ctl_def == 0 ) {
		return;
	}
	if ( _current_ctl_def->num_args() == 0 ) {
		return;
	}

	for ( unsigned int ii=0; ii < _current_ctl_def->num_args(); ++ii ) {
		const ::QSnd::CTL_Def_Arg * ctl_arg ( _current_ctl_def->arg ( ii ) );
		::MWdg::CTL_Arg_View * arg_view ( 0 );

		if ( ctl_arg->arg_name.compare ( _str_type_card, Qt::CaseInsensitive ) == 0 ) {
			arg_view = new ::MWdg::CTL_Arg_View_Card;
		} else if ( ctl_arg->arg_type.compare ( _str_type_string, Qt::CaseInsensitive ) == 0 ) {
			arg_view = new ::MWdg::CTL_Arg_View_String;
		} else if ( ctl_arg->arg_type.compare ( _str_type_integer, Qt::CaseInsensitive ) == 0 ) {
			arg_view = new ::MWdg::CTL_Arg_View_Integer;
		}

		if ( arg_view != 0 ) {
			arg_view->set_ctl_db ( &_controls_db );
			arg_view->set_ctl_arg ( ctl_arg );
			connect ( arg_view, SIGNAL ( sig_arg_changed() ),
				this, SLOT ( control_arg_changed() ) );
			_arg_views.append ( arg_view );
			_lay_arg_views->addWidget ( arg_view );
		}
	}
}


void
Dev_Select_View::restore_arg_views ( )
{
	if ( _current_ctl_def == 0 ) {
		return;
	}
	if ( _current_ctl_def->num_args() == 0 ) {
		return;
	}

	const ::QSnd::CTL_Address * ctl_addr (
		sel_db_find ( _current_ctl_def->ctl_name() ) );
	if ( ctl_addr == 0 ) {
		return;
	}
	if ( ctl_addr->num_args() > _current_ctl_def->num_args() ) {
		return;
	}
	for ( unsigned int ii=0; ii < ctl_addr->num_args(); ++ii ) {
		const ::QSnd::CTL_Address_Arg * ctl_arg ( ctl_addr->arg ( ii ) );
		::MWdg::CTL_Arg_View * view ( _arg_views[ii] );
		if ( !ctl_arg->arg_name.isEmpty() ) {
			if ( view->ctl_arg()->arg_name != ctl_arg->arg_name ) {
				continue;
			}
		}
		view->set_arg_string ( ctl_arg->arg_value );
	}
}


void
Dev_Select_View::contextMenuEvent (
	QContextMenuEvent * event_n )
{
	QMenu cmenu ( this );
	cmenu.addAction ( _act_close );
	cmenu.exec ( event_n->globalPos() );
}


} // End of namespace

