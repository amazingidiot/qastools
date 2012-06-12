//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_dpe2_pixmap_handle_hpp__
#define __INC_dpe2_pixmap_handle_hpp__

#include "pixmap.hpp"
#include "values_set.hpp"
#include <QTime>
#include <QScopedPointer>

namespace dpe2
{


/// @brief Pixmap_Handle
///
class Pixmap_Handle
{
	// Public methods
	public:

	Pixmap_Handle ( );

	~Pixmap_Handle ( );


	// Public attributes
	public:

	QScopedPointer < ::dpe2::Pixmap > pixmap;
	::dpe2::Values_Set id_values;

	unsigned int num_users;
	QTime remove_time;
};


} // End of namespace

#endif
