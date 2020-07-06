#include "backend.h"
#include <QDebug>

Backend::Backend()
{
  m_attack              = false ;
  m_block               = false ;
  m_side                =  '0'  ;
  m_otherPlayerSide     =  '0'  ;
  m_otherPlayerBlock    =  '0'  ;
  m_otherPlayerHit      =   0   ;
  m_otherPlayer         =   ""  ;

  // настройка ожидания действий
  // backend работает в одном потоке с GUI - QEventLoop блокирует
  // работу GUI тоже в момент ожидания ответа от другого игрока
  connect(this, &Backend::newBlockReceived, &m_loop, &QEventLoop::quit);
  connect(this, &Backend::newHitReceived, &m_loop, &QEventLoop::quit);
  connect(this, &Backend::stopLoop, &m_loop, &QEventLoop::quit);
}

void Backend::setHostPort(
    QString _host,
    int _port
  )
{
    m_host = _host;
    m_port = _port;
}

// устанавливаем вариант кнопок атаки и блока и флаги тут
void Backend::changeAttackMode(
    bool _attack,
    bool _block
  )
{
  m_attack = _attack;
  m_block  =  _block;

  emit blockStatusChanged(m_block);

  emit attackStatusChanged(m_attack);
}

void Backend::lostGame()
{
  if(m_isServer)
    serverSend("Lost");

  else
    clientSend("Lost");

  if(m_loop.isRunning() != true)
    emit stopLoop();

  changeAttackMode(true, true);

  setText("Defeated...");

  emit lost();
}

void Backend::youWin()
{
  if(m_loop.isRunning() != true)
    emit stopLoop();

  changeAttackMode(false, false);

  setText("You WIN!!!");

  emit win();
}

// запись в лог-файл
void Backend::logFileWrite(
    QString _msg
  )
{
  QFile file(m_log_file);

  QTime ct = QTime::currentTime();

  QByteArray block;

  block.append(ct.toString());

  block.append(" : ");

  block.append(_msg.toUtf8());

  block.append("\n");

  if (file.open(QIODevice::Append))
  {
    file.write(block);

    file.close();
  }
}

// чтение из лог-файла
QByteArray Backend::logFileRead()
{
  QFile file(m_log_file);

  QByteArray block;

  if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
  {
    block = file.readAll();

    file.close();
  }

  return block;
}

// вариант работы - сервер
void Backend::serverClicked()
{
  QThread *backendTR = new QThread;

  Server *server = new Server(m_host, m_port);

  server->moveToThread(backendTR);

  // удаление после окончания потока
  connect(backendTR, &QThread::finished, server, &Server::deleteLater);
  connect(backendTR, &QThread::finished, backendTR, &QThread::deleteLater);

  // сигнал на GUI клиент подключился
  connect(server, &Server::newConn, this, [=]()
                                          {
                                            emit gotConn();
                                          }
                                          );

  // сигнал на GUI клиент отсоединился и удаление потока и сервера
  connect(server, &Server::endConn, this, [=](QString _text)
                                          {
                                            endConnection(_text);
                                            emit clientDisconnected();
                                          }
                                          );

  // отключаемся по сигналу из GUI или от сервера
  connect(this, &Backend::endConn, backendTR, &QThread::quit);

  // получаем имя клиента
  connect(server, &Server::clientName, this, &Backend::setText);

  // отправка сообщения сервер
  connect(this, &Backend::serverMsg, server, &Server::sendMsg);

  // читаем блок и удар
  connect(server, &Server::newEvent, this, &Backend::newEvent);

  backendTR->start();

  changeAttackMode(true, false);

  m_isServer = true;
}

// отключение
void Backend::endConnection(
    QString _text
  )
{
  m_otherPlayer = "---";

  setText("Connection issue " + _text);

  changeAttackMode(false, false);

  m_side                = '0' ;
  m_otherPlayerSide     = '0' ;
  m_otherPlayerBlock    = '0' ;
  m_otherPlayerHit      =  0  ;
  m_otherPlayer         = ""  ;
  m_text                = ""  ;

  emit endConn();
}

// вариант работы - клиент
void Backend::clientClicked()
{

  Client *m_client = new Client(m_host, m_port);

  QThread *clientThread = new QThread;

  m_client->moveToThread(clientThread);

  // удаление после окончания потока
  connect(clientThread, &QThread::finished, m_client, &Client::deleteLater);
  connect(clientThread, &QThread::finished, clientThread, &QThread::deleteLater);

  // получаем имя сервера
  connect(m_client, &Client::serverName, this, &Backend::setText);

  // посылаем сообщение
  connect(this, &Backend::clientMsg, m_client, &Client::sendMsg);

  // читаем блок и удар
  connect(m_client, &Client::newEvent, this, &Backend::newEvent);

  // сигнал на GUI клиент отсоединился
  connect(m_client, &Client::disconnected, this, [=](QString _text)
                                                 {
                                                   endConnection(_text);
                                                   emit clientDisconnected();
                                                 }
                                                 );

  // отключаемся по сигналу из GUI или от сервера
  connect(this, &Backend::endConn, clientThread, &QThread::quit);

  clientThread->start();

  changeAttackMode(false, true);

  m_isServer = false;
}

