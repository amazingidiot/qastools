/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#include "single_application.hpp"
#include "unix_signal_handler.hpp"
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <QFile>
#include <QLocalSocket>
#include <QRegExp>
#include <QSessionManager>
#include <iostream>
#include <sstream>

Single_Application::Single_Application ( int & argc,
                                         char * argv[],
                                         const QString & unique_key_n )
: QApplication ( argc, argv )
, _is_running ( false )
, _local_server ( 0 )
, _timeout ( 2000 )
{
  // Unix signal handling
  {
    Unix_Signal_Handler::init_unix_signal_handlers ();

    Unix_Signal_Handler * usigh ( new Unix_Signal_Handler ( this ) );

    QObject::connect ( usigh, SIGNAL ( sig_int () ), this, SLOT ( quit () ) );

    QObject::connect ( usigh, SIGNAL ( sig_hup () ), this, SLOT ( quit () ) );

    QObject::connect ( usigh, SIGNAL ( sig_term () ), this, SLOT ( quit () ) );
  }

  if ( !unique_key_n.isEmpty () ) {
    set_unique_key ( unique_key_n );
  }
}

Single_Application::~Single_Application () {}

void
Single_Application::commitData ( QSessionManager & manager_n )
{
  //::std::cout << "Single_Application::commitData\n";
  emit commitDataRequest ( manager_n );
}

void
Single_Application::saveState ( QSessionManager & manager_n )
{
  //::std::cout << "Single_Application::saveState\n";
  manager_n.setRestartHint ( QSessionManager::RestartIfRunning );
  emit saveStateRequest ( manager_n );
}

bool
Single_Application::set_unique_key ( const QString & unique_key_n )
{
  if ( _unique_key == unique_key_n ) {
    return false;
  }

  // Clear
  {
    if ( _local_server != 0 ) {
      delete _local_server;
      _local_server = 0;
    }
    _is_running = false;
  }

  _unique_key = unique_key_n;
  _com_key = unique_key_n;

  if ( _com_key.isEmpty () ) {
    return _is_running;
  }

  // Adjust com key
  _com_key.replace ( "-", "_" );
  _com_key.replace ( ".", "_" );
  _com_key.replace ( QRegExp ( "[^A-Za-z0-9_]" ), QString () );
  {
    QString com_suff ( "_" );

    uid_t uid ( getuid () );
    struct passwd * pwd ( getpwuid ( uid ) );
    if ( pwd != 0 ) {
      com_suff.append ( QString::fromLocal8Bit ( pwd->pw_name ) );
    } else {
      com_suff.append ( "%1" );
      com_suff = com_suff.arg ( uid );
    }

    _com_key.append ( com_suff );
  }

  // Com socket file
  _com_file = "/tmp/";
  _com_file.append ( _com_key );

  {
    QFile com_fl ( _com_file );
    if ( com_fl.exists () ) {
      {
        // Try to connect
        QLocalSocket lsocket;
        lsocket.connectToServer ( _com_file, QIODevice::WriteOnly );
        _is_running = lsocket.waitForConnected ( _timeout );
      }
      if ( !_is_running ) {
        // Connection failed
        {
          ::std::stringstream msg;
          msg << "[WW] SApp: Existing socket does not reply.";
          msg << ::std::endl;
          msg << "[WW] SApp: Removing broken socket: ";
          msg << _com_file.toLocal8Bit ().constData ();
          msg << ::std::endl;
          ::std::cerr << msg.str ();
        }
        // Remove dead socket file
        if ( !com_fl.remove () ) {
          _is_running = true;
          {
            ::std::stringstream msg;
            msg << "[WW] Removing socket failed" << ::std::endl;
            ::std::cerr << msg.str ();
          }
        }
      }
    }
  }

  if ( !_is_running ) {
    // Create a local server and listen to incomming messages from other
    // instances
    _local_server = new QLocalServer ( this );
    if ( _local_server->listen ( _com_file ) ) {
      connect ( _local_server,
                SIGNAL ( newConnection () ),
                this,
                SLOT ( new_client () ) );
    } else {
      QString err ( _local_server->errorString () );
      if ( !err.isEmpty () ) {
        {
          ::std::stringstream msg;
          msg << "[EE] SApp: QLocalServer::listen" << ::std::endl;
          msg << "[EE] " << err.toLocal8Bit ().constData ();
          msg << ::std::endl;
          ::std::cerr << msg.str ();
        }
      }
      delete _local_server;
      _local_server = 0;
      _is_running = true;
    }
  }

  return _is_running;
}

