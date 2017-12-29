#include "menuwindow.h"
#include "ui_menuwindow.h"

MenuWindow::MenuWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MenuWindow)
{
    ui->setupUi(this);

    setWindowFlags(GUIConstants::windows_styles);

    networkConnectorWidget = new NetworkConnectorWidget;
    testWidget = new TestWindow(this);

    connect(ui->networkGameButton,SIGNAL(clicked(bool)),SLOT(onNetworkGameButtonClicked()));
    connect(ui->exitButton,SIGNAL(clicked(bool)),SLOT(onExitButtonClicked()));
    connect(ui->testButton, SIGNAL(clicked(bool)),SLOT(onTestButtonClicked()));
}

void MenuWindow::onNetworkGameButtonClicked(){
    this->hide();
    networkConnectorWidget->show();
}

void MenuWindow::onExitButtonClicked(){
    close();
}

void MenuWindow::onTestButtonClicked(){
    this->hide();
    testWidget->show();
}

MenuWindow::~MenuWindow()
{
    delete ui;
    networkConnectorWidget->deleteLater();
}
