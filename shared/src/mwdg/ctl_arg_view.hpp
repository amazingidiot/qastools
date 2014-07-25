//
// QasTools: Dektop toolset for the Linux sound system ALSA
//
// License: See COPYING file that comes with this source distribution
//
// Author: Sebastian Holtermann <sebholt@xwmw.org>, (C) 2012
//

#ifndef __INC_mwdg_ctl_arg_view_hpp__
#define __INC_mwdg_ctl_arg_view_hpp__

#include <qsnd/ctl_format_argument.hpp>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>


// Forward declaration
namespace QSnd {
	class Controls_Database;
}


namespace MWdg
{


/// @brief CTL_Arg_View
///
class CTL_Arg_View :
	public QWidget
{
	Q_OBJECT


	// Public methods
	public:

	CTL_Arg_View (
		QWidget * parent_n = 0 );


	// Controls database

	const ::QSnd::Controls_Database *
	ctl_db ( ) const;

	void
	set_ctl_db (
		const ::QSnd::Controls_Database * ctl_db_n );


	// Control argument

	const ::QSnd::CTL_Format_Argument &
	ctl_arg ( ) const;

	void
	set_ctl_arg (
		const ::QSnd::CTL_Format_Argument & ctl_arg_n );

	virtual
	QString
	arg_string ( ) const;

	virtual
	void
	set_arg_string (
		const QString & str_n );


	// Signals
	signals:

	void
	sig_arg_changed ( );


	// Protected methods
	protected:

	virtual
	void
	ctl_db_changed ( );

	virtual
	void
	ctl_arg_changed ( );

	QVBoxLayout *
	lay_content ( );


	// Private attributes
	private:

	const ::QSnd::Controls_Database * _ctl_db;
	::QSnd::CTL_Format_Argument _ctl_arg;
	QLabel * _wdg_title;
	QVBoxLayout * _lay_content;
};


inline
const ::QSnd::Controls_Database *
CTL_Arg_View::ctl_db ( ) const
{
	return _ctl_db;
}

inline
const ::QSnd::CTL_Format_Argument &
CTL_Arg_View::ctl_arg ( ) const
{
	return _ctl_arg;
}

inline
QVBoxLayout *
CTL_Arg_View::lay_content ( )
{
	return _lay_content;
}


} // End of namespace


#endif
