/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "color_methods.hpp"

namespace Wdg
{

QColor
col_mix ( const QColor & col_1, const QColor & col_2, int w_1, int w_2 )
{
  const int wsum ( w_1 + w_2 );

  unsigned int com1[ 4 ];
  com1[ 0 ] = col_1.alpha ();
  com1[ 1 ] = col_1.red ();
  com1[ 2 ] = col_1.green ();
  com1[ 3 ] = col_1.blue ();

  for ( int ii = 0; ii < 4; ++ii ) {
    com1[ ii ] *= w_1;
  }

  {
    unsigned int com2[ 4 ];
    com2[ 0 ] = col_2.alpha ();
    com2[ 1 ] = col_2.red ();
    com2[ 2 ] = col_2.green ();
    com2[ 3 ] = col_2.blue ();

    for ( int ii = 0; ii < 4; ++ii ) {
      com2[ ii ] *= w_2;
    }

    // Mix
    for ( int ii = 0; ii < 4; ++ii ) {
      com1[ ii ] += com2[ ii ];
    }
  }

  // Rescale
  for ( int ii = 0; ii < 4; ++ii ) {
    com1[ ii ] /= wsum;
  }

  QColor res ( com1[ 1 ], com1[ 2 ], com1[ 3 ], com1[ 0 ] );
  return res;
}

} // namespace Wdg
