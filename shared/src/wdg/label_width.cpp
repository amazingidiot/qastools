/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "label_width.hpp"

namespace Wdg
{

Label_Width::Label_Width ( QWidget * parent_n )
: QLabel ( parent_n )
{
}

QSize
Label_Width::minimumSizeHint () const
{
  QSize res ( QLabel::minimumSizeHint () );

  int w_min ( fontMetrics ().width ( _min_text ) );
  res.setWidth ( qMax ( res.width (), w_min ) );

  return res;
}

QSize
Label_Width::sizeHint () const
{
  QSize res ( QLabel::sizeHint () );

  int w_min ( fontMetrics ().width ( _min_text ) );
  {
    QMargins mg ( contentsMargins () );
    w_min += mg.left ();
    w_min += mg.right ();
  }

  res.setWidth ( qMax ( res.width (), w_min ) );

  return res;
}

void
Label_Width::set_min_text ( const QString txt_n )
{
  if ( txt_n != _min_text ) {
    _min_text = txt_n;
    updateGeometry ();
    adjustSize ();
  }
}

} // namespace Wdg
