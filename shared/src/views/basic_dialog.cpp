/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "basic_dialog.hpp"
#include <iostream>

namespace Views
{

Basic_Dialog::Basic_Dialog ( QWidget * parent_n, Qt::WindowFlags flags_n )
: QDialog ( parent_n, flags_n )
{
  _lay_main = new QVBoxLayout;
  setLayout ( _lay_main );

  _lbl_title = create_title_widget ( QString () );

  _lay_main->addWidget ( _lbl_title, 0, Qt::AlignTop );
}

Basic_Dialog::~Basic_Dialog () {}

void
Basic_Dialog::set_title_str ( const QString & str_n )
{
  _lbl_title->setText ( str_n );
}

void
Basic_Dialog::set_central_widget ( QWidget * wdg_n )
{
  _lay_main->addWidget ( wdg_n, 1 );
}

QLabel *
Basic_Dialog::create_title_widget ( const QString & txt_n )
{
  QLabel * lbl ( new QLabel ( txt_n ) );
  {
    QMargins marg ( lbl->contentsMargins () );
    marg.setTop ( lbl->fontMetrics ().height () * 1 / 4 );
    marg.setBottom ( lbl->fontMetrics ().height () * 3 / 4 );
    lbl->setContentsMargins ( marg );
  }
  {
    QSizePolicy policy ( lbl->sizePolicy () );
    policy.setHorizontalPolicy ( QSizePolicy::Expanding );
    policy.setVerticalPolicy ( QSizePolicy::Fixed );
    lbl->setSizePolicy ( policy );
  }
  {
    QFont fnt ( lbl->font () );
    fnt.setBold ( true );
    double scale ( 1.8 );
    if ( fnt.pixelSize () > 0 ) {
      fnt.setPixelSize ( fnt.pixelSize () * scale );
    } else {
      fnt.setPointSizeF ( fnt.pointSizeF () * scale );
    }
    lbl->setFont ( fnt );
  }

  return lbl;
}

QPushButton *
Basic_Dialog::create_close_button ()
{
  QPushButton * btn ( new QPushButton ( tr ( "&Close" ) ) );
  const char * icon_name ( "window-close" );
  if ( QIcon::hasThemeIcon ( icon_name ) ) {
    btn->setIcon ( QIcon::fromTheme ( icon_name ) );
  }
  connect ( btn, SIGNAL ( clicked () ), this, SLOT ( close () ) );

  return btn;
}

} // namespace Views
