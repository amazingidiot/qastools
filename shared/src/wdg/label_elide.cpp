/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "label_elide.hpp"
#include <QPainter>

namespace Wdg
{

Label_Elide::Label_Elide ( QWidget * parent_n )
: QWidget ( parent_n )
, _alignment ( Qt::AlignLeft | Qt::AlignVCenter )
, _elide_mode ( Qt::ElideMiddle )
{
}

QSize
Label_Elide::minimumSizeHint () const
{
  return QSize ( 0, fontMetrics ().height () );
}

QSize
Label_Elide::sizeHint () const
{
  QSize res ( fontMetrics ().width ( text () ), fontMetrics ().height () );
  return res;
}

void
Label_Elide::setText ( const QString txt_n )
{
  if ( txt_n != _text ) {
    _text = txt_n;
    update_text_elided ();
    updateGeometry ();
    update ();
  }
}

void
Label_Elide::setAlignment ( Qt::Alignment align_n )
{
  if ( align_n != _alignment ) {
    _alignment = align_n;
    update_text_elided ();
    update ();
  }
}

void
Label_Elide::setElideMode ( Qt::TextElideMode mode_n )
{
  if ( mode_n != _elide_mode ) {
    _elide_mode = mode_n;
    update_text_elided ();
    update ();
  }
}

void
Label_Elide::update_text_elided ()
{
  _text_elided = fontMetrics ().elidedText ( _text, _elide_mode, width () );
}

void
Label_Elide::resizeEvent ( QResizeEvent * event_n )
{
  QWidget::resizeEvent ( event_n );
  update_text_elided ();
}

void
Label_Elide::paintEvent ( QPaintEvent * event_n )
{
  QWidget::paintEvent ( event_n );
  QPainter painter ( this );
  painter.drawText ( contentsRect (), _alignment, _text_elided );
}

} // namespace Wdg
