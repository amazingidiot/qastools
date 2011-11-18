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

#include "view_base.hpp"

#include <mwdg/mixer_device_setup.hpp>
#include <views/message_widget.hpp>

#include <iostream>


namespace Views
{


View_Base::View_Base (
	QWidget * parent_n ) :
QWidget ( parent_n )
{
	_message_wdg = new ::Views::Message_Widget ( this );
	_message_wdg->hide();
	QStackedLayout * lay ( new QStackedLayout ( this ) );
	lay->addWidget ( _message_wdg );
}


View_Base::~View_Base ( )
{
}


void
View_Base::set_mdev_setup (
	const ::MWdg::Mixer_Device_Setup * setup_n )
{
	//::std::cout << "View_Base::set_mdev_setup: " << setup_n << "\n";
	_mdev_setup = setup_n;
}


void
View_Base::set_inputs_setup (
	const ::MWdg::Inputs_Setup * setup_n )
{
	//::std::cout << "View_Base::set_inputs_setup: " << setup_n << "\n";
	_inputs_setup = setup_n;
}


} // End of namespace
