/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_label_elide_hpp__
#define __INC_label_elide_hpp__

#include <QString>
#include <QWidget>

namespace Wdg
{

class Label_Elide : public QWidget
{
  // Public methods
  public:
  Label_Elide ( QWidget * parent = 0 );

  const QString &
  text () const;

  void
  setText ( const QString txt_n );

  Qt::Alignment
  alignment () const;

  void
  setAlignment ( Qt::Alignment align_n );

  Qt::TextElideMode
  elideMode () const;

  void
  setElideMode ( Qt::TextElideMode mode_n );

  QSize
  minimumSizeHint () const;

  QSize
  sizeHint () const;

  // Protected methods
  protected:
  void
  update_text_elided ();

  void
  resizeEvent ( QResizeEvent * event_n );

  void
  paintEvent ( QPaintEvent * event_n );

  // Private attributes
  private:
  QString _text;

  QString _text_elided;

  Qt::Alignment _alignment;

  Qt::TextElideMode _elide_mode;
};

inline const QString &
Label_Elide::text () const
{
  return _text;
}

inline Qt::Alignment
Label_Elide::alignment () const
{
  return _alignment;
}

inline Qt::TextElideMode
Label_Elide::elideMode () const
{
  return _elide_mode;
}

} // namespace Wdg

#endif
