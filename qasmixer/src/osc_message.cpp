#include "osc_message.hpp"

Osc::Osc_Message::Osc_Message ( QNetworkDatagram * datagram )
{
  this->address = QString ( datagram->data ().constData () );

  this->sourceAddress = datagram->senderAddress ();
  this->sourcePort = datagram->senderPort ();

  this->destinationAddress = datagram->destinationAddress ();
  this->destinationPort = datagram->destinationPort ();

  qsizetype format_start = datagram->data ().indexOf ( ',' );
  qsizetype format_end = datagram->data ().indexOf ( '\0', format_start );

  qsizetype value_marker = format_end + 4 - format_end % 4;

  for ( int i = format_start + 1; i < format_end; i++ ) {
    char currentFormat = datagram->data ().at ( i );

    switch ( currentFormat ) {
    case 'i': {
      qint32 value = 0;

      QByteArray slice =
          datagram->data ().sliced ( value_marker, sizeof ( value ) );

      QDataStream ( slice ) >> value;

      this->values.append ( value );

      value_marker += sizeof ( value );
    } break;
    case 'f': {
      float value = 0.0f;

      QByteArray slice =
          datagram->data ().sliced ( value_marker, sizeof ( value ) );

      QDataStream stream ( slice );
      stream.setFloatingPointPrecision ( QDataStream::SinglePrecision );

      stream >> value;

      this->values.append ( value );

      value_marker += sizeof ( value );
    } break;
    case 's': {
      qsizetype string_length =
          datagram->data ().indexOf ( '\0', value_marker ) - value_marker;

      QByteArray slice =
          datagram->data ().sliced ( value_marker, string_length );

      QString value ( slice );

      this->values.append ( value );

      value_marker = value_marker + string_length + 4 - string_length % 4;
    } break;
    case 'b': {
      qint32 blob_size = 0;

      QByteArray slice =
          datagram->data ().sliced ( value_marker, sizeof ( blob_size ) );

      QDataStream ( slice ) >> blob_size;

      QByteArray blob = datagram->data ().sliced (
          value_marker + sizeof ( blob_size ), blob_size );

      qDebug () << "blob_size:" << blob_size;

      this->values.append ( blob );

      value_marker = value_marker + blob_size + 4 - blob_size % 4;
    } break;
    case 'T': {
      this->values.append ( true );
    } break;
    case 'F': {
      this->values.append ( false );
    } break;
    }
  }
}

Osc::Osc_Message::Osc_Message ( QHostAddress destinationAddress,
                                quint16 destinationPort,
                                QHostAddress sourceAddress,
                                quint16 sourcePort,
                                QString address,
                                QList< QVariant > values )
{
  this->destinationAddress = destinationAddress;
  this->destinationPort = destinationPort;
  this->sourceAddress = sourceAddress;
  this->sourcePort = sourcePort;

  this->address = address;
  this->values = values;
}

QString
Osc::Osc_Message::format ()
{
  QVariant element;
  QString format;

  foreach ( element, values ) {
    switch ( element.userType () ) {
    case ( QMetaType::Int ): {
      format.append ( 'i' );
    } break;
    case ( QMetaType::Float ): {
      format.append ( 'f' );
    } break;
    case ( QMetaType::QString ): {
      format.append ( 's' );
    } break;
    case ( QMetaType::QByteArray ): {
      format.append ( 'b' );
    } break;
    }
  }

  return format;
}

QByteArray
Osc::Osc_Message::toByteArray ()
{
  QByteArray data;
  {
    QByteArray address = this->address.toLatin1 ();
    address.append ( QChar::Null );

    while ( address.length () % 4 > 0 ) {
      address.append ( QChar::Null );
    }

    data.append ( address );
  }
  {
    QByteArray format;

    format.append ( ',' );
    format.append ( this->format ().toLatin1 () );
    format.append ( QChar::Null );

    while ( format.length () % 4 > 0 ) {
      format.append ( QChar::Null );
    };

    data.append ( format );
  }

  QVariant element;

  foreach ( element, values ) {
    switch ( element.userType () ) {
    case ( QMetaType::Int ): {
      QByteArray int_value;
      QDataStream stream ( &int_value, QIODevice::WriteOnly );
      stream << element.toInt ();
      data.append ( int_value );
    } break;
    case ( QMetaType::Float ): {
      QByteArray float_value;
      QDataStream stream ( &float_value, QIODevice::WriteOnly );
      stream.setFloatingPointPrecision ( QDataStream::SinglePrecision );
      stream << element.toFloat ();
      data.append ( float_value );
    } break;
    case ( QMetaType::QString ): {
      QByteArray string_value = element.toString ().toLatin1 ();
      string_value.append ( QChar::Null );

      while ( string_value.length () % 4 > 0 ) {
        string_value.append ( QChar::Null );
      };

      data.append ( string_value );
    } break;
    case ( QMetaType::QByteArray ): {
      QByteArray blob_value = element.toByteArray ();
      blob_value.append ( QChar::Null );

      while ( blob_value.length () % 4 > 0 ) {
        blob_value.append ( QChar::Null );
      };

      data.append ( blob_value );
    } break;
    }
  }

  return data;
}