// общие методы для 2х вариантов работы

// обрабртка нового события (сообщения)
void Backend::newEvent(
    QString _msg
  )
{
  // если пришел проигрыш от другого игрока
  if(_msg == "Lost")
    youWin();

  if(_msg.length() == 1)
  {
    m_otherPlayerBlock = _msg[0];

    emit newBlockReceived();
  }
  else
  {
    m_otherPlayerSide = _msg[0];

    _msg.remove(0, 1);

    m_otherPlayerHit = _msg.toInt();

    emit newHitReceived();
  }
}

// удар левой
void Backend::hitLeft()
{
  m_side = 'L';

  m_hit = rand() % m_rnd_hit + m_rnd_min;

  QString msg = m_side + QString::number(m_hit);

  if(m_isServer)
    serverSend(msg);

  else
    clientSend(msg);

  // ждем пока придет ответ с блоком
  if(m_otherPlayerBlock == '0')
  {
    setText(m_wait_text);

    m_loop.exec();
  }

  // проверка стороны блока другого игрока
  if(m_otherPlayerBlock == m_side)
    setText(hitBlocked());

  else
    setText(hitResulted());

  m_otherPlayerBlock = '0';

  changeAttackMode(!m_attack, !m_block);
}

// удар правой
void Backend::hitRight()
{
  m_side = 'R';

  m_hit = rand() % m_rnd_hit + m_rnd_min;

  QString msg = m_side + QString::number(m_hit);

  if(m_isServer)
    serverSend(msg);

  else
    clientSend(msg);

  // ждем пока придет ответ с блоком
  if(m_otherPlayerBlock == '0')
  {
    setText(m_wait_text);

    m_loop.exec();
  }

  // проверка стороны блока другого игрока
  if(m_otherPlayerBlock == m_side)
    setText(hitBlocked());

  else
    setText(hitResulted());

  m_otherPlayerBlock = '0';

  changeAttackMode(!m_attack, !m_block);
}

//левый блок
void Backend::blockLeft()
{
  m_side = 'L';

  QString msg(m_side);

  if(m_isServer)
    serverSend(msg);

  else
    clientSend(msg);

  // ждем пока придет ответ с ударом
  if(m_otherPlayerSide == '0')
  {
    setText(m_wait_text);

    m_loop.exec();
  }

  // проверка стороны своего блока
  if(m_otherPlayerSide == m_side)
    setText(blockCorrect());

  else
  {
    setText(blockWrong());

    emit changeHealth(m_otherPlayerHit);
  }

  m_otherPlayerSide = '0';

  changeAttackMode(!m_attack, !m_block);
}

// правый блок
void Backend::blockRight()
{
  m_side = 'R';

  QString msg(m_side);

  if(m_isServer)
    serverSend(msg);

  else
    clientSend(msg);

  // ждем пока придет ответ с ударом
  if(m_otherPlayerSide == '0')
  {
    setText(m_wait_text);

    m_loop.exec();
  }

  // проверка стороны своего блока
  if(m_otherPlayerSide == m_side)
    setText(blockCorrect());
  else
  {
    setText(blockWrong());

    emit changeHealth(m_otherPlayerHit);
  }

  m_otherPlayerSide = '0';

  changeAttackMode(!m_attack, !m_block);
}

QString Backend::hitBlocked()
{
  return "Your hit is blocked. Prepare for defence";
}

QString Backend::hitResulted()
{
  return "Your hit "
         + QString::number(m_hit)
          + " points is succesfull. Prepare for defence";
}

QString Backend::blockWrong()
{
  return "You missed hit for "
         + QString::number(m_otherPlayerHit)
          + " points. Your turn";
}

QString Backend::blockCorrect()
{
  return "Player's hit is blocked. Your turn";
}

//через эту функцию забрасываем все текстовые сообщения в форму на 5 стр
void Backend::setText(
    QString _text
  )
{
  // присваиваем имя второго игрока и передам в форму
  if (m_otherPlayer == "")
  {
    m_otherPlayer = _text;

    _text += " is connected";

    emit newPlayer(m_otherPlayer);
  }

  if (m_text == _text)
    return;

  m_text = _text;

  // пишем все события в лог
  logFileWrite(m_text);

  emit textStatusChanged(m_text);
}
