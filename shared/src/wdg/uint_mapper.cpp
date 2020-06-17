/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "uint_mapper.hpp"
#include <iostream>

namespace Wdg
{

//
// Utility functions
//

unsigned int
integer_distance ( int int_min_n, int int_max_n )
{
  unsigned int res ( 0 );

  if ( int_min_n > int_max_n ) {
    int tmp = int_max_n;
    int_max_n = int_min_n;
    int_min_n = tmp;
  }

  if ( ( int_min_n >= 0 ) || ( int_max_n < 0 ) ) {
    // No integer overflow possible
    res = int ( int_max_n - int_min_n );
  } else {
    // Integer overflow possible
    res = int_max_n;
    ++int_min_n;
    res += int ( -int_min_n );
    ++res;
  }

  return res;
}

unsigned long
integer_distance ( long int_min_n, long int_max_n )
{
  unsigned long res;

  if ( int_min_n > int_max_n ) {
    long tmp = int_max_n;
    int_max_n = int_min_n;
    int_min_n = tmp;
  }

  if ( ( int_min_n > 0 ) || ( int_max_n < 0 ) ) {
    // No integer overflow possible
    res = long ( int_max_n - int_min_n );
  } else {
    // Integer overflow possible
    res = int_max_n;
    ++int_min_n;
    res += long ( -int_min_n );
    ++res;
  }

  return res;
}

unsigned int
permille ( unsigned long current_n, unsigned long total_n )
{
  unsigned int res;
  if ( total_n > 16000 ) {
    res = current_n / ( total_n / 1000 );
  } else {
    res = ( current_n * 1000 ) / total_n;
  }
  return res;
}

UInt_Mapper::~UInt_Mapper () {}

//
// UInt_Mapper_Down
//
UInt_Mapper_Down::UInt_Mapper_Down ( unsigned long max_idx_fine,
                                     unsigned long max_idx_coarse )
: _max_idx_fine ( max_idx_fine )
, _max_idx_coarse ( max_idx_coarse )
, _dist_small ( 0 )
, _dist_large ( 0 )
, _len_mod ( 0 )
{

  if ( _max_idx_coarse > 0 ) {
    _dist_small = _max_idx_fine / _max_idx_coarse;
    _dist_large = _dist_small;
    _len_mod = _max_idx_fine % _max_idx_coarse;

    if ( _len_mod > 0 ) {
      ++_dist_large;
    }
  }

  _mid_pos = _len_mod * _dist_large;
}

unsigned long
UInt_Mapper_Down::map ( unsigned long val_n ) const
{
  unsigned long res ( 0 );

  if ( _dist_small > 0 ) {
    unsigned long res_mod;
    unsigned long mod_lim;

    if ( val_n <= _mid_pos ) {
      res = val_n / _dist_large;
      res_mod = val_n % _dist_large;
      mod_lim = _dist_large / 2;
    } else {
      val_n -= _mid_pos;
      res = _len_mod;
      res += val_n / _dist_small;
      res_mod = val_n % _dist_small;
      mod_lim = _dist_small / 2;
    }

    if ( res_mod > mod_lim ) {
      ++res;
    }
  }

  return res;
}

//
// UInt_Mapper_Up
//
UInt_Mapper_Up::UInt_Mapper_Up ( unsigned long max_idx_coarse,
                                 unsigned long max_idx_fine )
: _max_idx_coarse ( max_idx_coarse )
, _max_idx_fine ( max_idx_fine )
, _len_div ( 0 )
, _len_mod ( 0 )
{
  if ( max_idx_coarse > 0 ) {
    _len_div = _max_idx_fine / _max_idx_coarse;
    _len_mod = _max_idx_fine % _max_idx_coarse;
  }
}

unsigned long
UInt_Mapper_Up::map ( unsigned long val_n ) const
{
  unsigned long res ( val_n * _len_div );

  if ( val_n >= _len_mod ) {
    res += _len_mod;
  } else {
    res += val_n;
  }

  return res;
}

//
// UInt_Mapper_Auto
//
UInt_Mapper_Auto::UInt_Mapper_Auto ( unsigned long max_idx_1_n,
                                     unsigned long max_idx_2_n )
: _max_idx_1 ( 0 )
, _max_idx_2 ( 0 )
{
  set_max_idx_all ( max_idx_1_n, max_idx_2_n );
}

void
UInt_Mapper_Auto::set_max_idx_all ( unsigned long max_idx_1_n,
                                    unsigned long max_idx_2_n )
{
  _max_idx_1 = max_idx_1_n;
  _max_idx_2 = max_idx_2_n;

  unsigned long lfine;
  unsigned long lcoarse;

  if ( max_idx_1 () > max_idx_2 () ) {
    lfine = max_idx_1 ();
    lcoarse = max_idx_2 ();
  } else {
    lfine = max_idx_2 ();
    lcoarse = max_idx_1 ();
  }

  _mapper_down = UInt_Mapper_Down ( lfine, lcoarse );
  _mapper_up = UInt_Mapper_Up ( lcoarse, lfine );
}

unsigned long
UInt_Mapper_Auto::v1_to_v2 ( unsigned long val_n ) const
{
  unsigned long res;

  if ( max_idx_1 () > max_idx_2 () ) {
    res = _mapper_down.map ( val_n );
  } else {
    res = _mapper_up.map ( val_n );
  }

  return res;
}

unsigned long
UInt_Mapper_Auto::v2_to_v1 ( unsigned long val_n ) const
{
  unsigned long res;

  if ( max_idx_1 () > max_idx_2 () ) {
    res = _mapper_up.map ( val_n );
  } else {
    res = _mapper_down.map ( val_n );
  }

  return res;
}

} // namespace Wdg
