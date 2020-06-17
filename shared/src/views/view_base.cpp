/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "view_base.hpp"
#include "mwdg/mixer_device_setup.hpp"
#include "views/message_widget.hpp"
#include <QVBoxLayout>
#include <iostream>

namespace Views
{

View_Base::View_Base ( QWidget * parent_n )
: QWidget ( parent_n )
, _mdev_setup ( 0 )
, _inputs_setup ( 0 )
, _message_wdg ( 0 )
{
  _message_wdg = new ::Views::Message_Widget ( this );
  _message_wdg->hide ();

  _lay_stack = new QStackedLayout;
  _lay_stack->addWidget ( _message_wdg );

  QVBoxLayout * lay_vbox ( new QVBoxLayout );
  lay_vbox->addLayout ( _lay_stack );
  setLayout ( lay_vbox );
}

View_Base::~View_Base () {}

void
View_Base::set_mdev_setup ( const ::MWdg::Mixer_Device_Setup * setup_n )
{
  //::std::cout << "View_Base::set_mdev_setup: " << setup_n << "\n";
  _mdev_setup = setup_n;
}

void
View_Base::set_inputs_setup ( const ::MWdg::Inputs_Setup * setup_n )
{
  //::std::cout << "View_Base::set_inputs_setup: " << setup_n << "\n";
  _inputs_setup = setup_n;
}

} // namespace Views
