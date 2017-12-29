#include "server.h"
#include "ui_widget.h"

void Server::slotRead(){
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    QDataStream in(client);
    while(true){
        if(block_size == 0){
            //Если можем прочитать новый блок
            if(client->bytesAvailable() < sizeof(BlockSize))
                break;//Если не пришел еще размер блока
            in >> block_size;
        }
        if(client->bytesAvailable() < block_size)
            break;//Если не пришли оставшиеся данные
        NetworkCommand command;
        in >> command;
        if((NetworkCommands)command == NetworkCommands::AuthRequest){
            QString login;
            in >> login;
            authenticate(client,login);
            break;
        }
        if(!auth_manager.authentificated(client)){
            slotSendAuthRequest(client,AuthStatuses::NeedAuth);
            break;
        }
        if((NetworkCommands)command == NetworkCommands::UsersListRequest){
            slotSendUsernames(client);
        }
        if((NetworkCommands)command == NetworkCommands::InviteRequest){
            QString session_parthner;
            in >> session_parthner;
            QTcpSocket* parthner_socket = auth_manager.getSocketByUsername(session_parthner);
            if(parthner_socket == nullptr or parthner_socket == client){
                slotSendInviteStatus(client,InviteStatuses::Failed);
            }
            else
                //Те если партнер существует и аутентифицирован
                slotInviteHandler(client,parthner_socket);
        }
        if((NetworkCommands)command == NetworkCommands::InviteStatus){
            InviteStatus invite_status;
            in >> invite_status;
            slotHandleInviteAnswer(client,(InviteStatuses)invite_status);
        }
        if((NetworkCommands)command == NetworkCommands::CloseInvite){
            handleCloseInvite(client);
        }
        if((NetworkCommands)command == NetworkCommands::SendCoords){
            QPoint coords;
            in >> coords;
            handleDeliveredCoords(client,coords);
        }
        if((NetworkCommands)command == NetworkCommands::CloseGame){
            CloseGameStatus status;
            in >> status;
            handleCloseGame(client,(CloseGameStatuses)status);
        }
        if((NetworkCommands)command == NetworkCommands::AnswerOnGameStep){
            GameStepAnswer answer;
            in >> answer;
            handleGameStepAnswer(client,(GameStepAnswers)answer);
        }
        if((NetworkCommands)command == NetworkCommands::ChatBroadcastMessage){
            QString message;
            in >> message;
            handleBroadcastMessage(client,message);
        }
        if((NetworkCommands)command == NetworkCommands::ChatPrivateMessage){
            QString receiver_username;
            QString message;
            in >> receiver_username;
            in >> message;
            handlePrivateMessage(client,receiver_username,message);
        }
        if((NetworkCommands)command == NetworkCommands::ChatSessionMessage){
            QString message;
            in >> message;
            handleSessionMessage(client,message);
        }
        block_size = 0;
    }
    block_size = 0;
}

void Server::slotSendSessionMessageTo(QTcpSocket * const receiver, const QString &sender, const QString &message){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::ChatSessionMessage << sender << message;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    receiver->write(block);
}


void Server::handleSessionMessage(QTcpSocket * const sender, const QString &message){
    Session* session = sessions_manager.getSession(sender);
    if(session == nullptr)
        return;
    if(session->sessionStatus() != SessionStatuses::Active)
        return;
    QTcpSocket* parthner = session->getParthner(sender);
    QString sender_username = auth_manager.getUsernameBySocket(sender);
    slotSendSessionMessageTo(parthner,sender_username,message);
}


void Server::slotSendPrivateMessageTo(QTcpSocket * const receiver, const QString &sender, const QString &message){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::ChatPrivateMessage << sender << message;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    receiver->write(block);
}

void Server::handlePrivateMessage(QTcpSocket * const sender, const QString &receiver, const QString &message){
    if(!auth_manager.loginIsUsed(receiver))
        return;
    QString sender_username = auth_manager.getUsernameBySocket(sender);
    QTcpSocket* receiver_socket = auth_manager.getSocketByUsername(receiver);
    slotSendPrivateMessageTo(receiver_socket,sender_username,message);
}

void Server::slotSendBroadcastMessageTo(QTcpSocket * const receiver, const QString &sender, const QString &message){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::ChatBroadcastMessage << sender << message;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    receiver->write(block);
}

