/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "slider_test.hpp"
#include <QVBoxLayout>

namespace Wdg
{

DS_Slider_Test::DS_Slider_Test ( QWidget * parent_n )
: QWidget ( parent_n )
{
  connect ( slider (),
            SIGNAL ( valueChanged ( int ) ),
            this,
            SLOT ( value_changed ( int ) ) );

  connect ( slider (),
            SIGNAL ( rangeChanged ( int, int ) ),
            this,
            SLOT ( range_changed ( int, int ) ) );

  QVBoxLayout * lay_vbox ( new QVBoxLayout () );
  lay_vbox->addWidget ( slider () );
  lay_vbox->addWidget ( &_value );
  lay_vbox->addWidget ( &_range );
  setLayout ( lay_vbox );

  value_changed ( 0 );
}

DS_Slider_Test::~DS_Slider_Test () {}

void
DS_Slider_Test::value_changed ( int value_n )
{
  //::std::cout << "DS_Slider_Test::value_changed " << value_n << "\n";
  _value.setText ( QString ( "<b>Value</b><br>\n%1" ).arg ( value_n ) );
}

void
DS_Slider_Test::range_changed ( int min_n, int max_n )
{
  //::std::cout << "DS_Slider_Test::range_changed " << min_n << " - " << max_n
  //<< "\n";
  _range.setText (
      QString ( "<b>Range</b><br>%1<br/>\n%2" ).arg ( max_n ).arg ( min_n ) );
}

} // namespace Wdg
