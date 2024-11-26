#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QColor>
#include "colorflask.h"
#include "Cauldron.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , game()
{
    ui->setupUi(this);
    ui->tit_white->changeColor(QColor("#FFFFFF"));
    ui->cad_yel->changeColor(QColor("#FDDA0D"));
    ui->cad_deep_yellow->changeColor(QColor("#E68125"));
    ui->cad_red->changeColor(QColor("#D22B2B"));
    ui->al_crimson->changeColor(QColor("#E32636"));
    ui->cer_blue->changeColor(QColor("#2a52be"));
    ui->cobalt_blue->changeColor(QColor("#0047AB"));
    ui->iv_black->changeColor(QColor("#0C0B0A"));

    connect(ui->tit_white,&ColorFlask::addToCauldron,ui->pot,&Cauldron::addColor);
    connect(ui->cad_yel,&ColorFlask::addToCauldron,ui->pot,&Cauldron::addColor);
    connect(ui->cad_deep_yellow,&ColorFlask::addToCauldron,ui->pot,&Cauldron::addColor);
    connect(ui->cad_red,&ColorFlask::addToCauldron,ui->pot,&Cauldron::addColor);
    connect(ui->al_crimson,&ColorFlask::addToCauldron,ui->pot,&Cauldron::addColor);
    connect(ui->cer_blue,&ColorFlask::addToCauldron,ui->pot,&Cauldron::addColor);
    connect(ui->cobalt_blue,&ColorFlask::addToCauldron,ui->pot,&Cauldron::addColor);
    connect(ui->iv_black,&ColorFlask::addToCauldron,ui->pot,&Cauldron::addColor);

    connect(ui->pot,&Cauldron::updateDisplay,ui->potColor,[this](QColor color){ui->potColor->setStyleSheet(QString("image: none; background-color:"+color.name()+"; border-image: none; border-radius: 28px"));});
}

MainWindow::~MainWindow()
{
    delete ui;
}