void Server::handleBroadcastMessage(QTcpSocket * const sender, const QString &message){
    QString sender_username = auth_manager.getUsernameBySocket(sender);
    QMap<QTcpSocket*,std::pair<bool,QString>> data = auth_manager.getData();
    QMapIterator<QTcpSocket*,std::pair<bool,QString>> iter(data);
    while(iter.hasNext()){
        iter.next();
        QTcpSocket* current_socket = iter.key();
        bool auth_status = iter.value().first;
        if(auth_status and current_socket!= sender)
            slotSendBroadcastMessageTo(current_socket,sender_username,message);
    }
}

void Server::slotSendGameStepAnswer(QTcpSocket * const receiver, const GameStepAnswers &answer){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::AnswerOnGameStep << (quint8)answer;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    receiver->write(block);
}

void Server::handleGameStepAnswer(QTcpSocket * const sender, const GameStepAnswers &answer){
    Session* session = sessions_manager.getSession(sender);
    if(session == nullptr)
        return;
    if(session->sessionStatus() != SessionStatuses::Active)
        return;
    slotSendGameStepAnswer(session->getParthner(sender),answer);
}

void Server::handleCloseGame(QTcpSocket* const sender, const CloseGameStatuses& status){
    Session* session = sessions_manager.getSession(sender);
    if(session == nullptr)
        return;
    if(session->sessionStatus() != SessionStatuses::Active)
        return;
    slotSendCloseGame(session->getParthner(sender),status);
    sessions_manager.removeSession(session);
}

void Server::slotSendCoords(QTcpSocket* const receiver, const QPoint& coords){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::SendCoords << coords;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    receiver->write(block);
}

void Server::handleDeliveredCoords(QTcpSocket * const sender, const QPoint& coords){
    Session* session = sessions_manager.getSession(sender);
    if(session == nullptr)
        return;
    if(session->sessionStatus() != SessionStatuses::Active)
        return;
    QTcpSocket* parthner = session->getParthner(sender);
    slotSendCoords(parthner,coords);
}


void Server::sendEasyCommand(QTcpSocket* const receiver, const NetworkCommands& command){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)command;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    receiver->write(block);
}


void Server::slotSendCloseInvite(QTcpSocket* const receiver){
    sendEasyCommand(receiver,NetworkCommands::CloseInvite);
}

void Server::handleCloseInvite(QTcpSocket* const sender){
    Session* session = sessions_manager.getSession(sender);
    if(session == nullptr)
        return;
    if(session->sessionStatus() != SessionStatuses::Invite)
        return;
    QTcpSocket* parthner = session->getParthner(sender);
    sessions_manager.removeSession(session);
    slotSendCloseInvite(parthner);
}



void Server::slotHandleInviteAnswer(QTcpSocket * const sender, const InviteStatuses& invite_answer){
    Session* session = sessions_manager.getSession(sender);
    if(session == nullptr)
        return;
    if(session->sessionStatus() != SessionStatuses::Invite)
        return;

    QTcpSocket* initiator = session->getParthner(sender);

    if(invite_answer == InviteStatuses::Accepted)
        session->setSessionStatus(SessionStatuses::Active);
    if(invite_answer == InviteStatuses::Failed or invite_answer == InviteStatuses::Rejected)
        sessions_manager.removeSession(session);
    slotSendInviteStatus(initiator,invite_answer);
}


void Server::slotSendInviteRequest(QTcpSocket* const receiver, const QString& sender_username){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::InviteRequest << sender_username;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    receiver->write(block);
}

void Server::slotInviteHandler(QTcpSocket* const sender, QTcpSocket* const receiver){
    Session* sender_session = sessions_manager.getSession(sender);
    Session* receiver_session = sessions_manager.getSession(receiver);
    if(sender_session == nullptr and receiver_session == nullptr){
        //То есть не более одного инвайта можно сделать в определенный момент
        Session* session = new Session(sender,receiver,SessionStatuses::Invite);
        sessions_manager.addSession(session);
        slotSendInviteRequest(receiver,auth_manager.getUsernameBySocket(sender));
    }
}


void Server::slotSendInviteStatus(QTcpSocket* const client, const InviteStatuses& invite_status){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::InviteStatus << (quint8)invite_status;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    client->write(block);
}



void Server::slotSendUsernames(QTcpSocket* const client){
    QStringList users_logins = auth_manager.getUsernames();
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::UsersListRequest << users_logins;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    client->write(block);
}



Server::Server(const quint16& port, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    server = new QTcpServer(this);
    block_size = 0;
    connect(server,SIGNAL(newConnection()),SLOT(slotNewConnection()));
    if(!server->listen(QHostAddress::LocalHost,port)){
        QMessageBox::critical(0,"Server error",server->errorString());
        server->close();
        return;
    }
}

