#include "networkconnectorwidget.h"
#include "ui_networkconnectorwidget.h"
#include "gui_constants.h"

NetworkConnectorWidget::NetworkConnectorWidget(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::NetworkConnectorWidget)
{
    ui->setupUi(this);
    setWindowFlags(GUIConstants::windows_styles);

    networkGameMainWindow = new NetworkGameMainWindow;

    connect(ui->connectButton, SIGNAL(clicked(bool)), SLOT(onConnectButtonClicked()));
}

void NetworkConnectorWidget::onConnectButtonClicked(){

}

NetworkConnectorWidget::~NetworkConnectorWidget()
{
    delete ui;
}
