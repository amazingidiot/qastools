/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "message_widget.hpp"
#include <QVBoxLayout>

namespace Views
{

Message_Widget::Message_Widget ( QWidget * parent_n )
: QWidget ( parent_n )
, _title_mask ( "<h3>%1</h3>" )
{
  {
    QString cstyle (
        "style=\"padding-left: 0px; padding-right: %1px; padding-top: 0px; "
        "padding-bottom: %2px;\" align=\"left\"" );
    cstyle = cstyle.arg ( fontMetrics ().averageCharWidth () * 2 );
    cstyle = cstyle.arg ( fontMetrics ().height () * 1 / 2 );

    _table_row_mask.append ( "<tr>" );
    _table_row_mask.append ( "<th " );
    _table_row_mask.append ( cstyle );
    _table_row_mask.append ( " >%1</th>" );
    _table_row_mask.append ( "<td " );
    _table_row_mask.append ( cstyle );
    _table_row_mask.append ( " >%2</td>" );
    _table_row_mask.append ( "</tr>" );
  }

  _title_open_mixer_fail = tr ( "Mixer device couldn't be opened" );
  _title_no_device = tr ( "No device selected" );

  // Layout
  {
    QVBoxLayout * lay_vbox ( new QVBoxLayout );
    lay_vbox->addWidget ( &_title );
    lay_vbox->addSpacing ( fontMetrics ().height () / 2 );
    lay_vbox->addWidget ( &_text );
    lay_vbox->addStretch ();
    setLayout ( lay_vbox );
  }
}

void
Message_Widget::set_title ( const QString & txt_n )
{
  _title.setText ( _title_mask.arg ( txt_n ) );
}

void
Message_Widget::set_text ( const QString & txt_n )
{
  _text.setText ( txt_n );
}

void
Message_Widget::set_mixer_open_fail ( const QString & device_n,
                                      const QString & error_n,
                                      const QString & func_n )
{
  set_title ( _title_open_mixer_fail );
  {
    QString txt;
    txt += "<table cellspacing=\"0\" cellpadding=\"0\"><tbody>";
    txt += _table_row_mask.arg ( tr ( "Function" ) + ":" ).arg ( func_n );
    txt += _table_row_mask.arg ( tr ( "Address" ) + ":" ).arg ( device_n );
    txt += _table_row_mask.arg ( tr ( "Error" ) + ":" ).arg ( error_n );
    txt += "<tbody><table>";

    set_text ( txt );
  }
}

void
Message_Widget::set_no_device ()
{
  set_title ( _title_no_device );
  set_text ( QString () );
}

} // namespace Views
