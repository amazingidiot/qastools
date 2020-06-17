/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "tray_mixer_balloon.hpp"
#include "wdg/ds_switch.hpp"
#include "wdg/ds_widget_types.hpp"
#include <QEvent>
#include <QHBoxLayout>
#include <QIcon>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <iostream>

Tray_Mixer_Balloon::Tray_Mixer_Balloon ( ::dpe::Image_Allocator * image_alloc_n,
                                         QWidget * parent_n )
: ::Wdg::Balloon_Widget ( parent_n )
, _image_alloc ( image_alloc_n )
{
  _value_mask = tr ( "Volume is at %1%" );

  {
    _lbl_icon = new QLabel;
    _lbl_icon->setMinimumSize ( pixmap_size () );
    _lbl_icon->setCursor ( Qt::PointingHandCursor );
    _lbl_icon->setToolTip ( tr ( "Show mixer" ) );
    _lbl_icon->installEventFilter ( this );
  }

  QAbstractButton * btn_close ( 0 );
  if ( image_alloc_n != 0 ) {
    ::Wdg::DS_Switch * btn ( new ::Wdg::DS_Switch ( 0, image_alloc_n ) );
    btn->setCheckable ( false );
    btn->set_variant_id ( ::Wdg::DS_CLOSE );
    {
      int btn_size ( fontMetrics ().height () );
      btn->setIconSize ( QSize ( btn_size, btn_size ) );
      btn->setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
      btn->setFixedSize ( btn_size, btn_size );
    }

    connect ( btn, SIGNAL ( clicked () ), this, SLOT ( close () ) );

    btn_close = btn;
  }

  {
    _lbl_volume_value = new ::Wdg::Label_Width ();
    _lbl_volume_value->set_min_text ( _value_mask.arg ( 100 ) );

    _lbl_volume_muted = new QLabel;
    _lbl_volume_muted->setText ( tr ( "Volume muted" ) );
  }

  {
    int hspace ( fontMetrics ().averageCharWidth () * 3 / 2 );
    QWidget * wdg_balloon ( new QWidget );
    QHBoxLayout * lay_txt ( new QHBoxLayout );
    _lay_volume_stack = new QStackedLayout;

    lay_txt->setContentsMargins ( 0, 0, 0, 0 );
    lay_txt->addWidget ( _lbl_icon );
    lay_txt->addSpacing ( hspace );
    lay_txt->addLayout ( _lay_volume_stack );
    lay_txt->addSpacing ( hspace );
    lay_txt->addWidget ( btn_close );
    wdg_balloon->setLayout ( lay_txt );

    _lay_volume_stack->addWidget ( _lbl_volume_value );
    _lay_volume_stack->addWidget ( _lbl_volume_muted );
    _lay_volume_stack->setCurrentIndex ( 0 );
    add_widget ( wdg_balloon );
  }
}

QSize
Tray_Mixer_Balloon::pixmap_size () const
{
  unsigned int len ( fontMetrics ().height () * 5 / 2 );
  return QSize ( len, len );
}

void
Tray_Mixer_Balloon::set_muted ( bool flag_n )
{
  _lay_volume_stack->setCurrentIndex ( ( flag_n ? 1 : 0 ) );
}

void
Tray_Mixer_Balloon::set_permille ( unsigned int value_n )
{
  _lbl_volume_value->setText ( _value_mask.arg ( value_n / 10 ) );
}

void
Tray_Mixer_Balloon::set_icon ( const QIcon & icon_n )
{
  set_pixmap ( icon_n.pixmap ( pixmap_size () ) );
}

void
Tray_Mixer_Balloon::set_pixmap ( const QPixmap & pixmap_n )
{
  _lbl_icon->setPixmap ( pixmap_n );
}

void
Tray_Mixer_Balloon::mousePressEvent ( QMouseEvent * event_n )
{
  ::Wdg::Balloon_Widget::mousePressEvent ( event_n );
  if ( event_n->button () == Qt::LeftButton ) {
    close ();
  }
}

bool
Tray_Mixer_Balloon::eventFilter ( QObject * obj_n, QEvent * event_n )
{
  bool res ( ::Wdg::Balloon_Widget::eventFilter ( obj_n, event_n ) );

  if ( obj_n == _lbl_icon ) {
    if ( event_n->type () == QEvent::Wheel ) {
      QWheelEvent * ev_wheel ( static_cast< QWheelEvent * > ( event_n ) );
      emit sig_wheel_delta ( ev_wheel->delta () );
      res = true;
    } else if ( ( event_n->type () == QEvent::MouseButtonPress ) ||
                ( event_n->type () == QEvent::MouseButtonDblClick ) ) {
      res = true;
      QMouseEvent * ev_mouse ( static_cast< QMouseEvent * > ( event_n ) );
      switch ( ev_mouse->button () ) {
      case Qt::LeftButton:
        emit sig_activated ();
        break;
      case Qt::MidButton:
        emit sig_middle_click ();
        break;
      default:
        break;
      }
    }
  }

  return res;
}
