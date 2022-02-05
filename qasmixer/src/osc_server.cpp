#include "osc_server.hpp"
#include <QDebug>
#include <memory>

Osc::Server::Server ()
{
  _socket = new QUdpSocket ( this );

  connect ( _socket, &QUdpSocket::readyRead, this, &Server::receiveDatagram );

  _cards_model = new QSnd::Cards_Model ( this );

  actions.append ( Osc::Action (
      QRegularExpression ( "/echo" ),
      [] ( Osc::Server * server, Osc::Message * message ) {
        Osc::Message echo_reply ( message->address, message->values );

        echo_reply.destinationAddress = message->sourceAddress;
        echo_reply.destinationPort = message->sourcePort;

        server->sendOscMessage ( &echo_reply );
        return true;
      } ) );
  actions.append ( Osc::Action (
      QRegularExpression ( "/alsa/device$" ),
      [] ( Osc::Server * server, Osc::Message * message ) {
        for ( int i = 0; i < server->_cards_model->count (); i++ ) {
          QList< QVariant > values;

          values.append (
              server->_cards_model->card_info_by_card_index ( i )->index () );
          values.append (
              server->_cards_model->card_info_by_card_index ( i )->id () );
          values.append (
              server->_cards_model->card_info_by_card_index ( i )->name () );
          values.append ( server->_cards_model->card_info_by_card_index ( i )
                              ->mixer_name () );
          values.append ( server->_cards_model->card_info_by_card_index ( i )
                              ->long_name () );
          values.append (
              server->_cards_model->card_info_by_card_index ( i )->driver () );
          values.append ( server->_cards_model->card_info_by_card_index ( i )
                              ->components () );

          Osc::Message response ( "/alsa/device", values );

          response.destinationAddress = message->sourceAddress;
          response.destinationPort = message->sourcePort;

          server->sendOscMessage ( &response );
        }

        return true;
      } ) );
}

Osc::Server::~Server ()
{
  _socket->close ();

  _socket->deleteLater ();
}

bool
Osc::Server::enabled ()
{
  return _enabled;
}

void
Osc::Server::setEnabled ( bool value )
{
  if ( value != _enabled ) {
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
Osc::Server::port ()
{
  return _port;
}

void
Osc::Server::setPort ( quint16 value )
{
  if ( value != _port ) {
    _port = value;

    emit portChanged ( _port );
  }
}

void
Osc::Server::receiveDatagram ()
{
  while ( _socket->hasPendingDatagrams () ) {
    QNetworkDatagram datagram = _socket->receiveDatagram ();

    std::unique_ptr< Osc::Message > received_message (
        new Osc::Message ( &datagram ) );

    foreach ( auto & action, actions ) {
      if ( action.address.match ( received_message.get ()->address )
               .hasMatch () ) {
        action.run ( this, received_message.get () );
      }
    }
  }
}

void
Osc::Server::sendOscMessage ( Osc::Message * message )
{
  QNetworkDatagram datagram;

  datagram.setSender ( message->sourceAddress, message->sourcePort );
  datagram.setDestination ( message->destinationAddress,
                            message->destinationPort );

  datagram.setData ( message->toByteArray () );

  _socket->writeDatagram ( datagram );
}