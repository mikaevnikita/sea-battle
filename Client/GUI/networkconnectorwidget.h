#ifndef NETWORKCONNECTORWIDGET_H
#define NETWORKCONNECTORWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include "networkgamemainwindow.h"
#include "gui_constants.h"

namespace Ui{
    class NetworkConnectorWidget;
}

class NetworkGameMainWindow;


class NetworkConnectorWidget : public QDialog
{
    Q_OBJECT
private:
    NetworkGameMainWindow* networkGameMainWindow;
public:
    explicit NetworkConnectorWidget(QDialog *parent = 0);
    ~NetworkConnectorWidget();

private:
    Ui::NetworkConnectorWidget *ui;
public slots:
    void onConnectButtonClicked();
};

#endif // NETWORKCONNECTORWIDGET_H
