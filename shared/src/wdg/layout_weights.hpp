/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_layout_weights_hpp__
#define __INC_layout_weights_hpp__

namespace Wdg
{

///
/// @brief Layout_Weights
///
class Layout_Weights
{
  // Public methods
  public:
  Layout_Weights ();

  Layout_Weights ( unsigned int * values_n );

  Layout_Weights ( unsigned long * values_n );

  Layout_Weights ( unsigned long value_all_n );

  unsigned long
  sum () const;

  void
  set_all ( unsigned long value_n );

  void
  normalize ( unsigned long weight_normal_n = 4096 );

  unsigned long operator[] ( unsigned int idx_n ) const;

  unsigned long & operator[] ( unsigned int idx_n );

  // Private attributes
  private:
  unsigned long _weights[ 4 ];
};

unsigned int
calc_weights_delta_order ( unsigned int * order_n,
                           const Layout_Weights & wnorm_req_n,
                           const Layout_Weights & wnorm_cur_n );

//
// Definitions
//

inline Layout_Weights::Layout_Weights () {}

inline Layout_Weights::Layout_Weights ( unsigned int * values_n )
{
  for ( unsigned int ii = 0; ii < 4; ++ii ) {
    _weights[ ii ] = values_n[ ii ];
  }
}

inline Layout_Weights::Layout_Weights ( unsigned long * values_n )
{
  for ( unsigned int ii = 0; ii < 4; ++ii ) {
    _weights[ ii ] = values_n[ ii ];
  }
}

inline Layout_Weights::Layout_Weights ( unsigned long value_all_n )
{
  for ( unsigned int ii = 0; ii < 4; ++ii ) {
    _weights[ ii ] = value_all_n;
  }
}

inline unsigned long Layout_Weights::operator[] ( unsigned int idx_n ) const
{
  return _weights[ idx_n ];
}

inline unsigned long & Layout_Weights::operator[] ( unsigned int idx_n )
{
  return _weights[ idx_n ];
}

inline void
Layout_Weights::set_all ( unsigned long value_n )
{
  for ( unsigned int ii = 0; ii < 4; ++ii ) {
    _weights[ ii ] = value_n;
  }
}

inline unsigned long
Layout_Weights::sum () const
{
  unsigned long res ( 0 );
  for ( unsigned int ii = 0; ii < 4; ++ii ) {
    res += _weights[ ii ];
  }
  return res;
}

} // namespace Wdg

#endif
