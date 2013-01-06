//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// For license information check the LICENSE-QasTools.txt file 
// that comes with this source distribution.
//

#ifndef __INC_dpe2_painter_hpp__
#define __INC_dpe2_painter_hpp__

#include "pixmap_ref.hpp"
#include "pixmap_request.hpp"
#include <QPainter>
#include <QMutex>

namespace dpe2
{


/// @brief Painter
///
class Painter
{
	// Public static variables
	public:

	static const unsigned int _img_area_max = 4096*4096;


	// Public methods
	public:

	Painter ( );

	virtual
	~Painter ( );

	QMutex &
	mutex ( );

	::dpe2::Pixmap_IRef0 *
	iref0_create ( );

	void
	iref0_destroy (
		::dpe2::Pixmap_IRef0 * iref0_n );

	void
	iref1_deref (
		::dpe2::Pixmap_IRef1 * iref1_n );

	/// @brief Signalizes this painter feels responsible
	///
	/// Must be implemented in a thread safe fashion
	virtual
	bool
	is_responsible (
		const ::dpe2::Key_Values & vset_n ) = 0;

	/// @brief Used in find_match. Can be reimplemented.
	///
	virtual
	bool
	key_values_match (
		const ::dpe2::Key_Values & vset1_n,
		const ::dpe2::Key_Values & vset2_n ) const;

	::dpe2::Pixmap_IRef1 *
	find_match (
		const ::dpe2::Key_Values & vset1_n );


	/// @brief Must be implemented in a thread safe fashion
	///
	/// The pixmap is already resized to the requested size
	virtual
	void
	paint (
		::dpe2::Pixmap & pxmap_n,
		const ::dpe2::Key_Values & kvals_n ) = 0;

	bool
	valid_size (
		unsigned int width_n,
		unsigned int height_n );


	// Private attributes
	private:

	typedef ::std::vector < ::dpe2::Pixmap_IRef0 * > IRef0_List;

	IRef0_List _iref0s;
	QMutex _mutex;
};


inline
QMutex &
Painter::mutex ( )
{
	return _mutex;
}


} // End of namespace

#endif
