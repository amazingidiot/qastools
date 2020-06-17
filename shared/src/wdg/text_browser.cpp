/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "text_browser.hpp"
#include <iostream>

namespace Wdg
{

Text_Browser::Text_Browser ( QWidget * parent )
: QTextBrowser ( parent )
{
}

void
Text_Browser::setPlainText ( const QString & txt_n )
{
  QTextBrowser::setPlainText ( txt_n );
  update_size_hint ();
}

void
Text_Browser::setHtml ( const QString & txt_n )
{
  QTextBrowser::setHtml ( txt_n );
  update_size_hint ();
}

QSize
Text_Browser::sizeHint () const
{
  const QSize shint ( _hint_width, _hint_height );
  return shint;
}

void
Text_Browser::update_size_hint ()
{
  const int max_width ( 800 );

  // Adjust main root frame padding
  QTextFrame * rf ( document ()->rootFrame () );
  {
    QTextFrameFormat fformat ( rf->frameFormat () );
    fformat.setPadding ( fontMetrics ().averageCharWidth () * 4 / 3 );
    rf->setFrameFormat ( fformat );
  }

  document ()->adjustSize ();

  _hint_width = document ()->size ().width () + 0.5;
  _hint_width += verticalScrollBar ()->sizeHint ().width ();
  _hint_width += 2 * frameWidth ();

  if ( _hint_width > max_width ) {
    _hint_width = max_width;
  }

  _hint_height = ( _hint_width * 2 ) / 3;

  updateGeometry ();
}

} // namespace Wdg
