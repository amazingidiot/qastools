#include "osc_server.hpp"
#include "QDebug"

Osc::Osc_Server::Osc_Server ()
{
  qDebug () << "Created Osc_Server instance";

  _socket = new QUdpSocket ( this );

  connect (
      _socket, &QUdpSocket::readyRead, this, &Osc_Server::receiveDatagram );

  _cards_model = new QSnd::Cards_Model ( this );
}

Osc::Osc_Server::~Osc_Server ()
{
  qDebug () << "Destroyed Osc_Server instance";

  _socket->close ();

  _socket->deleteLater ();
}

bool
Osc::Osc_Server::enabled ()
{
  return _enabled;
}

void
Osc::Osc_Server::setEnabled ( bool value )
{
  if ( value != _enabled ) {
    qDebug () << "Set osc_server enabled to" << value;

    _enabled = value;

    if ( _enabled ) {
      _socket->bind ( QHostAddress::LocalHost, _port );
    } else {
      _socket->close ();
    }
    emit enabledChanged ( value );
  }
}

quint16
Osc::Osc_Server::port ()
{
  return _port;
}

void
Osc::Osc_Server::setPort ( quint16 value )
{
  if ( value != _port ) {
    qDebug () << "Port changed!";

    _port = value;

    emit portChanged ( _port );
  }
}

void
Osc::Osc_Server::receiveDatagram ()
{
  while ( _socket->hasPendingDatagrams () ) {
    QNetworkDatagram datagram = _socket->receiveDatagram ();
    qDebug () << "Received" << datagram.data ().length () << "bytes from"
              << datagram.senderAddress ();
  }
}

void
Osc::Osc_Server::sendDatagram ( Osc::Osc_Message * message )
{
  QNetworkDatagram datagram;

  datagram.setSender ( message->sourceAddress, message->sourcePort );
  datagram.setDestination ( message->destinationAddress,
                            message->destinationPort );

  QByteArray data;

  data.append ( message->address.toUtf8 () );
  data.append ( ',' );
  data.append ( message->format.toUtf8 () );

  for ( int i = 0; i < message->values.count (); i++ ) {
    QVariant value = message->values.at ( i );

    switch ( value.type () ) {
    case ( QMetaType::Int ): {
      data.append ( value.toInt () );
    } break;
    case ( QMetaType::Float ): {
      data.append ( value.toFloat () );
    } break;
    case ( QMetaType::QString ): {
      QByteArray string_value = value.toString ().toLocal8Bit ();

      for ( int j = 0; i < ( 4 - string_value.length () % 4 ); i++ ) {
        string_value.append ( '0' );
      }

      data.append ( string_value );
    } break;
    case ( QMetaType::QByteArray ): {
      data.append ( value.toByteArray () );
    } break;
    }
  }

  _socket->writeDatagram ( datagram );
}
