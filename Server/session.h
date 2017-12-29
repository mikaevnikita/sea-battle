#ifndef SESSION_H
#define SESSION_H

#include <QTcpSocket>
#include <QTimer>
#include <QTimerEvent>
#include <QTcpSocket>
#include <NetworkConstants.h>
#include <utility>

class Session
{
private:
    QTcpSocket* initiator_socket;
    QTcpSocket* parthner_socket;
    SessionStatus session_status;
    static int sessions_count;
    int session_id;
public:
    Session();
    Session(const Session& session);
    Session(QTcpSocket* const _initiator_socket,QTcpSocket* const _parthner_socket, const SessionStatuses& _session_status);
    void setSessionStatus(const SessionStatuses& status);
    const SessionStatuses& sessionStatus() const;
    std::pair<QTcpSocket*,QTcpSocket*> getGamers() const;
    const int& sessionId() const;
    QTcpSocket* getParthner(QTcpSocket* const parthner) const;
};

#endif // SESSION_H
