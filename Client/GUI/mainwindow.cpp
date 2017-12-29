#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gamePanel/nbattlefield.h"
#include "gamePanel/playerfield.h"
#include "scorepanel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //field = new playerField(10, 1, 2, 3, 4, this);
    field = new playerPanel(this);
    panel = new ScorePanel(this);
    enemyField = new NBattleField(10, 1, 2, 3, 4, this);

    ui->horizontalLayout->addWidget(field);
    ui->horizontalLayout->addWidget(panel);
    ui->horizontalLayout->addWidget(enemyField);

    //NShip *ship = new NShip(4, this);
    //field->addShip(ship, 2, 4);
    //NShip *ship2 = new NShip(3, this, NShip::Orientation::Vertical);
    //field->addShip(ship2, 6, 6);
}

MainWindow::~MainWindow()
{
    delete ui;
}