void
Single_Application::clear_dead_clients ()
{
  //::std::cout << "Single_Application::clear_dead_clients\n";

  QLinkedList< Client >::iterator iter ( _clients.begin () );
  while ( iter != _clients.end () ) {
    bool remove ( false );

    Client & cln ( *iter );
    if ( cln.socket != 0 ) {
      if ( cln.socket->state () == QLocalSocket::UnconnectedState ) {
        delete cln.socket;
        publish_message ( cln.data );
        remove = true;
      }
    }

    if ( remove ) {
      //::std::cout << "Single_Application::clear_dead_clients - removing\n";
      iter = _clients.erase ( iter );
    } else {
      ++iter;
    }
  }
}

void
Single_Application::read_clients_data ()
{
  //::std::cout << "Single_Application::read_clients_data\n";

  QLinkedList< Client >::iterator iter ( _clients.begin () );
  while ( iter != _clients.end () ) {
    Client & cln ( *iter );
    if ( cln.socket != 0 ) {
      if ( cln.socket->bytesAvailable () > 0 ) {
        const int limit ( 1024 * 4 );
        cln.data.append ( cln.socket->read ( limit ) );
        if ( cln.data.size () >= limit ) {
          // Publish to avoid buffer size explosion
          publish_message ( cln.data );
        }
      }
    }
    ++iter;
  }
}

void
Single_Application::new_client ()
{
  QLocalSocket * lsocket ( _local_server->nextPendingConnection () );
  if ( lsocket != 0 ) {

    connect (
        lsocket, SIGNAL ( readyRead () ), this, SLOT ( read_clients_data () ) );

    connect ( lsocket,
              SIGNAL ( disconnected () ),
              this,
              SLOT ( clear_dead_clients () ),
              Qt::QueuedConnection );

    {
      Client cln;
      cln.socket = lsocket;
      _clients.append ( cln );
    }

    read_clients_data ();
  }
}

void
Single_Application::publish_message ( QByteArray & data_n )
{
  if ( data_n.size () > 0 ) {
    _latest_message = data_n.constData ();
    emit sig_message_available ( _latest_message );
    data_n.clear ();
  }
}

bool
Single_Application::send_message ( const QString & msg_n )
{
  if ( !_is_running || _com_file.isEmpty () ) {
    return false;
  }

  bool res ( false );

  QLocalSocket lsocket ( this );
  lsocket.connectToServer ( _com_file, QIODevice::WriteOnly );
  if ( lsocket.waitForConnected ( _timeout ) ) {
    lsocket.write ( msg_n.toUtf8 () );
    if ( lsocket.waitForBytesWritten ( _timeout ) ) {
      res = true;
    } else {
      QString err ( lsocket.errorString () );
      if ( !err.isEmpty () ) {
        {
          ::std::stringstream msg;
          msg << "[EE] SApp: send_message" << ::std::endl;
          msg << "[EE] " << err.toLocal8Bit ().constData ();
          msg << ::std::endl;
          ::std::cerr << msg.str ();
        }
      }
    }
    lsocket.disconnectFromServer ();
  } else {
    QString err ( lsocket.errorString () );
    if ( !err.isEmpty () ) {
      {
        ::std::stringstream msg;
        msg << "[EE] SApp: send_message" << ::std::endl;
        msg << "[EE] " << err.toLocal8Bit ().constData ();
        msg << ::std::endl;
        ::std::cerr << msg.str ();
      }
    }
  }

  return res;
}
