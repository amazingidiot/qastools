/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "cubic_curve.hpp"
#include <iostream>

namespace Wdg
{

Cubic_Curve::Cubic_Curve ()
{
  for ( int ii = 0; ii < 4; ++ii ) {
    _coeff[ ii ] = 0.0;
  }
}

void
Cubic_Curve::set_params ( double x0_n, double v0_n, double x1_n, double time_n )
{
  double xdiff ( x1_n - x0_n );
  double tt ( time_n );
  double tt2 ( tt * tt );
  double tt3 ( tt * tt * tt );

  _coeff[ 0 ] = x0_n;
  _coeff[ 1 ] = v0_n;
  _coeff[ 2 ] = ( 3.0 * xdiff / tt2 ) - ( 2.0 * v0_n / tt );
  _coeff[ 3 ] = ( v0_n / tt2 ) - ( 2.0 * xdiff / tt3 );
}

double
Cubic_Curve::eval ( double pos_n ) const
{
  double res ( _coeff[ 0 ] );
  res += _coeff[ 1 ] * ( pos_n );
  res += _coeff[ 2 ] * ( pos_n * pos_n );
  res += _coeff[ 3 ] * ( pos_n * pos_n * pos_n );
  return res;
}

double
Cubic_Curve::eval_speed ( double pos_n ) const
{
  double res ( _coeff[ 1 ] );
  res += 2.0 * _coeff[ 2 ] * ( pos_n );
  res += 3.0 * _coeff[ 3 ] * ( pos_n * pos_n );
  return res;
}

} // namespace Wdg
