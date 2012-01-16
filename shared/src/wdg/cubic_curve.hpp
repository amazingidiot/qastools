//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//


#ifndef __INC_cubic_curve_hpp__
#define __INC_cubic_curve_hpp__


namespace Wdg
{


class Cubic_Curve
{
	// Public methods
	public:

	Cubic_Curve ( );

	double
	eval (
		double pos_n ) const;

	double
	eval_speed (
		double pos_n ) const;

	void
	set_params (
		double x0_n,
		double v0_n,
		double x1_n,
		double time_n );


	// Private attributes
	private:

	double _coeff[4];
};


} // End of namespace

#endif
