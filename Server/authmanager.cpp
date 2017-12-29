#include "authmanager.h"

AuthManager::AuthManager()
{

}


QMap<QTcpSocket*,std::pair<bool,QString>> AuthManager::getData(){
    return clients;
}

void AuthManager::setUsername(QTcpSocket * const client, const QString &username){

    QMap<QTcpSocket*,std::pair<bool,QString>>::Iterator iter = clients.find(client);
    if(iter != clients.end())
        iter.value().second = username;

}

bool AuthManager::authentificated(QTcpSocket* const client){

    QMap<QTcpSocket*,std::pair<bool,QString>>::Iterator iter = clients.find(client);
    bool auth_status = false;
    if(iter != clients.end())
        auth_status = iter.value().first;
    return auth_status;
}

QStringList AuthManager::getUsernames(){

    QMapIterator<QTcpSocket*,std::pair<bool,QString>> iter(clients);
    QStringList usernames;
    while(iter.hasNext()){
        if(authentificated(iter.next().key())){
            QString current_username = iter.value().second;
            usernames.append(current_username);
        }
    }
    return usernames;
}

bool AuthManager::loginIsUsed(const QString &login){

    QMapIterator<QTcpSocket*,std::pair<bool,QString>> iter(clients);
    bool isUsed = false;
    while(iter.hasNext()){
        QString current_login = iter.next().value().second;
        if(current_login == login){
            isUsed = true;
            break;
        }
    }
    return isUsed;
}



QString AuthManager::getUsernameBySocket(QTcpSocket* const client){

    QString username = "";
    QMap<QTcpSocket*,std::pair<bool,QString>>::Iterator iter = clients.find(client);
    if(iter != clients.end()){
        username = iter.value().second;
    }
    return username;
}


QTcpSocket* AuthManager::getSocketByUsername(const QString &username){

    QTcpSocket* socket = nullptr;
    QMapIterator<QTcpSocket*,std::pair<bool,QString>> iter(clients);
    while(iter.hasNext()){
        QString current_username = iter.next().value().second;
        if(current_username == username){
            socket = iter.key();
            break;
        }
    }
    return socket;
}

void AuthManager::append(QTcpSocket* const client, const QString &username, bool auth_status){

    clients.insert(client,std::make_pair(auth_status,username));

}


void AuthManager::setAuthStatus(QTcpSocket* const client , bool new_auth_status){

    QMap<QTcpSocket*,std::pair<bool,QString>>::Iterator iter = clients.find(client);
    if(iter != clients.end()){
        iter.value().first = new_auth_status;
    }

}

void AuthManager::remove(QTcpSocket* const client){

    QMap<QTcpSocket*,std::pair<bool,QString>>::Iterator iter = clients.find(client);
    if(iter != clients.end()){
        clients.erase(iter);
    }

}

void AuthManager::remove(const QString& username){

    QMap<QTcpSocket*,std::pair<bool,QString>>::Iterator iter = clients.begin();
    for(iter;iter!=clients.end();++iter){
        QString current_username = iter.value().second;
        if(current_username == username){
            clients.erase(iter);
        }
    }

}
