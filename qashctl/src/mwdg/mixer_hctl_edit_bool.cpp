/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl_edit_bool.hpp"
#include "mwdg/mixer_hctl_editor_data.hpp"
#include "mwdg/mixer_hctl_proxies_group.hpp"
#include "mwdg/mixer_hctl_proxy_switch.hpp"
#include "qsnd/mixer_hctl_elem.hpp"
#include "qsnd/mixer_hctl_elem_group.hpp"
#include "wdg/ds_switch.hpp"
#include "wdg/label_width.hpp"
#include <QCheckBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QVBoxLayout>
#include <iostream>

namespace MWdg
{

Mixer_HCTL_Edit_Bool::Mixer_HCTL_Edit_Bool ( Mixer_HCTL_Editor_Data * data_n,
                                             QWidget * parent_n )
: Mixer_HCTL_Editor ( data_n, parent_n )
{
  if ( editor_data ()->elem_idx <
       editor_data ()->snd_elem_group->num_elems () ) {
    setup_single ();
  } else {
    setup_multi ();
  }
  update_proxies_values ();
}

Mixer_HCTL_Edit_Bool::~Mixer_HCTL_Edit_Bool () {}

Mixer_HCTL_Proxies_Group *
Mixer_HCTL_Edit_Bool::create_proxies_group ( ::QSnd::Mixer_HCTL_Elem * elem_n )
{
  Mixer_HCTL_Proxies_Group * pgroup ( new Mixer_HCTL_Proxies_Group ( this ) );

  const unsigned int num_channels ( elem_n->count () );
  for ( unsigned int ii = 0; ii < num_channels; ++ii ) {
    Mixer_HCTL_Proxy_Switch * mcps ( new Mixer_HCTL_Proxy_Switch ( pgroup ) );
    mcps->set_snd_elem ( elem_n );
    mcps->set_elem_idx ( ii );
    mcps->set_enabled ( elem_n->is_writable () );
    pgroup->append_proxy ( mcps );
  }

  pgroup->set_joined ( elem_n->switches_equal () );
  return pgroup;
}

void
Mixer_HCTL_Edit_Bool::setup_single ()
{
  ::QSnd::Mixer_HCTL_Elem * elem (
      editor_data ()->snd_elem_group->elem ( editor_data ()->elem_idx ) );
  const bool is_enabled ( elem->is_writable () );

  Mixer_HCTL_Proxies_Group * pgroup ( create_proxies_group ( elem ) );
  _proxies_groups.append ( pgroup );

  QHBoxLayout * lay_checks ( new QHBoxLayout );
  lay_checks->setContentsMargins ( 0, 0, 0, 0 );

  {
    QList< QWidget * > widgets;

    for ( unsigned int ii = 0; ii < pgroup->num_proxies (); ++ii ) {

      Mixer_HCTL_Proxy_Switch * mcps (
          static_cast< Mixer_HCTL_Proxy_Switch * > ( pgroup->proxy ( ii ) ) );

      ::Wdg::DS_Switch * wdg_sw (
          new ::Wdg::DS_Switch ( 0, editor_data ()->image_alloc ) );

      wdg_sw->installEventFilter ( mcps );
      wdg_sw->setEnabled ( mcps->is_enabled () );
      wdg_sw->set_style_id ( elem_style_id ( elem ) );

      connect ( mcps,
                SIGNAL ( sig_enabled_changed ( bool ) ),
                wdg_sw,
                SLOT ( setEnabled ( bool ) ) );

      connect ( mcps,
                SIGNAL ( sig_switch_state_changed ( bool ) ),
                wdg_sw,
                SLOT ( setChecked ( bool ) ) );

      connect ( wdg_sw,
                SIGNAL ( toggled ( bool ) ),
                mcps,
                SLOT ( set_switch_state ( bool ) ) );

      widgets.append ( wdg_sw );
    }

    QLayout * lay_grid ( create_channel_grid ( widgets, false ) );
    lay_checks->addLayout ( lay_grid, 0 );
    lay_checks->addStretch ( 1 );
  }

  // Mass checking buttons

  QVBoxLayout * lay_extra ( new QVBoxLayout );
  lay_extra->setContentsMargins ( 0, 0, 0, 0 );

  // Joined button
  if ( ( pgroup->num_proxies () > 1 ) && is_enabled ) {
    QCheckBox * btn ( new QCheckBox );
    btn->setText ( editor_data ()->str_joined );
    btn->setChecked ( elem->switches_equal () );
    connect ( btn,
              SIGNAL ( toggled ( bool ) ),
              pgroup,
              SLOT ( set_joined ( bool ) ) );

    QHBoxLayout * lay_hor ( new QHBoxLayout );
    lay_hor->setContentsMargins ( 0, 0, 0, 0 );
    lay_hor->addWidget ( btn, 0 );
    lay_hor->addStretch ( 1 );

    lay_extra->addLayout ( lay_hor );
  }

  QVBoxLayout * lay_vbox ( new QVBoxLayout );
  lay_vbox->setContentsMargins ( 0, 0, 0, 0 );
  lay_vbox->addLayout ( lay_checks );
  lay_vbox->addLayout ( lay_extra );
  lay_vbox->addStretch ( 1 );

  setLayout ( lay_vbox );
}

void
Mixer_HCTL_Edit_Bool::setup_multi ()
{
  // Create proxies
  const unsigned int num_elems ( editor_data ()->snd_elem_group->num_elems () );
  for ( unsigned int eii = 0; eii < num_elems; ++eii ) {
    ::QSnd::Mixer_HCTL_Elem * elem (
        editor_data ()->snd_elem_group->elem ( eii ) );
    Mixer_HCTL_Proxies_Group * pgroup ( create_proxies_group ( elem ) );
    _proxies_groups.append ( pgroup );
  }

  QVBoxLayout * lay_switches_pad ( new QVBoxLayout );
  lay_switches_pad->setContentsMargins ( 0, 0, 0, 0 );

  {
    QGridLayout * lay_grid ( new QGridLayout );
    lay_grid->setContentsMargins ( 0, 0, 0, 0 );

    // const Qt::Alignment align_rc ( Qt::AlignRight | Qt::AlignVCenter );
    const Qt::Alignment align_cc ( Qt::AlignHCenter | Qt::AlignVCenter );
    const QString val ( "%1" );

    unsigned int max_channels ( 0 );
    unsigned int num_groups ( _proxies_groups.size () );
    for ( unsigned int gii = 0; gii < num_groups; ++gii ) {
      ::QSnd::Mixer_HCTL_Elem * elem (
          editor_data ()->snd_elem_group->elem ( gii ) );
      Mixer_HCTL_Proxies_Group * pgroup ( _proxies_groups[ gii ] );

      // Element index label
      {
        ::Wdg::Label_Width * wdg_lbl ( new ::Wdg::Label_Width );
        wdg_lbl->setAlignment ( align_cc );
        wdg_lbl->set_min_text ( "999" );
        wdg_lbl->setText ( val.arg ( elem->elem_index () ) );
        wdg_lbl->setToolTip ( editor_data ()->ttip_grid_lbl_elem );
        lay_grid->addWidget ( wdg_lbl, 0, 1 + gii, align_cc );
      }

      if ( max_channels < pgroup->num_proxies () ) {
        max_channels = pgroup->num_proxies ();
      }

      for ( unsigned int pii = 0; pii < pgroup->num_proxies (); ++pii ) {

        Mixer_HCTL_Proxy_Switch * mcps (
            static_cast< Mixer_HCTL_Proxy_Switch * > (
                pgroup->proxy ( pii ) ) );

        // Switch widget
        ::Wdg::DS_Switch * wdg_sw (
            new ::Wdg::DS_Switch ( 0, editor_data ()->image_alloc ) );

        wdg_sw->installEventFilter ( mcps );
        wdg_sw->setEnabled ( mcps->is_enabled () );
        wdg_sw->setToolTip (
            editor_data ()->ttip_grid_widget.arg ( gii ).arg ( pii ) );
        wdg_sw->set_style_id ( elem_style_id ( elem ) );

        connect ( mcps,
                  SIGNAL ( sig_enabled_changed ( bool ) ),
                  wdg_sw,
                  SLOT ( setEnabled ( bool ) ) );

        connect ( mcps,
                  SIGNAL ( sig_switch_state_changed ( bool ) ),
                  wdg_sw,
                  SLOT ( setChecked ( bool ) ) );

        connect ( wdg_sw,
                  SIGNAL ( toggled ( bool ) ),
                  mcps,
                  SLOT ( set_switch_state ( bool ) ) );

        lay_grid->addWidget ( wdg_sw, 1 + pii, 1 + gii, align_cc );
      }
    }

    // Joined buttons
    if ( max_channels > 1 ) {
      for ( unsigned int cii = 0; cii < max_channels; ++cii ) {
        QLabel * lbl ( new QLabel );
        lbl->setText ( editor_data ()->str_list_channel.arg ( cii ) );
        lbl->setToolTip ( editor_data ()->ttip_list_channel.arg ( cii ) );
        lay_grid->addWidget ( lbl, 1 + cii, 0 );
      }
      {
        QLabel * lbl ( new QLabel ( editor_data ()->str_joined ) );
        lay_grid->addWidget ( lbl, 1 + max_channels, 0 );
      }
      for ( unsigned int gii = 0; gii < num_groups; ++gii ) {
        ::QSnd::Mixer_HCTL_Elem * elem (
            editor_data ()->snd_elem_group->elem ( gii ) );
        Mixer_HCTL_Proxies_Group * pgroup ( _proxies_groups[ gii ] );

        QWidget * btn ( create_small_joined_switch ( pgroup, elem ) );
        lay_grid->addWidget ( btn, 1 + max_channels, 1 + gii );
      }
    }

    // Wrap into stretching layout
    {
      QHBoxLayout * lay_switches_grid ( new QHBoxLayout );
      lay_switches_grid->setContentsMargins ( 0, 0, 0, 0 );
      lay_switches_grid->addLayout ( lay_grid );
      lay_switches_grid->addStretch ( 1 );

      lay_switches_pad->addLayout ( lay_switches_grid );
      lay_switches_pad->addStretch ( 1 );
    }
  }

  // Scroll area
  QScrollArea * scroll_area ( new QScrollArea );
  {
    QWidget * switches_pad ( new QWidget );
    switches_pad->setLayout ( lay_switches_pad );

    scroll_area->setFrameStyle ( QFrame::NoFrame );
    scroll_area->setWidget ( switches_pad );
  }

  // Pad layout
  {
    QVBoxLayout * lay_pad ( new QVBoxLayout );
    lay_pad->setContentsMargins ( 0, 0, 0, 0 );
    lay_pad->addWidget ( scroll_area );
    setLayout ( lay_pad );
  }
}

void
Mixer_HCTL_Edit_Bool::update_proxies_values ()
{
  for ( int gii = 0; gii < _proxies_groups.size (); ++gii ) {
    _proxies_groups[ gii ]->update_values ();
  }
}

} // namespace MWdg
