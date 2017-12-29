#include "testwindow.h"
#include "ui_TestWindow.h"
#include <QHostAddress>
#include <QDebug>

TestWindow::TestWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);
    invite_widget = new InviteWidget(this);
    invite_widget->hide();
    connect(ui->connectButton,SIGNAL(clicked(bool)),this,SLOT(slotConnectButtonClicked()));
    connect(ui->disconnectButton,SIGNAL(clicked(bool)),&interface,SLOT(disconnectFromHost()));
    connect(&interface,SIGNAL(connected()),this,SLOT(slotOnConnected()));
    connect(&interface,SIGNAL(disconnected()),this,SLOT(slotOnDisconnected()));
    connect(ui->authButton,SIGNAL(clicked(bool)),this,SLOT(slotAuthButtonClicked()));
    connect(&interface,SIGNAL(authenticatedStatus(const AuthStatuses&)),SLOT(slotAuthStatusHandler(const AuthStatuses&)));
    connect(&interface,SIGNAL(usersListIsDelivered(QStringList)),SLOT(slotUsersListIsDelivered(QStringList)));
    connect(ui->getUsersListButton,SIGNAL(clicked(bool)),&interface,SLOT(getUsersList()));

    connect(&interface,SIGNAL(newUserOnServer(const QString&)),SLOT(slotNewUser(const QString&)));
    connect(&interface,SIGNAL(userIsRemovedFromServer(const QString&)),SLOT(slotUserIsRemoved(const QString&)));

    connect(ui->inviteButton,SIGNAL(clicked(bool)),SLOT(slotInviteButtonClicked()));
    connect(&interface,SIGNAL(inviteStatus(const InviteStatuses&)),SLOT(slotInviteStatusHandler(const InviteStatuses&)));
    connect(&interface,SIGNAL(inviteRequest(const QString&)),SLOT(slotInviteRequestHandler(const QString&)));

    connect(invite_widget,SIGNAL(accept()),SLOT(slotAcceptHandler()));
    connect(invite_widget,SIGNAL(reject()),SLOT(slotRejectHandler()));

    connect(ui->closeInviteButton,SIGNAL(clicked(bool)),&interface,SLOT(revertInvite()));
    connect(&interface,SIGNAL(closeInvite()),SLOT(slotCloseInviteRequestHandler()));
    connect(&interface,SIGNAL(closeGame(const CloseGameStatuses&)),SLOT(slotGameClose(const CloseGameStatuses&)));
    connect(&interface,SIGNAL(coordsIsDelivered(const QPoint&)),SLOT(slotCoordsIsDelivered(QPoint)));

    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            GameButton* button = new GameButton(i,j);
            ui->coordsLayout->addWidget(button,i,j);
            connect(button,SIGNAL(clicked(bool)),SLOT(slotGameButtonClicked()));
        }
    }

    connect(ui->winButton,SIGNAL(clicked(bool)),SLOT(slotWinButtonClicked()));
    connect(ui->loseButton,SIGNAL(clicked(bool)),SLOT(slotLoseButtonClicked()));

    connect(ui->hitButton,SIGNAL(clicked(bool)),SLOT(slotOnHitButtonClicked()));
    connect(ui->missButton,SIGNAL(clicked(bool)),SLOT(slotOnMissButtonClicked()));

    connect(&interface,SIGNAL(answerOnGameStep(const GameStepAnswers&)),SLOT(slotOnAnswerGameStep(const GameStepAnswers&)));

    connect(ui->broadcastButton,SIGNAL(clicked(bool)),SLOT(slotOnBroadcastButtonClicked()));
    connect(&interface,SIGNAL(broadcastMessageIsDelivered(const QString&, const QString&)),SLOT(slotOnGetBroadcastMessage(const QString&,const QString&)));

    connect(ui->privateMessageButton,SIGNAL(clicked(bool)),SLOT(slotOnPrivateMessageButtonClicked()));
    connect(&interface,SIGNAL(privateMessageIsDelivered(const QString&,const QString&)),SLOT(slotOnGetPrivateMessage(const QString&,const QString&)));

    connect(ui->sessionMessageButton,SIGNAL(clicked(bool)),SLOT(slotOnSessionMessageButtonClicked()));
    connect(&interface,SIGNAL(sessionMessageIsDelivered(const QString&,const QString&)),SLOT(slotOnGetSessionMessage(const QString&, const QString&)));
}

void TestWindow::slotOnGetSessionMessage(const QString &sender, const QString &message){
    ui->textEdit->append("Session: " + sender + " " + message);
}

void TestWindow::slotOnSessionMessageButtonClicked(){
    QString text = ui->lineEdit->text();
    if(text == "")
        return;
    interface.sendSessionMessage(text);
    ui->lineEdit->clear();
}