void Server::slotSendAuthRequest(QTcpSocket* const client, const AuthStatuses& auth_status){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::AuthRequest << (quint8)auth_status;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    client->write(block);
}


void Server::slotNewConnection(){
    QTcpSocket* client = server->nextPendingConnection();
    connect(client,SIGNAL(readyRead()),SLOT(slotRead()));
    connect(client,SIGNAL(disconnected()),SLOT(slotDisconnected()));
    connect(client,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(slotError()));
    auth_manager.append(client,"",false);
    ui->textEdit->append("Connected");
    slotSendAuthRequest(client,AuthStatuses::NeedAuth);
}

void Server::slotSendBroadcastUserIsRemoved(const QString &username){
    if(username == "")
        return;
    QMap<QTcpSocket*,std::pair<bool,QString>> clients = auth_manager.getData();
    QMapIterator<QTcpSocket*,std::pair<bool,QString>> iter(clients);
    while(iter.hasNext()){
        iter.next();
        QTcpSocket* client = iter.key();
        bool auth_status = iter.value().first;
        QString receiver_username = iter.value().second;
        if(auth_status and receiver_username != username){
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out << (BlockSize)0 << (quint8)NetworkCommands::UserIsRemoved << username;
            out.device()->seek(0);
            out << (BlockSize)(block.size() - sizeof(BlockSize));
            client->write(block);        }
    }
}

void Server::slotSendCloseGame(QTcpSocket* const receiver, const CloseGameStatuses& status){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::CloseGame << (quint8)status;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    receiver->write(block);
}

void Server::handleDisconnect(QTcpSocket * const client){
    Session* session = sessions_manager.getSession(client);
    if(session == nullptr)
        return;
    SessionStatuses session_status = session->sessionStatus();
    if(session_status == SessionStatuses::Active)
        slotSendCloseGame(session->getParthner(client),CloseGameStatuses::GamerIsLeave);
    else{
        std::pair<QTcpSocket*,QTcpSocket*> gamers = session->getGamers();
        QTcpSocket* initiator = gamers.first;
        QTcpSocket* parthner = gamers.second;
        if(client == initiator){
            //Те если отключается инициатор инвайта
            sendEasyCommand(parthner,NetworkCommands::CloseInvite);
        }
        else{
            //Если отключается принимающий инвайт отправляем ответ отклонено на инициатора
            slotHandleInviteAnswer(parthner,InviteStatuses::Rejected);
        }
    }
    sessions_manager.removeSession(session);
}

void Server::slotDisconnected(){
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    QString login = auth_manager.getUsernameBySocket(socket);
    slotSendBroadcastUserIsRemoved(login);
    ui->textEdit->append(login + " was Disconnected");
    handleDisconnect(socket);
    clearSocket(socket);
}

void Server::slotError(){
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    QString login = auth_manager.getUsernameBySocket(socket);
    slotSendBroadcastUserIsRemoved(login);
    ui->textEdit->append(login + " has error " + socket->errorString());
    handleDisconnect(socket);
    clearSocket(socket);
}

void Server::clearSocket(QTcpSocket* const socket){
    auth_manager.remove(socket);
    socket->close();
}


void Server::slotSendBroadcastAddNewUser(const QString &username){
    QMap<QTcpSocket*,std::pair<bool,QString>> clients = auth_manager.getData();
    QMapIterator<QTcpSocket*,std::pair<bool,QString>> iter(clients);
    while(iter.hasNext()){
        iter.next();
        QTcpSocket* client = iter.key();
        bool auth_status = iter.value().first;
        if(auth_status){
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out << (BlockSize)0 << (quint8)NetworkCommands::NewUserIsAdded << username;
            out.device()->seek(0);
            out << (BlockSize)(block.size() - sizeof(BlockSize));
            client->write(block);
        }
    }
}


void Server::authenticate(QTcpSocket* const client, const QString &login){
    if(auth_manager.loginIsUsed(login) or auth_manager.authentificated(client)){
        slotSendAuthRequest(client,AuthStatuses::Failed);
        return;
    }
    auth_manager.setAuthStatus(client,true);
    auth_manager.setUsername(client,login);
    slotSendAuthRequest(client,AuthStatuses::Ok);
    slotSendBroadcastAddNewUser(login);
}

Server::~Server()
{
    delete ui;

}
