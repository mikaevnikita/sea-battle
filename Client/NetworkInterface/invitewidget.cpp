#include "invitewidget.h"
#include "ui_invitewidget.h"

InviteWidget::InviteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InviteWidget)
{
    ui->setupUi(this);
    connect(ui->acceptButton,SIGNAL(clicked(bool)),SIGNAL(accept()));
    connect(ui->rejectButton,SIGNAL(clicked(bool)),SIGNAL(reject()));
}

InviteWidget::~InviteWidget()
{
    delete ui;
}

void InviteWidget::setText(const QString &text){
    ui->label->setText(text);
}
