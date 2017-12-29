#include "NetworkInterface.h"
#include <QDebug>

void NetworkInterface::slotReadDataFromServer(){
    QDataStream in(socket);
    while(true){
        if(block_size == 0){
            //Если можем прочитать новый блок
            if(socket->bytesAvailable() < sizeof(BlockSize))
                break;//Если не пришел еще размер блока
            in >> block_size;
        }
        if(socket->bytesAvailable() < block_size)
            break;//Если не пришли оставшиеся данные
        NetworkCommand command;
        in >> command;
        if((NetworkCommands)command == NetworkCommands::AuthRequest){
            AuthStatus auth_status;
            in >> auth_status;
            emit authenticatedStatus((AuthStatuses)auth_status);
        }
        if((NetworkCommands)command == NetworkCommands::UsersListRequest){
            QStringList users_list;
            in >> users_list;
            emit usersListIsDelivered(users_list);
        }
        if((NetworkCommands)command == NetworkCommands::NewUserIsAdded){
            QString username;
            in >> username;
            emit newUserOnServer(username);
        }
        if((NetworkCommands)command == NetworkCommands::UserIsRemoved){
            QString username;
            in >> username;
            emit userIsRemovedFromServer(username);
        }
        if((NetworkCommands)command == NetworkCommands::InviteRequest){
            QString initiator;
            in >> initiator;
            emit inviteRequest(initiator);
        }
        if((NetworkCommands)command == NetworkCommands::InviteStatus){
            InviteStatus invite_status;
            in >> invite_status;
            emit inviteStatus((InviteStatuses)invite_status);
        }
        if((NetworkCommands)command == NetworkCommands::CloseInvite){
            emit closeInvite();
        }
        if((NetworkCommands)command == NetworkCommands::CloseGame){
            CloseGameStatus status;
            in >> status;
            emit closeGame((CloseGameStatuses)status);
        }
        if((NetworkCommands)command == NetworkCommands::SendCoords){
            QPoint coords;
            in >> coords;
            emit coordsIsDelivered(coords);
        }
        if((NetworkCommands)command == NetworkCommands::AnswerOnGameStep){
            GameStepAnswer answer;
            in >> answer;
            emit answerOnGameStep((GameStepAnswers)answer);
        }
        if((NetworkCommands)command == NetworkCommands::ChatBroadcastMessage){
            QString sender;
            QString message;
            in >> sender;
            in >> message;
            emit broadcastMessageIsDelivered(sender,message);
        }
        if((NetworkCommands)command == NetworkCommands::ChatPrivateMessage){
            QString sender;
            QString message;
            in >> sender;
            in >> message;
            emit privateMessageIsDelivered(sender,message);
        }
        if((NetworkCommands)command == NetworkCommands::ChatSessionMessage){
            QString sender;
            QString message;
            in >> sender;
            in >> message;
            emit sessionMessageIsDelivered(sender,message);
        }
        block_size=0;
    }
    block_size = 0;
}

void NetworkInterface::sendSessionMessage(const QString &message){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::ChatSessionMessage << message;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    socket->write(block);
}

void NetworkInterface::sendPrivateMessage(const QString &receiver, const QString &message){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::ChatPrivateMessage << receiver << message;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    socket->write(block);
}

void NetworkInterface::sendBroadcastMessage(const QString &message){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::ChatBroadcastMessage << message;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    socket->write(block);
}


void NetworkInterface::sendAnswerOnGameStep(const GameStepAnswers &answer){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::AnswerOnGameStep << (quint8)answer;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    socket->write(block);
}

void NetworkInterface::sendGameResult(const CloseGameStatuses &status){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::CloseGame << (quint8)status;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    socket->write(block);
}

void NetworkInterface::sendCoords(const QPoint &coords){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::SendCoords << coords;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    socket->write(block);
}


void NetworkInterface::revertInvite(){
    sendEasyCommand(NetworkCommands::CloseInvite);
}


void NetworkInterface::sendInviteAnswer(const InviteStatuses &invite_answer){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::InviteStatus << (quint8)invite_answer;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    socket->write(block);
}

void NetworkInterface::invite(const QString &parthner_username){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::InviteRequest << parthner_username;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    socket->write(block);
}



void NetworkInterface::sendEasyCommand(const NetworkCommands &command){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)command;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    socket->write(block);
}

void NetworkInterface::getUsersList(){
    sendEasyCommand(NetworkCommands::UsersListRequest);
}

void NetworkInterface::authenticate(const QString &login){
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out << (BlockSize)0 << (quint8)NetworkCommands::AuthRequest << login;
    out.device()->seek(0);
    out << (BlockSize)(block.size() - sizeof(BlockSize));
    socket->write(block);
}


NetworkInterface::NetworkInterface(QObject *parent):QObject(parent){
    socket = new QTcpSocket(this);
    block_size = 0;

    QObject::connect(socket,SIGNAL(connected()),SIGNAL(connected()));
    QObject::connect(socket,SIGNAL(disconnected()),SIGNAL(disconnected()));
    QObject::connect(socket,SIGNAL(readyRead()),SLOT(slotReadDataFromServer()));
    QObject::connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),SIGNAL(error(const QAbstractSocket::SocketError&)));

}

void NetworkInterface::connectToHost(const QHostAddress &host, const quint16 &port){
    socket->connectToHost(host,port);
}

void NetworkInterface::connectToHost(const QString &host, const quint16 &port){
    socket->connectToHost(host,port);
}

const QString NetworkInterface::getSocketErrorString(){
    return socket->errorString();
}

NetworkInterface::~NetworkInterface(){
    disconnectFromHost();
}

void NetworkInterface::disconnectFromHost(){
    socket->close();
}


