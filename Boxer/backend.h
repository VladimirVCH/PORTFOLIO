#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QDataStream>
#include <QTimer>
#include <QEventLoop>
#include <QFile>
#include <QTime>
#include <QDir>
#include "server.h"
#include "client.h"

class Backend : public QObject
{
  Q_OBJECT

  Q_PROPERTY(bool attack         READ getAttackStatus                      NOTIFY attackStatusChanged)
  Q_PROPERTY(bool block          READ getBlockStatus                       NOTIFY blockStatusChanged)
  Q_PROPERTY(QString _text       READ getText              WRITE setText   NOTIFY textStatusChanged)
  Q_PROPERTY(QString namePlayer  READ getOtherPlayerName                   NOTIFY newPlayer)

public:   
  Backend();
  ~Backend() {};

  bool getAttackStatus() const
  {
    return m_attack;
  }

  bool getBlockStatus() const
  {
    return m_block;
  }

  void changeAttackMode(
      bool _attack,
      bool _block
    );

  QString getText() const
  {
    return m_text;
  }

  QString getOtherPlayerName() const
  {
    return m_otherPlayer;
  };

  void youWin();

  void logFileWrite(QString);

private:  
  bool                  m_attack,
                         m_block,
                      m_isServer;

  QString                 m_host,
                          m_text,
                   m_otherPlayer,
                     m_wait_text = "Waiting for another player",
                      m_log_file =                    "log.txt";

  uint16_t                m_port;

  QChar                   m_side,
               m_otherPlayerSide,
              m_otherPlayerBlock;

  int                      m_hit,
                m_otherPlayerHit,
                        m_rnd_hit = 30 , // лимиты удара
                        m_rnd_min = 5  ; // лимиты удара

  QEventLoop              m_loop;

  QString hitBlocked();

  QString hitResulted();

  QString blockWrong();

  QString blockCorrect();

public slots:
  void setHostPort(
      QString _host = "127.0.0.1",
      int _port     =     1234
    );

  void serverClicked();

  void endConnection(
      QString
    );

  void endConnGUI()
  {
    emit endConn();
  };

  void serverSend(
      QString _msg
    )
  {
    emit serverMsg(_msg);
  };

  void newEvent(
      QString _msg
    );

  void clientClicked();

  void clientSend(
      QString _msg
    )
  {
    emit clientMsg(_msg);
  };

  void hitLeft();

  void hitRight();

  void blockLeft();

  void blockRight();

  bool isServer()
  {
    return m_isServer;
  };

  void setText(
      QString _text
    );

  void lostGame();

  QByteArray logFileRead();

signals:
  void gotConn();

  void endConn();

  void clientDisconnected();

  void serverMsg(
      QString _msg
    );

  void clientMsg(
      QString _msg
    );

  void changeHealth(
      int _health
    );

  void newHitReceived();

  void newBlockReceived();

  void attackStatusChanged(
      bool _attack
    );

  void blockStatusChanged(
      bool _block
    );

  void textStatusChanged(
      QString _text
    );

  void newPlayer(
      QString _namePlayer
    );

  void win();

  void lost();

  void stopLoop();
};

#endif // BACKEND_H
