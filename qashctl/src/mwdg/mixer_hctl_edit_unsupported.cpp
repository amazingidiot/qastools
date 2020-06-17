/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "mixer_hctl_edit_unsupported.hpp"
#include "mwdg/mixer_hctl_editor_data.hpp"
#include "qsnd/mixer_hctl_elem.hpp"
#include "qsnd/mixer_hctl_elem_group.hpp"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <iostream>

namespace MWdg
{

Mixer_HCTL_Edit_Unsupported::Mixer_HCTL_Edit_Unsupported (
    Mixer_HCTL_Editor_Data * data_n, QWidget * parent_n )
: Mixer_HCTL_Editor ( data_n, parent_n )
{
  _str_unsupported = tr ( "Elements of the type %1 are not supported" );
  setup_single ();
}

Mixer_HCTL_Edit_Unsupported::~Mixer_HCTL_Edit_Unsupported () {}

void
Mixer_HCTL_Edit_Unsupported::setup_single ()
{
  QLabel * lbl_head ( new QLabel );
  {
    ::QSnd::Mixer_HCTL_Elem * elem (
        editor_data ()->snd_elem_group->elem ( 0 ) );

    QString val ( elem->elem_type_display_name () );
    val = _str_unsupported.arg ( val );
    val = QString ( "<h4>%1</h4>" ).arg ( val );
    lbl_head->setText ( val );
  }

  // Pad layout
  {
    const unsigned int vspace ( qMax ( 0, fontMetrics ().height () / 2 ) );
    QVBoxLayout * lay_pad ( new QVBoxLayout );
    lay_pad->setContentsMargins ( 0, 0, 0, 0 );
    lay_pad->addSpacing ( vspace );
    lay_pad->addWidget ( lbl_head, 0 );
    lay_pad->addStretch ( 1 );
    setLayout ( lay_pad );
  }
}

} // namespace MWdg
