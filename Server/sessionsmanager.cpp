#include "sessionsmanager.h"

SessionsManager::SessionsManager()
{

}


void SessionsManager::addSession(Session* session){
    sessions_list.append(session);
}

Session* SessionsManager::getSession(QTcpSocket* const socket){
    //Принимает один из сокетов в сессии
    QMutableListIterator<Session*> iter(sessions_list);
    while(iter.hasNext()){
        iter.next();
        Session* current_session = iter.value();
        std::pair<QTcpSocket*,QTcpSocket*> gamers = current_session->getGamers();
        if(gamers.first == socket or gamers.second == socket){
            return current_session;
        }
    }
    return nullptr;
}

Session* SessionsManager::getSession(const int& session_id){
    QMutableListIterator<Session*> iter(sessions_list);
    while(iter.hasNext()){
        iter.next();
        Session* current_session = iter.value();
        if(current_session->sessionId() == session_id)
            return current_session;
    }
    return nullptr;
}


void SessionsManager::removeSession(QTcpSocket* const socket){
    QMutableListIterator<Session*> iter(sessions_list);
    while(iter.hasNext()){
        iter.next();
        Session* current_session = iter.value();
        std::pair<QTcpSocket*,QTcpSocket*> gamers = current_session->getGamers();
        if(gamers.first == socket or gamers.second == socket){
            iter.remove();
            delete current_session;
            return;
        }
    }
}

void SessionsManager::removeSession(const int& session_id){
    QMutableListIterator<Session*> iter(sessions_list);
    while(iter.hasNext()){
        iter.next();
        Session* current_session = iter.value();
        if(current_session->sessionId() == session_id){
            iter.remove();
            delete current_session;
            return;
        }
    }
}

void SessionsManager::removeSession(Session * const session){
    QMutableListIterator<Session*> iter(sessions_list);
    while(iter.hasNext()){
        iter.next();
        Session* current_session = iter.value();
        if(current_session == session){
            iter.remove();
            delete current_session;
            return;
        }
    }
}

SessionsManager::~SessionsManager(){
    QMutableListIterator<Session*> iter(sessions_list);
    while(iter.hasNext()){
        iter.next();
        Session* current_session = iter.value();
        delete current_session;
    }
}
