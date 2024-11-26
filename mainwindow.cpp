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
    connect(ui->tit_white, &ColorFlask::addToCauldron, ui->pot, &Cauldron::addColor);
    connect(ui->reset, &QPushButton::clicked, ui->pot, &Cauldron::dump);
}

MainWindow::~MainWindow()
{
    delete ui;
}
