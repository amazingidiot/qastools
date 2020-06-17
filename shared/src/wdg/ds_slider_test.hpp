/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_ds_slider_test_hpp__
#define __INC_ds_slider_test_hpp__

#include "namespace_wdg.hpp"
#include "qslider.hpp"
#include <QLabel>
#include <iostream>

namespace Wdg
{

class DS_Slider_Test : public QWidget
{
  Q_OBJECT

  public:
  DS_Slider_Test ( QWidget * parent_n = 0 );

  ~DS_Slider_Test ();

  DS_Slider *
  slider ();

  protected slots:

  void
  value_changed ( int value_n );

  void
  range_changed ( int min_n, int max_n );

  // Private attributes
  private:
  DS_Slider _slider;

  QLabel _value;

  QLabel _range;
};

inline DS_Slider *
DS_Slider_Test::slider ()
{
  return &_slider;
}

} // namespace Wdg

#endif
