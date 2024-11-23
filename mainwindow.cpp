#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
, ui(new Ui::MainWindow)
    , game()
{
    ui->setupUi(this);
    ui->tit_white->setStyleSheet("background-color: #FFFFFF;");
    ui->cad_yel->setStyleSheet("background-color: #FDDA0D;");
    ui->cad_deep_yellow->setStyleSheet("background-color: #E68125;");
    ui->cad_red->setStyleSheet("background-color: #D22B2B;");
    ui->al_crimson->setStyleSheet("background-color: #E32636;");
    ui->cer_blue->setStyleSheet("background-color: #2a52be;");
    ui->cobalt_blue->setStyleSheet("background-color: #0047AB;");
    ui->iv_black->setStyleSheet("background-color: #0C0B0A;");
}

MainWindow::~MainWindow()
{
    delete ui;
}
