#include "osc_server.hpp"
#include <QDebug>
#include <QScopedPointer>

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

    qDebug () << datagram.data ();

    QScopedPointer< Osc::Osc_Message > received_message (
        new Osc::Osc_Message ( &datagram ) );

    qDebug () << received_message.get ()->values;

    QScopedPointer< Osc::Osc_Message > message (
        new Osc::Osc_Message ( QHostAddress::LocalHost,
                               10023,
                               QHostAddress::LocalHost,
                               _port,
                               received_message.get ()->address,
                               received_message.get ()->values ) );

    sendOscMessage ( message.get () );
  }
}

void
Osc::Osc_Server::sendOscMessage ( Osc::Osc_Message * message )
{
  QNetworkDatagram datagram;

  datagram.setSender ( message->sourceAddress, message->sourcePort );
  datagram.setDestination ( message->destinationAddress,
                            message->destinationPort );

  datagram.setData ( message->toByteArray () );

  _socket->writeDatagram ( datagram );
}
