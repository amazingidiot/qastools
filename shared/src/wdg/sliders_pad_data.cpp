/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "sliders_pad_data.hpp"
#include "pad_proxies_column.hpp"
#include "sliders_pad_footer.hpp"
#include "sliders_pad_header.hpp"
#include <iostream>

namespace Wdg
{

Sliders_Pad_Data_Column::Sliders_Pad_Data_Column ( QObject * parent_n )
: QObject ( parent_n )
, col_idx ( 0 )
, col_total_idx ( 0 )
, col_pos ( 0 )
, col_width ( 0 )
, show_value_label ( true )
, sppc ( 0 )
{
}

void
Sliders_Pad_Data_Column::update_footer_label ()
{
  if ( show_value_label ) {
    const QString & txt ( sppc->value_string () );
    sp_data ()->footer_cast ()->set_label_text ( col_total_idx, txt );
  }
}

Sliders_Pad_Data_Group::Sliders_Pad_Data_Group ( QObject * parent_n )
: QObject ( parent_n )
, group_idx ( 0 )
, group_pos ( 0 )
, group_width ( 0 )
, num_sliders ( 0 )
, num_switches ( 0 )
, sppg ( 0 )
{
}

Sliders_Pad_Data::Sliders_Pad_Data ( QObject * parent_n )
: QObject ( parent_n )
, header ( 0 )
, footer ( 0 )
, show_value_labels ( true )
{
}

Sliders_Pad_Header *
Sliders_Pad_Data::header_cast ()
{
  return static_cast< Sliders_Pad_Header * > ( header );
}

Sliders_Pad_Footer *
Sliders_Pad_Data::footer_cast ()
{
  return static_cast< Sliders_Pad_Footer * > ( footer );
}

} // namespace Wdg
