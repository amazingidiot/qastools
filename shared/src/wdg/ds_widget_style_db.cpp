/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "ds_widget_style_db.hpp"

namespace Wdg
{

DS_Widget_Style_Db::DS_Widget_Style_Db ()
{
  QPalette pal;
  palettes.insert ( ST_NORMAL, pal );
}

} // namespace Wdg
