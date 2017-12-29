#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <utility>
#include "NetworkConstants.h"
#include "authmanager.h"
#include <QMessageBox>
#include <QDebug>
#include <functional>
#include "sessionsmanager.h"
#include <QPoint>

namespace Ui {
class Widget;
}

class Server : public QWidget
{
    Q_OBJECT
private:
    QTcpServer* server;
    BlockSize block_size;
    AuthManager auth_manager;
    SessionsManager sessions_manager;
    void clearSocket(QTcpSocket* const socket);
    void authenticate(QTcpSocket* const client, const QString &login);

    void handleCloseInvite(QTcpSocket* const sender);
    void handleDisconnect(QTcpSocket* const client);

    void sendEasyCommand(QTcpSocket* const receiver, const NetworkCommands& command);

    void handleDeliveredCoords(QTcpSocket* const sender, const QPoint& coords);

    void handleCloseGame(QTcpSocket* const sender, const CloseGameStatuses& status);

    void handleGameStepAnswer(QTcpSocket* const sender, const GameStepAnswers& answer);

    void handleBroadcastMessage(QTcpSocket* const sender, const QString& message);
    void handlePrivateMessage(QTcpSocket* const sender, const QString& receiver, const QString& message);
    void handleSessionMessage(QTcpSocket* const sender, const QString& message);
public:
    explicit Server(const quint16& port, QWidget *parent = 0);
    ~Server();
private slots:
    void slotRead();
    void slotNewConnection();
    void slotDisconnected();
    void slotError();

    void slotSendAuthRequest(QTcpSocket* const client, const AuthStatuses& auth_status);

    void slotSendUsernames(QTcpSocket* const client);
    void slotSendBroadcastAddNewUser(const QString& username);
    void slotSendBroadcastUserIsRemoved(const QString& username);

    void slotSendInviteStatus(QTcpSocket* const client, const InviteStatuses& invite_status);

    void slotInviteHandler(QTcpSocket* const sender, QTcpSocket* const receiver);
    void slotSendInviteRequest(QTcpSocket* const receiver, const QString& sender_username);
    void slotHandleInviteAnswer(QTcpSocket* const sender, const InviteStatuses& invite_answer);

    void slotSendCloseInvite(QTcpSocket* const receiver);

    void slotSendCoords(QTcpSocket* const receiver, const QPoint& coords);

    void slotSendCloseGame(QTcpSocket* const receiver, const CloseGameStatuses& status);

    void slotSendGameStepAnswer(QTcpSocket* const receiver, const GameStepAnswers& answer);

    void slotSendBroadcastMessageTo(QTcpSocket* const receiver, const QString& sender, const QString& message);
    void slotSendPrivateMessageTo(QTcpSocket* const receiver, const QString& sender, const QString& message);
    void slotSendSessionMessageTo(QTcpSocket* const receiver, const QString& sender, const QString& message);
private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
