/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "ds_slider_test_dialog.hpp"
#include <QVBoxLayout>
#include <climits>

namespace Wdg
{

DS_Slider_Test_Dialog::DS_Slider_Test_Dialog ( QWidget * parent_n )
: QDialog ( parent_n )
{
  const int num_blocks = 7;
  const int num_sliders = 3 * num_blocks;
  Slider_Test * sliders[ num_sliders ];

  for ( int ii = 0; ii < num_sliders; ++ii ) {
    sliders[ ii ] = new Slider_Test ( this );
    sliders[ ii ]->slider ()->set_px_buffers ( _widget_hub.slider_buffers () );
  }

  int int_max ( INT_MAX );
  int int_min ( INT_MIN );
  int sli ( 0 );
  sliders[ sli++ ]->slider ()->setRange ( 0, 0 );
  sliders[ sli++ ]->slider ()->setRange ( 1, 1 );
  sliders[ sli++ ]->slider ()->setRange ( int_max, int_max );

  sliders[ sli++ ]->slider ()->setRange ( 0, 1 );
  sliders[ sli++ ]->slider ()->setRange ( int_max - 1, int_max );
  sliders[ sli++ ]->slider ()->setRange ( -1, 0 );

  sliders[ sli++ ]->slider ()->setRange ( 0, 5 );
  sliders[ sli++ ]->slider ()->setRange ( 1, 6 );
  sliders[ sli++ ]->slider ()->setRange ( -5, 0 );

  sliders[ sli++ ]->slider ()->setRange ( 0, 31 );
  sliders[ sli++ ]->slider ()->setRange ( 1, 32 );
  sliders[ sli++ ]->slider ()->setRange ( int_max - 31, int_max );

  sliders[ sli++ ]->slider ()->setRange ( 0, 127 );
  sliders[ sli++ ]->slider ()->setRange ( 1, 128 );
  sliders[ sli++ ]->slider ()->setRange ( int_max - 127, int_max );

  sliders[ sli++ ]->slider ()->setRange ( -127, 0 );
  sliders[ sli++ ]->slider ()->setRange ( -127, -16 );
  sliders[ sli++ ]->slider ()->setRange ( -256, 256 );

  sliders[ sli++ ]->slider ()->setRange ( 0, int_max );
  sliders[ sli++ ]->slider ()->setRange ( int_min, 0 );
  sliders[ sli++ ]->slider ()->setRange ( int_min, int_max );

  QHBoxLayout * lay_hbox ( new QHBoxLayout () );

  sli = 0;

  for ( int jj = 0; jj < num_blocks; ++jj ) {

    if ( jj > 0 ) {
      lay_hbox->addWidget ( new_vline () );
    }

    for ( int ii = 0; ii < 3; ++ii ) {
      lay_hbox->addWidget ( sliders[ sli ] );
      ++sli;
    }
  }

  lay_hbox->addStretch ();
  setLayout ( lay_hbox );
}

QWidget *
DS_Slider_Test_Dialog::new_vline ()
{
  QFrame * fr ( new QFrame ( this ) );
  fr->setFrameStyle ( QFrame::VLine | QFrame::Plain );
  return fr;
}

DS_Slider_Test_Dialog::~DS_Slider_Test_Dialog () {}

} // namespace Wdg
