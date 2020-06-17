/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_simple.hpp"
#include "mwdg/inputs_setup.hpp"
#include "mwdg/mixer_device_setup.hpp"
#include "mwdg/mixer_sliders.hpp"
#include "mwdg/mixer_sliders_status_widget.hpp"
#include "mwdg/mixer_style.hpp"
#include "mwdg/mixer_switches.hpp"
#include "qastools_config.hpp"
#include "qsnd/mixer_simple.hpp"
#include "views/message_widget.hpp"
#include "views/mixer_simple_setup.hpp"
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QIcon>
#include <QKeyEvent>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QWidgetAction>
#include <iostream>

namespace Views
{

Mixer_Simple::Mixer_Simple ( QWidget * parent_n )
: ::Views::View_Base ( parent_n )
, _view_setup ( 0 )
, _status_group_idx ( ~0 )
, _status_column_idx ( ~0 )
, _qsnd_mixer ( 0 )
{
  // Strings and icons
  _act_stream_text[ 0 ] = tr ( "Show playback" );
  _act_stream_text[ 1 ] = tr ( "Show capture" );
  _act_stream_ttip[ 0 ] = tr ( "Show playback elements" );
  _act_stream_ttip[ 1 ] = tr ( "Show capture elements" );

  {
    const char * fd_app[ 3 ] = {"show-playback", "show-capture"};
    const char * fd_def[ 3 ] = {"media-playback-start", "media-record"};

    QString icon_path_base ( INSTALL_DIR_APP_ICONS );
    icon_path_base.append ( "/" );
    for ( unsigned int ii = 0; ii < 2; ++ii ) {
      QIcon icon;
      {
        QString icon_path ( icon_path_base );
        icon_path.append ( fd_app[ ii ] );
        icon_path.append ( ".svg" );
        QFileInfo finfo ( icon_path );
        if ( finfo.exists () && finfo.isReadable () ) {
          icon = QIcon ( icon_path );
        } else {
          icon = QIcon::fromTheme ( fd_def[ ii ] );
        }
      }
      _act_stream_icon[ ii ] = icon;
    }
  }

  // Mixer object
  {
    _qsnd_mixer = new ::QSnd::Mixer_Simple ( this );
    connect ( _qsnd_mixer,
              SIGNAL ( sig_mixer_reload_request () ),
              this,
              SIGNAL ( sig_mdev_reload_request () ) );
  }

  // Mixer sliders
  {
    _mixer_sliders = new ::MWdg::Mixer_Sliders;

    connect (
        _mixer_sliders,
        SIGNAL ( sig_footer_label_selected ( unsigned int, unsigned int ) ),
        this,
        SLOT ( footer_label_selected ( unsigned int, unsigned int ) ) );
  }

  // Mixer switches
  {
    _switches_area = new QWidget;
    _mixer_switches = new ::MWdg::Mixer_Switches ( _switches_area );

    // Stream buttons
    for ( unsigned int ii = 0; ii < 2; ++ii ) {
      _btn_stream[ ii ] = new QPushButton ( _switches_area );
      _btn_stream[ ii ]->setCheckable ( true );
      if ( _act_stream_icon[ ii ].isNull () ) {
        _btn_stream[ ii ]->setText ( _act_stream_text[ ii ] );
      } else {
        _btn_stream[ ii ]->setIcon ( _act_stream_icon[ ii ] );
      }
      _btn_stream[ ii ]->setToolTip ( _act_stream_ttip[ ii ] );
    }

    connect ( _btn_stream[ 0 ],
              SIGNAL ( toggled ( bool ) ),
              this,
              SLOT ( show_playback ( bool ) ),
              Qt::QueuedConnection );

    connect ( _btn_stream[ 1 ],
              SIGNAL ( toggled ( bool ) ),
              this,
              SLOT ( show_capture ( bool ) ),
              Qt::QueuedConnection );
  }

  // Vertical splitter
  {
    _mixer_split.setOrientation ( Qt::Vertical );
    _mixer_split.addWidget ( _mixer_sliders );
    _mixer_split.addWidget ( _switches_area );

    QVBoxLayout * lay_vbox ( new QVBoxLayout );
    lay_vbox->setContentsMargins ( 0, 0, 0, 0 );
    lay_vbox->addWidget ( &_mixer_split );
    _wdg_areas.setLayout ( lay_vbox );

    lay_stack ()->addWidget ( &_wdg_areas );
  }

  // Adjust layout margins
  {
    QMargins mgs ( _mixer_sliders->contentsMargins () );
    mgs.setTop ( 0 );
    _mixer_sliders->setContentsMargins ( mgs );
  }

  if ( _mixer_sliders->layout () != 0 ) {
    QMargins mgs ( _mixer_sliders->layout ()->contentsMargins () );
    mgs.setTop ( 0 );
    _mixer_sliders->layout ()->setContentsMargins ( mgs );
  }
}

Mixer_Simple::~Mixer_Simple ()
{
  set_mdev_setup ( 0 );
  set_view_setup ( 0 );
}

void
Mixer_Simple::set_mdev_setup ( const ::MWdg::Mixer_Device_Setup * setup_n )
{
  if ( mdev_setup () != 0 ) {
    clear_view ();
    _qsnd_mixer->close ();
  }

  ::Views::View_Base::set_mdev_setup ( setup_n );

  if ( mdev_setup () != 0 ) {
    _qsnd_mixer->open ( mdev_setup ()->ctl_addr );
    setup_view ();
  }
}

void
Mixer_Simple::set_inputs_setup ( const ::MWdg::Inputs_Setup * setup_n )
{
  ::Views::View_Base::set_inputs_setup ( setup_n );
  _mixer_sliders->set_inputs_setup ( setup_n );
  _mixer_switches->set_inputs_setup ( setup_n );

  if ( inputs_setup () != 0 ) {
    // Append key sequence texts to the tooltip
    const QString & mask ( "%1 (%2)" );
    for ( unsigned int ii = 0; ii < 2; ++ii ) {
      const QKeySequence & kseq ( inputs_setup ()->ks_toggle_vis_stream[ ii ] );
      const QString & kstr ( kseq.toString ( QKeySequence::NativeText ) );
      if ( !kstr.isEmpty () ) {
        _btn_stream[ ii ]->setToolTip (
            mask.arg ( _act_stream_ttip[ ii ], kstr ) );
      }
    }
  }
}

void
Mixer_Simple::set_view_setup ( ::Views::View_Base_Setup * setup_n )
{
  clear_view ();
  _view_setup = dynamic_cast<::Views::Mixer_Simple_Setup * > ( setup_n );
  setup_view ();
}

void
Mixer_Simple::clear_view ()
{
  if ( ( mdev_setup () == 0 ) || ( _view_setup == 0 ) ) {
    return;
  }

  if ( _status_wdg != 0 ) {
    delete _status_wdg;
    _status_wdg = 0;
  }

  if ( _qsnd_mixer->is_open () ) {
    _mixer_sliders->set_mixer_setup ( 0 );
    _mixer_switches->set_mixer_setup ( 0 );
  }
}

void
Mixer_Simple::setup_view ()
{
  if ( ( mdev_setup () == 0 ) || ( _view_setup == 0 ) ) {
    return;
  }

  // Fix show stream flags on demand
  if ( !( _view_setup->show_stream[ 0 ] || _view_setup->show_stream[ 1 ] ) ) {
    _view_setup->show_stream[ 0 ] = true;
  }

  _btn_stream[ 0 ]->setChecked ( _view_setup->show_stream[ 0 ] );
  _btn_stream[ 1 ]->setChecked ( _view_setup->show_stream[ 1 ] );

  // The stream dirs shown are temporarily overridden for
  // devices with only one stream direction type
  bool show_stream[ 2 ];
  show_stream[ 0 ] = _view_setup->show_stream[ 0 ];
  show_stream[ 1 ] = _view_setup->show_stream[ 1 ];
  if ( ( _qsnd_mixer->num_elems_stream ( 0 ) == 0 ) ||
       ( _qsnd_mixer->num_elems_stream ( 1 ) == 0 ) ) {
    show_stream[ 0 ] = ( _qsnd_mixer->num_elems_stream ( 0 ) > 0 );
    show_stream[ 1 ] = ( _qsnd_mixer->num_elems_stream ( 1 ) > 0 );
  }

  _smixer_setup.show_stream[ 0 ] = show_stream[ 0 ];
  _smixer_setup.show_stream[ 1 ] = show_stream[ 1 ];
  _smixer_setup.show_slider_value_labels =
      _view_setup->show_slider_value_labels;
  _smixer_setup.mixer_simple = _qsnd_mixer;
  _smixer_setup.wdg_style_db = _view_setup->wdg_style_db;
  _smixer_setup.image_alloc = _view_setup->image_alloc;

  unsigned int lay_stack_idx ( 0 );
  if ( mdev_setup ()->ctl_addr.isEmpty () ) {
    message_wdg ()->set_no_device ();
  } else {
    if ( _qsnd_mixer->is_open () ) {
      lay_stack_idx = 1;
    } else {
      message_wdg ()->set_mixer_open_fail ( mdev_setup ()->ctl_addr,
                                            _qsnd_mixer->err_message (),
                                            _qsnd_mixer->err_func () );
    }
  }

  // Show simple mixer
  if ( lay_stack_idx == 1 ) {

    unsigned int num_vis_sliders ( 0 );
    unsigned int num_vis_switches ( 0 );

    for ( unsigned int ii = 0; ii < 2; ++ii ) {
      if ( _smixer_setup.show_stream[ ii ] ) {
        num_vis_sliders += _qsnd_mixer->num_elems_volume ( ii );
        num_vis_switches += _qsnd_mixer->num_elems_no_volume ( ii );
      }
    }

    bool vis_stream_select ( ( _qsnd_mixer->num_elems_stream ( 0 ) > 0 ) &&
                             ( _qsnd_mixer->num_elems_stream ( 1 ) > 0 ) );
    bool vis_switches_area ( vis_stream_select || ( num_vis_switches > 0 ) );
    bool vis_splitter ( ( num_vis_sliders > 0 ) && vis_switches_area );

    // Hide areas before installing changes
    _mixer_split.hide ();
    _mixer_sliders->hide ();
    _switches_area->hide ();

    // Install mixer setup
    if ( num_vis_sliders > 0 ) {
      _mixer_sliders->set_mixer_setup ( &_smixer_setup );
    }
    if ( vis_switches_area ) {
      _mixer_switches->set_mixer_setup ( &_smixer_setup );
    }

    // Switches area layout
    if ( vis_switches_area ) {
      QHBoxLayout * lay_hbox ( new QHBoxLayout );
      if ( num_vis_switches > 0 ) {
        lay_hbox->addWidget ( _mixer_switches );
      }
      // Stream select buttons layout
      if ( vis_stream_select ) {
        QBoxLayout * lay_btns;
        if ( num_vis_switches > 1 ) {
          lay_btns = new QVBoxLayout;
        } else {
          lay_btns = new QHBoxLayout;
        }
        lay_btns->setContentsMargins ( 0, 0, 0, 0 );
        lay_btns->setAlignment ( Qt::AlignRight | Qt::AlignBottom );
        lay_btns->addWidget ( _btn_stream[ 0 ] );
        lay_btns->addWidget ( _btn_stream[ 1 ] );

        lay_hbox->addLayout ( lay_btns );
      }
      if ( _switches_area->layout () != 0 ) {
        delete _switches_area->layout ();
      }
      _switches_area->setLayout ( lay_hbox );

      // Switches area items visiblity
      _mixer_switches->setVisible ( num_vis_switches > 0 );
      _btn_stream[ 0 ]->setVisible ( vis_stream_select );
      _btn_stream[ 1 ]->setVisible ( vis_stream_select );
    }

    // Update splitter layout
    if ( vis_splitter ) {
      _mixer_split.addWidget ( _mixer_sliders );
      _mixer_split.addWidget ( _switches_area );
      _mixer_split.setCollapsible ( 0, false );
      _mixer_split.setCollapsible ( 1, false );
      _mixer_split.setStretchFactor ( 0, 1 );
      _mixer_split.setStretchFactor ( 1, 0 );

      _mixer_sliders->show ();
      _switches_area->show ();
      {
        // Shrink switches area to minimum size
        QList< int > sizes;
        sizes.append ( height () ); // A value way too big
        sizes.append ( 1 );         // A value way too small
        _mixer_split.setSizes ( sizes );
      }
      _mixer_split.show ();
    } else {
      if ( num_vis_sliders > 0 ) {
        _wdg_areas.layout ()->addWidget ( _mixer_sliders );
      } else {
        _wdg_areas.layout ()->addWidget ( _switches_area );
      }
      _mixer_sliders->setVisible ( num_vis_sliders > 0 );
      _switches_area->setVisible ( vis_switches_area );
    }
  }

  lay_stack ()->setCurrentIndex ( lay_stack_idx );
}

void
Mixer_Simple::show_playback ( bool flag_n )
{
  if ( _view_setup == 0 ) {
    return;
  }
  if ( flag_n != _view_setup->show_stream[ 0 ] ) {
    clear_view ();

    _view_setup->show_stream[ 0 ] = flag_n;
    if ( !_view_setup->show_stream[ 0 ] && !_view_setup->show_stream[ 1 ] ) {
      _view_setup->show_stream[ 1 ] = true;
    }

    setup_view ();
    setFocus ();
  }
}

void
Mixer_Simple::show_capture ( bool flag_n )
{
  if ( _view_setup == 0 ) {
    return;
  }
  if ( flag_n != _view_setup->show_stream[ 1 ] ) {
    clear_view ();

    _view_setup->show_stream[ 1 ] = flag_n;
    if ( !_view_setup->show_stream[ 0 ] && !_view_setup->show_stream[ 1 ] ) {
      _view_setup->show_stream[ 0 ] = true;
    }

    setup_view ();
    setFocus ();
  }
}

void
Mixer_Simple::toggle_show_playback ()
{
  if ( _view_setup != 0 ) {
    show_playback ( !_view_setup->show_stream[ 0 ] );
  }
}

void
Mixer_Simple::toggle_show_capture ()
{
  if ( _view_setup != 0 ) {
    show_capture ( !_view_setup->show_stream[ 1 ] );
  }
}

void
Mixer_Simple::show_slider_value_widget ()
{
  if ( _status_wdg == 0 ) {
    ::MWdg::Mixer_Sliders_Status_Widget * swdg (
        new ::MWdg::Mixer_Sliders_Status_Widget ( this ) );
    swdg->setAttribute ( Qt::WA_DeleteOnClose );
    swdg->set_sliders_pad ( _mixer_sliders->sliders_pad () );
    swdg->slider_focus_changed ();

    _status_wdg = swdg;
    _status_wdg->show ();
  }
}

void
Mixer_Simple::footer_label_selected ( unsigned int group_idx_n,
                                      unsigned int column_idx_n )
{
  //::std::cout << "Footer label selected " << group_idx_n << " " <<
  //column_idx_n << "\n";
  bool keep_indices ( true );
  if ( _status_wdg == 0 ) {
    show_slider_value_widget ();
  } else {
    if ( ( group_idx_n == _status_group_idx ) &&
         ( column_idx_n == _status_column_idx ) ) {
      _status_group_idx = ~0;
      _status_column_idx = ~0;
      _status_wdg->close ();
      keep_indices = false;
    }
  }

  if ( keep_indices ) {
    _status_group_idx = group_idx_n;
    _status_column_idx = column_idx_n;
  }
}

void
Mixer_Simple::keyPressEvent ( QKeyEvent * event_n )
{
  bool handled ( false );
  if ( inputs_setup () != 0 ) {
    handled = true;
    const QKeySequence kseq ( event_n->key () );
    if ( kseq == inputs_setup ()->ks_toggle_vis_stream[ 0 ] ) {
      toggle_show_playback ();
    } else if ( kseq == inputs_setup ()->ks_toggle_vis_stream[ 1 ] ) {
      toggle_show_capture ();
    } else {
      handled = false;
    }
  }
  if ( !handled ) {
    ::Views::View_Base::keyPressEvent ( event_n );
  }
}

void
Mixer_Simple::showEvent ( QShowEvent * event_n )
{
  View_Base::showEvent ( event_n );
  if ( _status_wdg != 0 ) {
    _status_wdg->show ();
  }
}

void
Mixer_Simple::hideEvent ( QHideEvent * event_n )
{
  View_Base::hideEvent ( event_n );
  if ( _status_wdg != 0 ) {
    _status_wdg->hide ();
  }
}

} // namespace Views
