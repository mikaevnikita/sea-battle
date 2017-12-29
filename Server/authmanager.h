#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include "NetworkConstants.h"
#include <utility>
#include <QString>
#include <functional>
#include <QMutex>
#include <QMutexLocker>

class AuthManager
{
private:
    QMap<QTcpSocket*,std::pair<bool,QString>> clients;
public:
    AuthManager();
    bool loginIsUsed(const QString& login);
    bool authentificated(QTcpSocket* const client);
    void append(QTcpSocket* const client , const QString& username, bool auth_status);
    void setAuthStatus(QTcpSocket* const client , bool new_auth_status);
    void setUsername(QTcpSocket* const client, const QString& username);
    void remove(QTcpSocket* const client);
    void remove(const QString& username);
    QStringList getUsernames();
    QString getUsernameBySocket(QTcpSocket* const client);
    QTcpSocket* getSocketByUsername(const QString& username);
    QMap<QTcpSocket*,std::pair<bool,QString>> getData();
};

#endif // AUTHMANAGER_H
