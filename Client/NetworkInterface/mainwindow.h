#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <NetworkInterface.h>
#include "invitewidget.h"
#include "gamebutton.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    NetworkInterface interface;
    InviteWidget* invite_widget;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
private slots:
    void slotConnectButtonClicked();
    void slotOnConnected();
    void slotOnDisconnected();

    void slotAuthStatusHandler(const AuthStatuses& auth_status);
    void slotAuthButtonClicked();

    void slotUsersListIsDelivered(QStringList users_list);

    void slotNewUser(const QString& username);
    void slotUserIsRemoved(const QString& username);

    void slotInviteButtonClicked();

    void slotInviteStatusHandler(const InviteStatuses& invite_status);

    void slotInviteRequestHandler(const QString& initiator);

    void slotAcceptHandler();
    void slotRejectHandler();

    void slotCloseInviteRequestHandler();

    void slotGameClose(const CloseGameStatuses& status);

    void slotGameButtonClicked();

    void slotCoordsIsDelivered(const QPoint& p);

    void slotWinButtonClicked();
    void slotLoseButtonClicked();

    void slotOnAnswerGameStep(const GameStepAnswers& answer);

    void slotOnHitButtonClicked();
    void slotOnMissButtonClicked();

    void slotOnBroadcastButtonClicked();
    void slotOnGetBroadcastMessage(const QString& sender, const QString& message);

    void slotOnPrivateMessageButtonClicked();
    void slotOnGetPrivateMessage(const QString& sender, const QString& message);

    void slotOnSessionMessageButtonClicked();
    void slotOnGetSessionMessage(const QString& sender, const QString& message);

};

#endif // MAINWINDOW_H
