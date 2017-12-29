#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include <QObject>
#include <QTcpSocket>
#include "NetworkConstants.h"
#include <QByteArray>
#include <QDataStream>
#include <QPoint>

class NetworkInterface : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* socket;
    BlockSize block_size;


    void sendEasyCommand(const NetworkCommands& command);
public:
    NetworkInterface(QObject *parent = 0);
    const QString getSocketErrorString();
    ~NetworkInterface();

signals:
    void connected();
    void authenticatedStatus(const AuthStatuses& auth_status);
    void disconnected();
    void error(const QAbstractSocket::SocketError&);
    void usersListIsDelivered(const QStringList& users_list);

    void newUserOnServer(const QString& username);
    void userIsRemovedFromServer(const QString& username);

    void inviteStatus(const InviteStatuses& invite_status);
    void inviteRequest(const QString& initiator);

    void closeInvite();
    void closeGame(const CloseGameStatuses& status);

    void coordsIsDelivered(const QPoint& coords);

    void answerOnGameStep(const GameStepAnswers& answer);

    void broadcastMessageIsDelivered(const QString& sender, const QString& message);
    void privateMessageIsDelivered(const QString& sender, const QString& message);
    void sessionMessageIsDelivered(const QString& sender, const QString& message);
public slots:
    void connectToHost(const QString& host, const quint16& port);
    void connectToHost(const QHostAddress& host, const quint16& port);
    void disconnectFromHost();
    void authenticate(const QString& login);

    void getUsersList();

    void invite(const QString& parthner_username);
    void sendInviteAnswer(const InviteStatuses& invite_answer);

    void revertInvite();

    void sendCoords(const QPoint& coords);

    void sendGameResult(const CloseGameStatuses& status);

    void sendAnswerOnGameStep(const GameStepAnswers& answer);

    void sendBroadcastMessage(const QString& message);
    void sendPrivateMessage(const QString& receiver, const QString& message);
    void sendSessionMessage(const QString& message);
private slots:
    void slotReadDataFromServer();
};

#endif
