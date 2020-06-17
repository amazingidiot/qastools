/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_text_browser_hpp__
#define __INC_text_browser_hpp__

#include <QScrollBar>
#include <QString>
#include <QTextBrowser>
#include <QTextFrame>

namespace Wdg
{

class Text_Browser : public QTextBrowser
{
  // Public methods
  public:
  Text_Browser ( QWidget * parent = 0 );

  void
  setPlainText ( const QString & txt_n );

  void
  setHtml ( const QString & txt_n );

  QSize
  sizeHint () const;

  // Protected methods
  protected:
  void
  update_size_hint ();

  // Private attributes
  private:
  int _hint_width;
  int _hint_height;
};

} // namespace Wdg

#endif
