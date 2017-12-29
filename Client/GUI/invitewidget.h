#ifndef INVITEWIDGET_H
#define INVITEWIDGET_H

#include <QWidget>

namespace Ui {
class InviteWidget;
}

class InviteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InviteWidget(QWidget *parent = 0);
    ~InviteWidget();
signals:
    void accept();
    void reject();
public slots:
    void setText(const QString& text);
private:
    Ui::InviteWidget *ui;
};

#endif // INVITEWIDGET_H
