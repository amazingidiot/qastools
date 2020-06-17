/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_mwdg_ctl_arg_view_hpp__
#define __INC_mwdg_ctl_arg_view_hpp__

#include <qsnd/ctl_format_argument.hpp>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

// Forward declaration
namespace QSnd
{
class Controls_Database;
class Cards_Model;
} // namespace QSnd

namespace MWdg
{

/// @brief CTL_Arg_View
///
class CTL_Arg_View : public QWidget
{
  Q_OBJECT

  // Public methods
  public:
  CTL_Arg_View ( QWidget * parent_n = 0 );

  // Controls database

  const ::QSnd::Controls_Database *
  ctl_db () const;

  void
  set_ctl_db ( const ::QSnd::Controls_Database * ctl_db_n );

  // Cards database

  ::QSnd::Cards_Model *
  cards_model () const;

  void
  set_cards_model ( ::QSnd::Cards_Model * cards_model_n );

  // Control argument

  const ::QSnd::CTL_Format_Argument &
  ctl_arg () const;

  void
  set_ctl_arg ( const ::QSnd::CTL_Format_Argument & ctl_arg_n );

  const QString &
  arg_string () const;

  virtual void
  set_arg_string ( const QString & str_n );

  // Signals
  signals:

  void
  sig_arg_changed ();

  // Protected methods
  protected:
  bool
  set_arg_string_private ( const QString & str_n );

  virtual void
  ctl_db_changed ();

  virtual void
  cards_model_changed ();

  virtual void
  ctl_arg_changed ();

  QVBoxLayout *
  lay_content ();

  // Private attributes
  private:
  const ::QSnd::Controls_Database * _ctl_db;
  ::QSnd::Cards_Model * _cards_model;

  ::QSnd::CTL_Format_Argument _ctl_arg;
  QString _arg_string;
  QLabel * _wdg_title;
  QVBoxLayout * _lay_content;
};

inline const ::QSnd::Controls_Database *
CTL_Arg_View::ctl_db () const
{
  return _ctl_db;
}

inline ::QSnd::Cards_Model *
CTL_Arg_View::cards_model () const
{
  return _cards_model;
}

inline const ::QSnd::CTL_Format_Argument &
CTL_Arg_View::ctl_arg () const
{
  return _ctl_arg;
}

inline const QString &
CTL_Arg_View::arg_string () const
{
  return _arg_string;
}

inline QVBoxLayout *
CTL_Arg_View::lay_content ()
{
  return _lay_content;
}

} // namespace MWdg

#endif
