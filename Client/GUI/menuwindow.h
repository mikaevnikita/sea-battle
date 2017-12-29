#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "networkconnectorwidget.h"
#include "networkgamemainwindow.h"
#include "gui_constants.h"
#include "testwindow.h"


namespace Ui{
    class MenuWindow;
}

class MenuWindow : public QMainWindow
{
    Q_OBJECT
private:
    NetworkConnectorWidget* networkConnectorWidget;
    TestWindow* testWidget;
public:
    explicit MenuWindow(QWidget *parent = 0);
    ~MenuWindow();

private:
    Ui::MenuWindow *ui;
private slots:
    void onNetworkGameButtonClicked();
    void onExitButtonClicked();
    void onTestButtonClicked();
};

#endif // MENUWINDOW_H
