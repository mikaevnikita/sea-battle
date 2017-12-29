#include "networkgamemainwindow.h"
#include "ui_networkgamemainwindow.h"

NetworkGameMainWindow::NetworkGameMainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::NetworkGameMainWindow)
{
    ui->setupUi(this);
    setWindowFlags(GUIConstants::windows_styles);
}

NetworkGameMainWindow::~NetworkGameMainWindow(){
    delete ui;
}
