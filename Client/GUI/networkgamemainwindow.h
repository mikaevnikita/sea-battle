#ifndef NETWORKGAMEMAINWINDOW_H
#define NETWORKGAMEMAINWINDOW_H

#include <QObject>
#include <QWidget>
#include "gui_constants.h"
#include "networkconnectorwidget.h"

namespace Ui{
    class NetworkGameMainWindow;
}

class NetworkGameMainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkGameMainWindow(QWidget *parent = nullptr);
    ~NetworkGameMainWindow();
signals:

private:
    Ui::NetworkGameMainWindow* ui;
};

#endif // NETWORKGAMEMAINWINDOW_H
