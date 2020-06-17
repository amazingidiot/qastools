/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mwdg/mixer_hctl_editor.hpp"
#include "mwdg/mixer_hctl_editor_data.hpp"
#include "mwdg/mixer_hctl_proxies_group.hpp"
#include "mwdg/mixer_style.hpp"
#include "qsnd/mixer_hctl_elem.hpp"
#include "wdg/ds_switch.hpp"
#include "wdg/ds_widget_types.hpp"
#include "wdg/label_width.hpp"
#include <QCheckBox>
#include <iostream>

namespace MWdg
{

Mixer_HCTL_Editor::Mixer_HCTL_Editor ( Mixer_HCTL_Editor_Data * data_n,
                                       QWidget * parent_n )
: QWidget ( parent_n )
, _editor_data ( data_n )
, _inputs_setup ( 0 )
{
}

Mixer_HCTL_Editor::~Mixer_HCTL_Editor () {}

void
Mixer_HCTL_Editor::set_inputs_setup ( const ::MWdg::Inputs_Setup * setup_n )
{
  _inputs_setup = setup_n;
}

void
Mixer_HCTL_Editor::update_proxies_values ()
{
  // Dummy
}

QGridLayout *
Mixer_HCTL_Editor::create_channel_grid ( const QList< QWidget * > & items_n,
                                         bool bold_labels_n )
{
  QGridLayout * lay_grid ( new QGridLayout () );
  lay_grid->setContentsMargins ( 0, 0, 0, 0 );

  if ( items_n.size () == 0 ) {
    return lay_grid;
  }

  const Qt::Alignment align_cc ( Qt::AlignHCenter | Qt::AlignVCenter );

  unsigned int num ( items_n.size () );
  unsigned int lay_cols ( 1 );
  for ( unsigned int ii = 1; ii <= num; ii *= 2 ) {
    if ( ii * ii >= num ) {
      lay_cols = ii;
      break;
    }
  }

  const QString val ( "%1" );
  for ( unsigned int cii = 0; cii < num; ++cii ) {
    QWidget * wdg_input ( items_n[ cii ] );
    const QString str_idx ( val.arg ( cii ) );

    ::Wdg::Label_Width * wdg_label ( new ::Wdg::Label_Width );
    wdg_label->set_min_text ( val.arg ( 999 ) );
    wdg_label->setText ( str_idx );
    wdg_label->setToolTip (
        editor_data ()->ttip_grid_lbl_channel.arg ( str_idx ) );
    wdg_label->setAlignment ( align_cc );
    wdg_label->setEnabled ( wdg_input->isEnabled () );

    if ( bold_labels_n ) {
      QFont fnt ( wdg_label->font () );
      fnt.setBold ( true );
      wdg_label->setFont ( fnt );
    }

    unsigned int lay_row = ( cii / lay_cols );
    unsigned int lay_col = ( cii % lay_cols );
    lay_grid->addWidget ( wdg_label, lay_row * 2, lay_col, align_cc );
    lay_grid->addWidget ( wdg_input, lay_row * 2 + 1, lay_col, align_cc );
  }

  return lay_grid;
}

unsigned int
Mixer_HCTL_Editor::elem_style_id (
    const ::QSnd::Mixer_HCTL_Elem * elem_n ) const
{
  unsigned int style_id ( 0 );
  {
    const QString ename ( elem_n->elem_name () );
    if ( ename.contains ( "playback", Qt::CaseInsensitive ) ) {
      style_id = ::MWdg::Mixer_Style::PLAYBACK;
    } else if ( ename.contains ( "capture", Qt::CaseInsensitive ) ) {
      style_id = ::MWdg::Mixer_Style::CAPTURE;
    } else if ( ename.contains ( "input", Qt::CaseInsensitive ) ) {
      style_id = ::MWdg::Mixer_Style::CAPTURE;
    } else if ( ename.contains ( "mic", Qt::CaseInsensitive ) ) {
      style_id = ::MWdg::Mixer_Style::CAPTURE;
    } else {
      style_id = ::MWdg::Mixer_Style::PLAYBACK;
    }
  }
  return style_id;
}

QWidget *
Mixer_HCTL_Editor::create_small_joined_switch (
    ::MWdg::Mixer_HCTL_Proxies_Group * pgroup_n,
    ::QSnd::Mixer_HCTL_Elem * elem_n )
{
  ::Wdg::DS_Switch * btn (
      new ::Wdg::DS_Switch ( 0, editor_data ()->image_alloc ) );
  btn->setChecked ( elem_n->values_equal () );
  btn->setToolTip ( editor_data ()->str_joined );
  btn->set_variant_id ( ::Wdg::DS_SVG_JOINED );

  connect ( btn,
            SIGNAL ( toggled ( bool ) ),
            pgroup_n,
            SLOT ( set_joined ( bool ) ) );

  return btn;
}

} // namespace MWdg
