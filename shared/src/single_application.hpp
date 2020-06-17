/// QasTools: Desktop toolset for the Linux sound system ALSA.
/// \copyright See COPYING file.

#ifndef __INC_single_application_hpp__
#define __INC_single_application_hpp__

#include <QApplication>
#include <QLinkedList>
#include <QLocalServer>
#include <QPointer>

class Single_Application : public QApplication
{
  Q_OBJECT

  // Public methods
  public:
  Single_Application ( int & argc,
                       char * argv[],
                       const QString & unique_key_n = QString () );

  ~Single_Application ();

  // Unique key

  const QString &
  unique_key () const;

  bool
  set_unique_key ( const QString & unique_key_n );

  bool
  is_running () const;

  // Message

  bool
  send_message ( const QString & msg_n );

  const QString
  latest_message () const;

  // Session management

  void
  commitData ( QSessionManager & manager_n );

  void
  saveState ( QSessionManager & manager_n );

  // Signals
  signals:

  void
  sig_message_available ( QString mesg_n );

  // Protected slots
  protected slots:

  void
  new_client ();

  void
  read_clients_data ();

  void
  clear_dead_clients ();

  // Protected methods
  protected:
  void
  publish_message ( QByteArray & data_n );

  // Private attributes
  private:
  bool _is_running;
  QString _unique_key;
  QString _com_key;
  QString _com_file;
  QString _latest_message;

  QLocalServer * _local_server;

  struct Client
  {
    QPointer< QLocalSocket > socket;
    QByteArray data;
  };

  QLinkedList< Client > _clients;

  const unsigned int _timeout;
};

inline bool
Single_Application::is_running () const
{
  return _is_running;
}

inline const QString &
Single_Application::unique_key () const
{
  return _unique_key;
}

inline const QString
Single_Application::latest_message () const
{
  return _latest_message;
}

#endif
