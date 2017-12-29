#ifndef SESSIONSMANAGER_H
#define SESSIONSMANAGER_H
#include "session.h"

class SessionsManager
{
private:
    QList<Session*> sessions_list;
public:
    SessionsManager();
    void addSession(Session* const session);
    Session* getSession(QTcpSocket* const socket);
    Session* getSession(const int& session_id);
    void removeSession(QTcpSocket* const socket);
    void removeSession(const int& session_id);
    void removeSession(Session* const session);
    ~SessionsManager();
};

#endif // SESSIONSMANAGER_H
