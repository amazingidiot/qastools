/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "slider_status_widget.hpp"
#include "wdg/sliders_pad.hpp"
#include <QAction>
#include <QContextMenuEvent>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QMenu>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <iostream>

namespace MWdg
{

Slider_Status_Widget::Slider_Status_Widget ( QWidget * parent_n )
: QWidget ( parent_n, Qt::Tool )
, _sliders_pad ( 0 )
{
  setWindowTitle ( tr ( "Slider value" ) );

  QFont fnt_title ( _volume_title.font () );
  fnt_title.setWeight ( QFont::Bold );

  // const Qt::Alignment align_lc ( Qt::AlignLeft | Qt::AlignVCenter );
  const Qt::Alignment align_rc ( Qt::AlignRight | Qt::AlignVCenter );
  const Qt::Alignment align_cc ( Qt::AlignHCenter | Qt::AlignVCenter );

  _lbl_value.setText ( tr ( "Value" ) );
  _lbl_value.setFont ( fnt_title );

  _lbl_max.setText ( tr ( "Max." ) );
  _lbl_max.setToolTip ( tr ( "Maximum" ) );
  _lbl_max.setFont ( fnt_title );

  _lbl_min.setText ( tr ( "Min." ) );
  _lbl_min.setFont ( fnt_title );
  _lbl_min.setToolTip ( tr ( "Minimum" ) );

  // Volume
  // Volume label
  _volume_title.setFont ( fnt_title );
  _volume_title.setText ( tr ( "Volume" ) );

  _volume_value.setSingleStep ( 1 );
  _volume_value.setKeyboardTracking ( false );

  // Volume value
  _volume_value.setToolTip ( tr ( "Current volume" ) );
  connect ( &_volume_value,
            SIGNAL ( valueChanged ( int ) ),
            this,
            SLOT ( volume_value_changed ( int ) ) );

  // Volume range
  _volume_max.setAlignment ( align_cc );
  _volume_max.setToolTip ( tr ( "Volume maximum" ) );
  _volume_max.setTextInteractionFlags ( Qt::TextSelectableByMouse );
  _volume_max.set_min_text ( volume_string ( -65000 ) );

  _volume_min.setAlignment ( align_cc );
  _volume_min.setToolTip ( tr ( "Volume minimum" ) );
  _volume_min.setTextInteractionFlags ( Qt::TextSelectableByMouse );
  _volume_min.set_min_text ( volume_string ( -65000 ) );

  // Decibel
  // Decibel label
  _db_title.setFont ( fnt_title );
  _db_title.setText ( tr ( "Decibel" ) );

  // Decibel value
  _db_value.setButtonSymbols ( QAbstractSpinBox::NoButtons );
  _db_value.setSingleStep ( 0 );
  _db_value.setKeyboardTracking ( false );
  _db_value.setToolTip ( tr ( "Current Decibel value" ) );

  connect ( &_db_value,
            SIGNAL ( valueChanged ( double ) ),
            this,
            SLOT ( db_value_changed ( double ) ) );

  // Decibel range
  _db_max.setAlignment ( align_rc );
  _db_max.set_min_text ( db_string ( -99.99 ) );
  _db_max.setToolTip ( tr ( "Decibel maximum" ) );
  _db_max.setTextInteractionFlags ( Qt::TextSelectableByMouse );

  _db_min.setAlignment ( align_rc );
  _db_min.set_min_text ( db_string ( -99.99 ) );
  _db_min.setToolTip ( tr ( "Decibel minimum" ) );
  _db_min.setTextInteractionFlags ( Qt::TextSelectableByMouse );

  // Element name
  _elem_name.setFont ( fnt_title );
  _elem_name.setToolTip ( tr ( "Element name" ) );

  // Layout
  {
    QVBoxLayout * lay_vbox ( new QVBoxLayout );

    lay_vbox->addWidget ( &_elem_name );

    {
      QGridLayout * lay_grid ( new QGridLayout );
      lay_grid->setContentsMargins ( 0, 0, 0, 0 );

      const int fnt_w ( fontMetrics ().averageCharWidth () );
      const int fnt_h ( fontMetrics ().height () );
      QSpacerItem * spacer[ 8 ];
      for ( unsigned int ii = 0; ii < 8; ++ii ) {
        spacer[ ii ] = new QSpacerItem ( fnt_w, fnt_h );
      }

      unsigned int sp_idx ( 0 );
      unsigned int row ( 0 );
      unsigned int col ( 0 );
      ++col;
      lay_grid->addItem ( spacer[ sp_idx++ ], row, col++ );
      lay_grid->addWidget ( &_volume_title, row, col++ );
      lay_grid->addItem ( spacer[ sp_idx++ ], row, col++ );
      lay_grid->addWidget ( &_db_title, row, col++ );

      ++row;
      col = 0;
      lay_grid->addWidget ( &_lbl_value, row, col++ );
      lay_grid->addItem ( spacer[ sp_idx++ ], row, col++ );
      lay_grid->addWidget ( &_volume_value, row, col++ );
      lay_grid->addItem ( spacer[ sp_idx++ ], row, col++ );
      lay_grid->addWidget ( &_db_value, row, col++ );

      ++row;
      col = 0;
      lay_grid->addWidget ( &_lbl_max, row, col++ );
      lay_grid->addItem ( spacer[ sp_idx++ ], row, col++ );
      lay_grid->addWidget ( &_volume_max, row, col++ );
      lay_grid->addItem ( spacer[ sp_idx++ ], row, col++ );
      lay_grid->addWidget ( &_db_max, row, col++ );

      ++row;
      col = 0;
      lay_grid->addWidget ( &_lbl_min, row, col++ );
      lay_grid->addItem ( spacer[ sp_idx++ ], row, col++ );
      lay_grid->addWidget ( &_volume_min, row, col++ );
      lay_grid->addItem ( spacer[ sp_idx++ ], row, col++ );
      lay_grid->addWidget ( &_db_min, row, col++ );

      QBoxLayout * lay_hbox ( new QHBoxLayout () );
      lay_hbox->setContentsMargins ( 0, 0, 0, 0 );
      lay_hbox->addLayout ( lay_grid );
      lay_hbox->addStretch ( 1 );

      lay_vbox->addLayout ( lay_hbox );
    }

    lay_vbox->addStretch ( 1 );

    setLayout ( lay_vbox );
  }
}

Slider_Status_Widget::~Slider_Status_Widget () {}

void
Slider_Status_Widget::set_sliders_pad ( ::Wdg::Sliders_Pad * pad_n )
{
  if ( sliders_pad () != 0 ) {
    disconnect ( sliders_pad (), 0, this, 0 );
  }

  _sliders_pad = pad_n;

  if ( sliders_pad () != 0 ) {
    connect ( sliders_pad (),
              SIGNAL ( sig_focus_changed () ),
              this,
              SLOT ( slider_focus_changed () ) );
  }
}

void
Slider_Status_Widget::slider_focus_changed ()
{
  //::std::cout << "Slider_Status_Widget::slider_focus_changed" << "\n";

  if ( sliders_pad () != 0 ) {
    if ( sliders_pad ()->focus_info ().has_focus ) {
      this->select_slider ( sliders_pad ()->focus_info ().group_idx,
                            sliders_pad ()->focus_info ().column_idx );
    }
  }
}

void
Slider_Status_Widget::volume_value_changed ( int value_n )
{
  this->elem_set_volume ( value_n );
}

void
Slider_Status_Widget::db_value_changed ( double value_n )
{
  const long dB_long ( value_n * 100.0 );
  this->elem_set_nearest_dB ( dB_long );
  update_values ();
}

void
Slider_Status_Widget::mixer_values_changed ()
{
  update_values ();
}

void
Slider_Status_Widget::update_values ()
{
  QString txt ( "%1" );
  {
    const long val ( this->elem_volume_value () );
    _volume_value.blockSignals ( true );
    _volume_value.setValue ( val );
    _volume_value.blockSignals ( false );
  }

  if ( this->elem_has_dB () ) {
    const double val ( this->elem_dB_value () / 100.0 );
    _db_value.blockSignals ( true );
    _db_value.setValue ( val );
    _db_value.blockSignals ( false );
  }
}

void
Slider_Status_Widget::setup_values ()
{
  QString txt_special;
  QString txt_max;
  QString txt_min;

  long vol_min;
  long vol_max;

  bool has_vol ( this->elem_has_volume () );
  bool has_db ( this->elem_has_dB () );

  _elem_name.setText ( this->elem_name () );

  // Volume
  _volume_title.setEnabled ( has_vol );
  _volume_value.setEnabled ( has_vol );
  _volume_max.setEnabled ( has_vol );
  _volume_min.setEnabled ( has_vol );

  if ( has_vol ) {
    vol_min = this->elem_volume_min ();
    vol_max = this->elem_volume_max ();

    txt_special = QString ();
    txt_max = volume_string ( vol_max );
    txt_min = volume_string ( vol_min );
  } else {
    vol_min = 0;
    vol_max = 0;

    txt_special = " ";
    txt_max = "";
    txt_min = "";
  }

  _volume_value.blockSignals ( true );
  _volume_value.setSpecialValueText ( txt_special );
  _volume_value.setRange ( vol_min, vol_max );
  _volume_value.setValue ( vol_min ); // To show the special text on demand
  _volume_value.blockSignals ( false );

  _volume_max.setText ( txt_max );
  _volume_min.setText ( txt_min );

  // Decibel

  _db_title.setEnabled ( has_db );
  _db_value.setEnabled ( has_db );
  _db_max.setEnabled ( has_db );
  _db_min.setEnabled ( has_db );

  if ( has_db ) {
    vol_min = this->elem_dB_min ();
    vol_max = this->elem_dB_max ();

    txt_special = QString ();
    txt_max = db_string ( vol_max / 100.0 );
    txt_min = db_string ( vol_min / 100.0 );
  } else {
    vol_min = -9900;
    vol_max = 0;

    txt_special = " ";
    txt_max = "";
    txt_min = "";
  }

  _db_value.blockSignals ( true );
  _db_value.setSpecialValueText ( txt_special );
  _db_value.setRange ( vol_min / 100.0, vol_max / 100.0 );
  _db_value.setValue ( vol_min / 100.0 ); // To show the special text on demand
  _db_value.blockSignals ( false );

  _db_max.setText ( txt_max );
  _db_min.setText ( txt_min );

  updateGeometry ();
}

void
Slider_Status_Widget::contextMenuEvent ( QContextMenuEvent * event_n )
{
  QMenu cmenu ( this );

  QAction * act_close (
      cmenu.addAction ( tr ( "Close slider value display" ) ) );
  if ( QIcon::hasThemeIcon ( "window-close" ) ) {
    act_close->setIcon ( QIcon::fromTheme ( "window-close" ) );
  }
  connect ( act_close, SIGNAL ( triggered ( bool ) ), this, SLOT ( close () ) );

  cmenu.exec ( event_n->globalPos () );
}

} // namespace MWdg
