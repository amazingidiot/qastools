/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_wdg_color_methods_hpp__
#define __INC_wdg_color_methods_hpp__

#include <QColor>

namespace Wdg
{

QColor
col_mix ( const QColor & col_1, const QColor & col_2, int w_1, int w_2 );

} // namespace Wdg

#endif
