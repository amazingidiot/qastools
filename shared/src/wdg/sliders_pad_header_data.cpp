/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "sliders_pad_header_data.hpp"

namespace Wdg
{

//
// Sliders_Pad_Header_Label
//
Sliders_Pad_Header_Label::Sliders_Pad_Header_Label ()
: label_length_max ( 0 )
, group_idx ( 0 )
, column_idx ( 0 )
, column_total_idx ( 0 )
{
}

//
// Sliders_Pad_Header_Data
//

Sliders_Pad_Header_Data::Sliders_Pad_Header_Data ()
: update_elided_texts ( false )
, update_decoration ( false )
, upside_down ( false )
, column_labels ( false )
, label_sliding ( true )
, angle ( 0.0 )
, angle_sin ( 0.0 )
, angle_cos ( 1.0 )
, max_str_length_px ( 100 )
, pad_left ( 2 )
, pad_right ( 1 )
, spacing_inter ( 3 )
, spacing_vertical ( 3 )
{
}

} // namespace Wdg
