/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "layout_weights.hpp"
#include <algorithm>
#include <iostream>

namespace Wdg
{

void
Layout_Weights::normalize ( unsigned long weight_normal_n )
{
  unsigned long wsum ( sum () );
  if ( wsum == weight_normal_n ) {
    return;
  }
  if ( wsum == 0 ) {
    set_all ( weight_normal_n / 4 );
    wsum = sum ();
  }

  Layout_Weights weights_cur;
  Layout_Weights weights_cur_mod;

  for ( unsigned int ii = 0; ii < 4; ++ii ) {
    weights_cur[ ii ] = ( _weights[ ii ] * weight_normal_n );
  }
  for ( unsigned int ii = 0; ii < 4; ++ii ) {
    weights_cur_mod[ ii ] = ( weights_cur[ ii ] % wsum );
  }
  for ( unsigned int ii = 0; ii < 4; ++ii ) {
    weights_cur[ ii ] = ( weights_cur[ ii ] / wsum );
  }

  wsum = weights_cur.sum ();
  while ( wsum < weight_normal_n ) {

    // Find largest modulo (division rest) value
    unsigned int max_mod_idx ( 0 );
    {
      unsigned long max_mod ( 0 );
      for ( int ii = 0; ii < 4; ++ii ) {
        const unsigned long & cur_mod ( weights_cur_mod[ ii ] );
        if ( cur_mod > max_mod ) {
          max_mod = cur_mod;
          max_mod_idx = ii;
        }
      }
    }

    // Increase weight with largest division rest
    weights_cur[ max_mod_idx ] += 1;
    weights_cur_mod[ max_mod_idx ] = 0;

    wsum = weights_cur.sum ();
  }

  *this = weights_cur;
}

unsigned int
calc_weights_delta_order ( unsigned int * order_n,
                           const Layout_Weights & wnorm_req_n,
                           const Layout_Weights & wnorm_cur_n )
{
  unsigned int res ( 0 );

  // Use weight values as indices
  for ( unsigned int ii = 0; ii < 4; ++ii ) {
    order_n[ ii ] = ii;
  }

  for ( unsigned int ii = 0; ii < 4; ++ii ) {
    unsigned int min_idx ( ii );
    unsigned long min_delta_minus ( 0 );
    unsigned long min_delta_plus ( 0 );

    // Find minimum value in remaining values
    for ( unsigned int jj = ii; jj < 4; ++jj ) {
      unsigned int idx ( order_n[ jj ] );
      unsigned long delta_minus ( 0 );
      unsigned long delta_plus ( 0 );
      if ( wnorm_req_n[ idx ] > wnorm_cur_n[ idx ] ) {
        delta_minus = wnorm_req_n[ idx ] - wnorm_cur_n[ idx ];
      } else {
        delta_plus = wnorm_cur_n[ idx ] - wnorm_req_n[ idx ];
      }
      if ( ( delta_minus > min_delta_minus ) ||
           ( delta_plus < min_delta_plus ) ) {
        min_delta_minus = delta_minus;
        min_delta_plus = delta_plus;
        min_idx = jj;
      }
    }

    if ( min_delta_minus > 0 ) {
      ++res;
    }

    // Swap the minimum index to the front
    if ( min_idx != ii ) {
      std::swap ( order_n[ ii ], order_n[ min_idx ] );
    }
  }

  return res;
}

} // namespace Wdg