void TestWindow::slotOnPrivateMessageButtonClicked(){
    QString text = ui->lineEdit->text();
    QString receiver = ui->receiverLineEdit->text();
    if(text == "" or receiver == "")
        return;
    interface.sendPrivateMessage(receiver,text);
    ui->lineEdit->clear();
}


void TestWindow::slotOnGetPrivateMessage(const QString& sender, const QString &message){
    ui->textEdit->append("Private: "+sender + " : " + message);
}

void TestWindow::slotOnGetBroadcastMessage(const QString& sender, const QString& message){
    ui->textEdit->append("Broadcast: " + sender + " : " + message);
}


void TestWindow::slotOnBroadcastButtonClicked(){
    QString text = ui->lineEdit->text();
    if(text == "")
        return;
    interface.sendBroadcastMessage(text);
    ui->lineEdit->clear();
}

void TestWindow::slotOnHitButtonClicked(){
    interface.sendAnswerOnGameStep(GameStepAnswers::Hit);
}

void TestWindow::slotOnMissButtonClicked(){
    interface.sendAnswerOnGameStep(GameStepAnswers::Miss);
}

void TestWindow::slotOnAnswerGameStep(const GameStepAnswers &answer){
    if(answer == GameStepAnswers::Hit)
        ui->textEdit->append("Попадание");
    if(answer == GameStepAnswers::Miss)
        ui->textEdit->append("Промах");
}

void TestWindow::slotWinButtonClicked(){
    interface.sendGameResult(CloseGameStatuses::Win);
}

void TestWindow::slotLoseButtonClicked(){
    interface.sendGameResult(CloseGameStatuses::Lose);
}

void TestWindow::slotCoordsIsDelivered(const QPoint &p){
    GameButton* gb = static_cast<GameButton*>(ui->coordsLayout->itemAtPosition(p.x(),p.y())->widget());
    gb->setText("P2");
}

void TestWindow::slotGameButtonClicked(){
    GameButton* gb = static_cast<GameButton*>(sender());
    gb->setText("P1");
    interface.sendCoords(QPoint(gb->getX(),gb->getY()));
}


void TestWindow::slotGameClose(const CloseGameStatuses& status){
    QString text;
    if(status == CloseGameStatuses::Win)
        text = "Lose";
    if(status == CloseGameStatuses::Lose)
        text = "Win";
    if(status == CloseGameStatuses::GamerIsLeave)
        text = "Parthner is leave";
    ui->textEdit->append(text);
}

void TestWindow::slotCloseInviteRequestHandler(){
    invite_widget->hide();
    qDebug() << "Invite has been closed";
}

void TestWindow::slotAcceptHandler(){
    interface.sendInviteAnswer(InviteStatuses::Accepted);
    invite_widget->hide();
}

void TestWindow::slotRejectHandler(){
    interface.sendInviteAnswer(InviteStatuses::Rejected);
    invite_widget->hide();
}

void TestWindow::slotInviteRequestHandler(const QString &initiator){
    invite_widget->setText(initiator);
    invite_widget->show();
}

void TestWindow::slotInviteStatusHandler(const InviteStatuses &invite_status){
    if(invite_status == InviteStatuses::Accepted){
        ui->textEdit->append("Принято");
    }
    if(invite_status == InviteStatuses::Failed){
        ui->textEdit->append("Ошибка");
    }
    if(invite_status == InviteStatuses::Rejected){
        ui->textEdit->append("Отклонено");
    }
}

void TestWindow::slotInviteButtonClicked(){
    QString receiver = ui->receiverLineEdit->text();
    interface.invite(receiver);
}


void TestWindow::slotNewUser(const QString& username){
    ui->textEdit->append("New user: "+username);
}

void TestWindow::slotUserIsRemoved(const QString& username){
    ui->textEdit->append("User is removed: "+username);
}


void TestWindow::slotUsersListIsDelivered(QStringList users_list){
    ui->textEdit->append("Users list: ");
    foreach(QString current_username, users_list){
        ui->textEdit->append(current_username);
    }
    ui->textEdit->append("---");
}

void TestWindow::slotAuthStatusHandler(const AuthStatuses &auth_status){
    if(auth_status == AuthStatuses::Ok){
        ui->textEdit->append("Auth ok");
    }
    if(auth_status == AuthStatuses::Failed){
        ui->textEdit->append("Auth failed");
    }
    if(auth_status == AuthStatuses::NeedAuth){
        ui->textEdit->append("Need auth");
    }
}

void TestWindow::slotAuthButtonClicked(){
    QString username = ui->lineEdit->text();
    if(username != "")
        interface.authenticate(username);
    ui->lineEdit->clear();
}

TestWindow::~TestWindow()
{
    delete ui;
}

void TestWindow::slotConnectButtonClicked(){
    interface.connectToHost(QHostAddress::LocalHost,1234);
}


void TestWindow::slotOnConnected(){
    ui->textEdit->append("Connected");
}

void TestWindow::slotOnDisconnected(){
    ui->textEdit->append("Disconnected");
}
