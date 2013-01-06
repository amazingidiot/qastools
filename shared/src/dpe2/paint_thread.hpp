//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_dpe2_paint_thread_hpp__
#define __INC_dpe2_paint_thread_hpp__

#include <QThread>

// Forward declaration
namespace dpe2 {
	class Pixmap_Server_Shared;
}


namespace dpe2
{


/// @brief Paint_Thread
///
class Paint_Thread :
	public QThread
{
	// Public methods
	public:

	Paint_Thread (
		::dpe2::Pixmap_Server_Shared & shared_n );

	// Protected methods
	protected:

	void
	run ( );

	// Private attributes
	private:

	::dpe2::Pixmap_Server_Shared & _shared;
};


} // End of namespace


#endif
