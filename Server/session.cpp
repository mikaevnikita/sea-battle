#include "session.h"

int Session::sessions_count = 0;

Session::Session()
{
    initiator_socket = nullptr;
    parthner_socket = nullptr;
    session_status = (SessionStatus)SessionStatuses::Invite;
    session_id = sessions_count;
    ++sessions_count;
}


QTcpSocket* Session::getParthner(QTcpSocket* const parthner) const{
    if(parthner == initiator_socket)
        return parthner_socket;
    else{
        if(parthner == parthner_socket)
            return initiator_socket;
        else
            return nullptr;
    }
}

Session::Session(const Session &session){
    initiator_socket = session.initiator_socket;
    parthner_socket = session.parthner_socket;
    session_status = session.session_status;
    session_id = session.session_id;
}

std::pair<QTcpSocket*,QTcpSocket*> Session::getGamers() const{
    return std::make_pair((QTcpSocket*)initiator_socket,(QTcpSocket*)parthner_socket);
}

Session::Session(QTcpSocket* const _initiator_socket,QTcpSocket* const _parthner_socket, const SessionStatuses& _session_status){
    initiator_socket = _initiator_socket;
    parthner_socket = _parthner_socket;
    session_status = (SessionStatus)_session_status;
    session_id = sessions_count;
    ++sessions_count;
}



void Session::setSessionStatus(const SessionStatuses& status){
    session_status = (SessionStatus)status;
}

const SessionStatuses& Session::sessionStatus() const{
    return (SessionStatuses)session_status;
}


const int& Session::sessionId() const{
    return session_id;
}
