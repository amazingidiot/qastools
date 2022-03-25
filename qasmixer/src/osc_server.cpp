#include "osc_server.hpp"
#include "qsnd/card_info.hpp"
#include "qsnd/controls_database.hpp"
#include "qsnd/controls_model.hpp"
#include <QDebug>
#include <memory>

Osc::Server::Server ()
{
  _subscription_timer.setInterval ( 1000 * 30 );

  _subscription_timer.connect (
      &_subscription_timer, &QTimer::timeout, this, [ = ] () {
        for ( qsizetype i = 0; i < _clients.size (); ++i ) {
          qInfo () << _clients.at ( i ).address;
        }
      } );

  _subscription_timer.start ();

  _socket = new QUdpSocket ( this );

  connect ( _socket, &QUdpSocket::readyRead, this, &Server::receiveDatagram );

  _cards_model = new QSnd::Cards_Model ( this );

  for ( uint16_t i = 0; i < _cards_model->count (); i++ ) {
    ::QSnd::Mixer_Simple * mixer = new ::QSnd::Mixer_Simple ();

    mixer->open ( QString ( "hw:%1" ).arg ( i ) );

    qInfo () << "Mixer" << i << "is_open:" << mixer->is_open ();

    for ( uint16_t j = 0; j < mixer->num_elems (); j++ ) {
      ::QSnd::Mixer_Simple_Elem * elem = mixer->elem ( j );
      qInfo () << "Mixerelement" << j << ":" << elem->display_name ();
      if ( elem->has_volume ( 0 ) && elem->has_dB ( 0 ) ) {
        qInfo () << "Playback volume:"
                 << ( 100 * elem->volume ( 0, 0 ) /
                      ( elem->volume_max ( 0 ) - elem->volume_min ( 0 ) ) )
                 << "%";
      }
    }
    _mixers[ i ] = mixer;
  }

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
      QRegularExpression ( "^/subscribe$" ),
      [] ( Osc::Server * server, Osc::Message * message ) {
        // Find client and resubscribe it in server->_clients

        Osc::Message subscribe_reply ( message->address, QList< QVariant > () );

        subscribe_reply.destinationAddress = message->sourceAddress;
        subscribe_reply.destinationPort = message->sourcePort;

        server->sendOscMessage ( &subscribe_reply );
        return true;
      } ) );

  actions.append ( Osc::Action (
      QRegularExpression ( "^/alsa/get_devices$" ),
      [] ( Osc::Server * server, Osc::Message * message ) {
        qInfo () << "Received /alsa/get_devices";

        QList< QVariant > values;

        for ( int i = 0; i < server->_cards_model->count (); i++ ) {
          values.append ( server->_cards_model
                              ->card_info_by_model_index (
                                  server->_cards_model->index ( i, 0 ) )
                              ->index () );
        }

        Osc::Message response ( "/alsa/get_devices", values );

        response.destinationAddress = message->sourceAddress;
        response.destinationPort = message->sourcePort;

        server->sendOscMessage ( &response );

        return true;
      } ) );

  actions.append ( Osc::Action (
      QRegularExpression ( "^/alsa/get_device$" ),
      [] ( Osc::Server * server, Osc::Message * message ) {
        if ( message->values.count () != 1 ) {
          qInfo () << "Invalid argument count for '/alsa/get_device':"
                   << message->values.count ();

          return false;
        }

        bool device_index_valid = false;

        uint32_t device_index =
            message->values[ 0 ].toInt ( &device_index_valid );

        if ( !device_index_valid ) {
          qInfo () << device_index << "is not a valid index";
          return false;
        }

        QList< QVariant > values;

        auto card_info =
            server->_cards_model->card_info_by_card_index ( device_index );

        if ( card_info == 0 ) {
          qInfo () << "Could not get card_info for index" << device_index;

          return true;
        }

        values.append ( device_index );

        values.append (
            server->_cards_model->card_info_by_card_index ( device_index )
                ->index () );
        values.append (
            server->_cards_model->card_info_by_card_index ( device_index )
                ->id () );
        values.append (
            server->_cards_model->card_info_by_card_index ( device_index )
                ->name () );
        values.append (
            server->_cards_model->card_info_by_card_index ( device_index )
                ->mixer_name () );
        values.append (
            server->_cards_model->card_info_by_card_index ( device_index )
                ->long_name () );
        values.append (
            server->_cards_model->card_info_by_card_index ( device_index )
                ->driver () );
        values.append (
            server->_cards_model->card_info_by_card_index ( device_index )
                ->components () );

        Osc::Message response ( "/alsa/get_device", values );

        response.destinationAddress = message->sourceAddress;
        response.destinationPort = message->sourcePort;

        server->sendOscMessage ( &response );

        return true;
      } ) );

  actions.append ( Osc::Action (
      QRegularExpression ( "^/alsa/get_mixer_element_count$" ),
      [] ( Osc::Server * server, Osc::Message * message ) {
        if ( message->values.count () != 1 ) {
          qInfo ()
              << "Invalid argument count for '/alsa/get_mixer_element_count':"
              << message->values.count ();

          return false;
        }
        // get device index from arg 0
        bool device_index_valid = false;

        uint32_t device_index =
            message->values[ 0 ].toInt ( &device_index_valid );

        if ( !device_index_valid ) {
          qInfo () << device_index << "is not a valid index";
          return false;
        }

        ::QSnd::Mixer_Simple * mixer = server->get_mixer ( device_index );

        if ( mixer == nullptr ) {
          return false;
        }

        QList< QVariant > values;

        values.append ( device_index );

        values.append ( static_cast< int32_t > ( mixer->num_elems () ) );

        Osc::Message response ( "/alsa/get_mixer_element_count", values );

        response.destinationAddress = message->sourceAddress;
        response.destinationPort = message->sourcePort;

        server->sendOscMessage ( &response );

        return true;
      } ) );
  actions.append ( Osc::Action (
      QRegularExpression ( "^/alsa/get_mixer_element$" ),
      [] ( Osc::Server * server, Osc::Message * message ) {
        if ( message->values.count () != 2 ) {
          qInfo () << "Invalid argument count for '/alsa/get_mixer_element':"
                   << message->values.count ();

          return false;
        }
        // get device index from arg 0
        bool device_index_valid = false;

        uint32_t device_index =
            message->values[ 0 ].toInt ( &device_index_valid );

        if ( !device_index_valid ) {
          qInfo () << device_index << "is not a valid index";
          return false;
        }

        bool element_index_valid = false;

        uint32_t element_index =
            message->values[ 1 ].toInt ( &element_index_valid );

        if ( !element_index_valid ) {
          qInfo () << element_index << "is not a valid index";

          return false;
        }

        ::QSnd::Mixer_Simple * mixer = server->get_mixer ( device_index );

        if ( mixer == nullptr ) {
          return false;
        }

        QList< QVariant > values;

        QSnd::Mixer_Simple_Elem * elem = mixer->elem ( element_index );

        if ( elem == nullptr ) {
          qInfo () << "Failed to get mixer element" << element_index;

          return false;
        }

        values.append ( device_index );
        values.append ( element_index );
        values.append ( elem->elem_index () );
        values.append ( elem->elem_name () );
        values.append ( elem->has_volume ( 0 ) );
        values.append ( elem->has_volume ( 1 ) );
        values.append ( elem->has_dB ( 0 ) );
        values.append ( elem->has_dB ( 1 ) );
        values.append ( elem->has_switch ( 0 ) );
        values.append ( elem->has_switch ( 1 ) );
        values.append ( elem->has_enum ( 0 ) );
        values.append ( elem->has_enum ( 1 ) );

        qInfo () << values;

        Osc::Message response ( "/alsa/get_mixer_element", values );

        response.destinationAddress = message->sourceAddress;
        response.destinationPort = message->sourcePort;

        server->sendOscMessage ( &response );

        return true;
      } ) );
  actions.append ( Osc::Action (
      QRegularExpression ( "^/alsa/get_mixer_element_volume$" ),
      [] ( Osc::Server * server, Osc::Message * message ) {
        if ( message->values.count () != 3 ) {
          qInfo ()
              << "Invalid argument count for '/alsa/get_mixer_element_count':"
              << message->values.count ();

          return false;
        }
        // get device index from arg 0
        bool device_index_valid = false;

        int32_t device_index =
            message->values[ 0 ].toInt ( &device_index_valid );

        if ( !device_index_valid ) {
          qInfo () << device_index << "is not a valid device index";
          return false;
        }

        // get element index from arg 1
        bool element_index_valid = false;

        int32_t element_index =
            message->values[ 1 ].toInt ( &element_index_valid );

        if ( !element_index_valid ) {
          qInfo () << element_index << "is not a valid element index";

          return false;
        }

        // get element direction from arg 2
        bool element_dir_valid = false;

        int32_t element_dir = message->values[ 2 ].toInt ( &element_dir_valid );

        if ( !element_dir_valid ) {
          qInfo () << element_dir << "is not a valid element direction";

          return false;
        }

        ::QSnd::Mixer_Simple * mixer = server->get_mixer ( device_index );

        if ( mixer == nullptr ) {
          return false;
        }

        QList< QVariant > values;

        QSnd::Mixer_Simple_Elem * elem = mixer->elem ( element_index );

        if ( elem == nullptr ) {
          qInfo () << "Failed to get mixer element" << element_index;

          return false;
        }

        values.append ( device_index );
        values.append ( element_index );
        values.append ( element_dir );

        values.append (
            static_cast< int32_t > ( elem->volume ( 0, element_dir ) ) );
        values.append (
            static_cast< int32_t > ( elem->volume_min ( element_dir ) ) );
        values.append (
            static_cast< int32_t > ( elem->volume_max ( element_dir ) ) );

        Osc::Message response ( "/alsa/get_mixer_element_volume", values );

        response.destinationAddress = message->sourceAddress;
        response.destinationPort = message->sourcePort;

        server->sendOscMessage ( &response );

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

QSnd::Mixer_Simple *
Osc::Server::get_mixer ( int32_t index )
{
  ::QSnd::Mixer_Simple * mixer = _mixers[ index ];

  if ( mixer == nullptr ) {
    qInfo () << "Mixer for device" << index
             << "not yet opened, attempting to open.";

    mixer = new QSnd::Mixer_Simple ();

    mixer->open ( QString ( "hw:%1" ).arg ( index ) );

    if ( !mixer->is_open () ) {
      qInfo () << "Could not open mixer for device" << index;
      return nullptr;
    }

    _mixers[ index ] = mixer;

    qInfo () << "Mixer for device" << index << "successfully opened";
  }

  return mixer;
}