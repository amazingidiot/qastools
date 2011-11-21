//
// C++ Interface:
//
// Description:
//
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef __INC_mwdg_mixer_hctl_hpp__
#define __INC_mwdg_mixer_hctl_hpp__

#include <wdg/label_width.hpp>

#include <QString>
#include <QAction>
#include <QMenu>
#include <QLabel>


// Forward declaration
namespace QSnd {
	class Mixer_HCTL_Elem_Group;
	class Mixer_HCTL_Info_Db;
}
namespace dpe {
	class Image_Allocator;
}
namespace Wdg {
	class DS_Widget_Style_Db;
}
namespace MWdg {
	class Mixer_HCTL_Editor;
	class Mixer_HCTL_Editor_Data;
	class Mixer_HCTL_Info_Db;
	class Mixer_HCTL_Elem_Group;
	class Inputs_Setup;
}


namespace MWdg
{


/// @brief Mixer_HCTL
///
/// This widgets shows the appropriate Mixer_HCTL_Editor widget
/// for a given ::QSnd::Mixer_HCTL_Elem_Group
///
class Mixer_HCTL :
	public QWidget
{
	Q_OBJECT

	// Public typedefs
	public:

	struct Label_Pair {
		::Wdg::Label_Width name;
		::Wdg::Label_Width value;
	};


	// Public methods
	public:

	Mixer_HCTL (
		QWidget * parent_n = 0 );

	~Mixer_HCTL ( );


	// Editor data

	const ::MWdg::Mixer_HCTL_Editor_Data *
	editor_data ( ) const;

	::MWdg::Mixer_HCTL_Editor_Data *
	editor_data ( );


	// Shared data

	void
	set_ctl_info_db (
		const ::QSnd::Mixer_HCTL_Info_Db * info_db_n );

	const ::QSnd::Mixer_HCTL_Info_Db *
	ctl_info_db ( ) const;


	// Element

	::QSnd::Mixer_HCTL_Elem_Group *
	snd_elem_group ( ) const;

	unsigned int
	elem_idx ( ) const;

	void
	set_snd_elem_group (
		::QSnd::Mixer_HCTL_Elem_Group * elem_group_n,
		unsigned int index_n = 0 );


	// Image allocator

	::dpe::Image_Allocator *
	image_alloc ( ) const;

	void
	set_image_alloc (
		::dpe::Image_Allocator * alloc_n );


	// Widget style db

	const ::Wdg::DS_Widget_Style_Db *
	wdg_style_db ( ) const;

	void
	set_wdg_style_db (
		const ::Wdg::DS_Widget_Style_Db * style_db_n );


	// Inputs setup

	const ::MWdg::Inputs_Setup *
	inputs_setup ( ) const;

	void
	set_inputs_setup (
		const ::MWdg::Inputs_Setup * setup_n  );


	// Protected methods
	protected:

	void
	clear ( );

	void
	update_info ( );


	void
	setup_widgets ( );


	// Private attributes
	private:

	::MWdg::Mixer_HCTL_Editor_Data * _editor_data;
	const ::MWdg::Inputs_Setup * _inputs_setup;

	// Widgets
	QFrame _info_wdg;
	QWidget _pad_wdg;

	::MWdg::Mixer_HCTL_Editor * _editor_pad;

	QLabel _info_lbl_name;
	Label_Pair _info_lbl_index;
	Label_Pair _info_lbl_dev;
	Label_Pair _info_lbl_flags;
	Label_Pair _info_lbl_count;
	Label_Pair _info_lbl_numid;

	QString _info_dev_mask;
	QString _ttip_name_lbl_mask;
};


inline
const ::MWdg::Mixer_HCTL_Editor_Data *
Mixer_HCTL::editor_data ( ) const
{
	return _editor_data;
}


inline
::MWdg::Mixer_HCTL_Editor_Data *
Mixer_HCTL::editor_data ( )
{
	return _editor_data;
}


inline
const ::MWdg::Inputs_Setup *
Mixer_HCTL::inputs_setup ( ) const
{
	return _inputs_setup;
}


} // End of namespace


#endif
