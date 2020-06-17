/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl_edit_enum.hpp"
#include "mwdg/mixer_hctl_editor_data.hpp"
#include "mwdg/mixer_hctl_proxies_group.hpp"
#include "mwdg/mixer_hctl_proxy_enum.hpp"
#include "qsnd/mixer_hctl_elem.hpp"
#include "qsnd/mixer_hctl_elem_group.hpp"
#include "wdg/label_width.hpp"
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QVBoxLayout>
#include <iostream>

namespace MWdg
{

Mixer_HCTL_Edit_Enum::Mixer_HCTL_Edit_Enum ( Mixer_HCTL_Editor_Data * data_n,
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

Mixer_HCTL_Edit_Enum::~Mixer_HCTL_Edit_Enum () {}

Mixer_HCTL_Proxies_Group *
Mixer_HCTL_Edit_Enum::create_proxies_group ( ::QSnd::Mixer_HCTL_Elem * elem_n )
{
  Mixer_HCTL_Proxies_Group * pgroup ( new Mixer_HCTL_Proxies_Group ( this ) );

  const unsigned int num_channels ( elem_n->count () );
  for ( unsigned int ii = 0; ii < num_channels; ++ii ) {
    Mixer_HCTL_Proxy_Enum * mcpe ( new Mixer_HCTL_Proxy_Enum ( pgroup ) );
    mcpe->set_snd_elem ( elem_n );
    mcpe->set_elem_idx ( ii );
    mcpe->set_enabled ( elem_n->is_writable () );
    pgroup->append_proxy ( mcpe );
  }

  pgroup->set_joined ( elem_n->switches_equal () );
  return pgroup;
}

void
Mixer_HCTL_Edit_Enum::setup_single ()
{
  ::QSnd::Mixer_HCTL_Elem * elem (
      editor_data ()->snd_elem_group->elem ( editor_data ()->elem_idx ) );
  const bool is_enabled ( elem->is_writable () );

  Mixer_HCTL_Proxies_Group * pgroup ( create_proxies_group ( elem ) );
  _proxies_groups.append ( pgroup );

  //
  // ComboBox area
  //

  QHBoxLayout * lay_items ( new QHBoxLayout );
  lay_items->setContentsMargins ( 0, 0, 0, 0 );

  {
    QList< QWidget * > wdg_items;
    QStringList item_names;

    {
      unsigned int num_items ( elem->enum_num_items () );
      for ( unsigned int ii = 0; ii < num_items; ++ii ) {
        item_names.append ( elem->enum_item_display_name ( ii ) );
      }
    }

    for ( unsigned int pii = 0; pii < pgroup->num_proxies (); ++pii ) {

      Mixer_HCTL_Proxy_Enum * mcpe (
          static_cast< Mixer_HCTL_Proxy_Enum * > ( pgroup->proxy ( pii ) ) );

      QComboBox * wdg_sel ( new QComboBox );

      wdg_sel->setEnabled ( mcpe->is_enabled () );
      wdg_sel->addItems ( item_names );
      wdg_sel->installEventFilter ( mcpe );

      connect ( mcpe,
                SIGNAL ( sig_enabled_changed ( bool ) ),
                wdg_sel,
                SLOT ( setEnabled ( bool ) ) );

      connect ( mcpe,
                SIGNAL ( sig_enum_index_changed ( int ) ),
                wdg_sel,
                SLOT ( setCurrentIndex ( int ) ) );

      connect ( wdg_sel,
                SIGNAL ( currentIndexChanged ( int ) ),
                mcpe,
                SLOT ( set_enum_index ( int ) ) );

      wdg_items.append ( wdg_sel );
    }

    QLayout * lay_grid ( create_channel_grid ( wdg_items, false ) );
    lay_items->addLayout ( lay_grid, 0 );
    lay_items->addStretch ( 1 );
  }

  // Extra buttons/labels layout
  QHBoxLayout * lay_extra ( new QHBoxLayout );
  lay_extra->setContentsMargins ( 0, 0, 0, 0 );
  if ( ( pgroup->num_proxies () > 1 ) && is_enabled ) {
    QCheckBox * btn ( new QCheckBox );
    btn->setText ( editor_data ()->str_joined );
    btn->setChecked ( elem->enum_idices_equal () );
    connect ( btn,
              SIGNAL ( toggled ( bool ) ),
              pgroup,
              SLOT ( set_joined ( bool ) ) );

    lay_extra->addWidget ( btn );
    lay_extra->addStretch ( 1 );
  }

  // Pad layout
  {
    QVBoxLayout * lay_pad ( new QVBoxLayout );
    lay_pad->setContentsMargins ( 0, 0, 0, 0 );
    lay_pad->addLayout ( lay_items, 0 );
    lay_pad->addSpacing ( fontMetrics ().height () );
    lay_pad->addLayout ( lay_extra, 0 );
    lay_pad->addStretch ( 1 );
    setLayout ( lay_pad );
  }
}

void
Mixer_HCTL_Edit_Enum::setup_multi ()
{
  // Create proxies
  const unsigned int num_elems ( editor_data ()->snd_elem_group->num_elems () );
  for ( unsigned int eii = 0; eii < num_elems; ++eii ) {
    ::QSnd::Mixer_HCTL_Elem * elem (
        editor_data ()->snd_elem_group->elem ( eii ) );
    Mixer_HCTL_Proxies_Group * pgroup ( create_proxies_group ( elem ) );
    _proxies_groups.append ( pgroup );
  }

  QVBoxLayout * lay_enums_pad ( new QVBoxLayout );
  lay_enums_pad->setContentsMargins ( 0, 0, 0, 0 );

  {
    QGridLayout * lay_grid ( new QGridLayout );
    lay_grid->setContentsMargins ( 0, 0, 0, 0 );

    QStringList item_names;

    // const Qt::Alignment align_rc ( Qt::AlignRight | Qt::AlignVCenter );
    const Qt::Alignment align_cc ( Qt::AlignHCenter | Qt::AlignVCenter );
    const QString val ( "%1" );

    unsigned int max_channels ( 0 );
    const unsigned int num_groups ( _proxies_groups.size () );
    for ( unsigned int gii = 0; gii < num_groups; ++gii ) {
      ::QSnd::Mixer_HCTL_Elem * elem (
          editor_data ()->snd_elem_group->elem ( gii ) );
      Mixer_HCTL_Proxies_Group * pgroup ( _proxies_groups[ gii ] );

      // Acquire item names
      {
        item_names.clear ();
        unsigned int num_items ( elem->enum_num_items () );
        for ( unsigned int ii = 0; ii < num_items; ++ii ) {
          item_names.append ( elem->enum_item_display_name ( ii ) );
        }
      }

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

        Mixer_HCTL_Proxy_Enum * mcpe (
            static_cast< Mixer_HCTL_Proxy_Enum * > ( pgroup->proxy ( pii ) ) );

        // Selection widget
        QComboBox * wdg_sel ( new QComboBox );
        wdg_sel->setEnabled ( mcpe->is_enabled () );
        wdg_sel->setToolTip (
            editor_data ()->ttip_grid_widget.arg ( gii ).arg ( pii ) );
        wdg_sel->installEventFilter ( mcpe );
        wdg_sel->addItems ( item_names );

        connect ( mcpe,
                  SIGNAL ( sig_enum_index_changed ( int ) ),
                  wdg_sel,
                  SLOT ( setCurrentIndex ( int ) ) );

        connect ( wdg_sel,
                  SIGNAL ( currentIndexChanged ( int ) ),
                  mcpe,
                  SLOT ( set_enum_index ( int ) ) );

        lay_grid->addWidget ( wdg_sel, 1 + pii, 1 + gii, align_cc );
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
      QHBoxLayout * lay_enums_grid ( new QHBoxLayout );
      lay_enums_grid->setContentsMargins ( 0, 0, 0, 0 );
      lay_enums_grid->addLayout ( lay_grid );
      lay_enums_grid->addStretch ( 1 );

      lay_enums_pad->addLayout ( lay_enums_grid );
      lay_enums_pad->addStretch ( 1 );
    }
  }

  // Scroll area
  QScrollArea * scroll_area ( new QScrollArea );
  {
    QWidget * enums_pad ( new QWidget );
    enums_pad->setLayout ( lay_enums_pad );

    scroll_area->setFrameStyle ( QFrame::NoFrame );
    scroll_area->setWidget ( enums_pad );
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
Mixer_HCTL_Edit_Enum::update_proxies_values ()
{
  for ( int gii = 0; gii < _proxies_groups.size (); ++gii ) {
    _proxies_groups[ gii ]->update_values ();
  }
}

} // namespace MWdg
