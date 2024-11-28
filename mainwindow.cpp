#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QColor>
#include "colorflask.h"
#include "Cauldron.h"
#include "Game.h"

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

    //find all flasks and connect them
    for (ColorFlask* flask : this->findChildren<ColorFlask*>())
        connect(flask,&ColorFlask::addToCauldron,ui->pot,&Cauldron::addColor);
    connect(ui->reset, &QPushButton::clicked, ui->pot, &Cauldron::dump);
    connect(ui->pot, &Cauldron::newMixedColor, &game, &Game::checkGoal);
    connect(ui->pot, &Cauldron::newMixedColor,ui->potColor,
            [this](QColor color) {
                ui->potColor->setStyleSheet(QString(
                    "background-color:"+color.name()+";"
                    "border-radius: 25px"
                    ));
            }
        );

    connect(ui->pot, &Cauldron::colorsChanged, ui->pieChart, &MixingPieChart::updatePieSeries);

    connect(&game, &Game::newGoal, this, &MainWindow::updateGoal);
    connect(&game, &Game::newHint, this, &MainWindow::newHint);

    labelsIdx = 0;
    labels = {
        {"Here are your flasks of the 8 basic oil painting colors. "
         "You can drag and drop them into the cauldron to mix colors.", QPoint(330, 20)},
        {"This is Ivory Black. It has a slight warm tone, and is good for darkening shadows. Mixed with white, it is good for making neutral grays.", QPoint(30, 20)},
        {"Here is Titanium White. It's an opaque white good for brightening colors.", QPoint(340, 20)},
        {"This is Cobalt Blue. It's a bright, cool blue with a slightly greenish undertone, good for mixing greens and purples.", QPoint(110, 20)},
        {"This is Cerulean Blue. It's a light, sky blue good for mixing light greens. Its slight opacity makes it ideal for softer, pastel tones.", QPoint(190, 20)},
        {"On the right shelf is Cadmium Deep Yellow. It's a rich, warm yellow with a deep orange undertone good for mixing vibrant oranges and earthy tones.", QPoint(260, 20)},
        {"Next to that is Cadmium Yellow. It's a bright, sunny yellow that adds vividness to mixes and is perfect for creating greens and oranges.", QPoint(340, 20)},
        {"Then we have Alizarin Crimson. It's a deep, cool red with a slight bluish tint, good for mixing "
         "rich purples, dark shadows, and muted tones, and often used to add depth as a glaze.", QPoint(340, 20)},
        {"Finally, we have Cadmium Red. It's a bold, warm red good for mixing oranges and vibrant purples.", QPoint(340, 20)},
        {"The cauldron shows you a pie chart of what ratio of color is currently in the pot.", QPoint(340, 20)},
        {"On the right, you can see the goal swatches to match. Follow the instructions for a guided walk-through of what to mix.", QPoint(340, 20)},
        {"If you ever make a mistake or want to start over, you can dump out the cauldron using this button.", QPoint(600, 20)},
        {"Happy color mixing!", QPoint(340, 20)}

    };

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::walkThroughLabels);

    timer->setSingleShot(true);
    timer->start(3000);
}

void MainWindow::walkThroughLabels(){
    if(labelsIdx == labels.count()){
        ui->intro_Label->hide();
    }
    else{
        ui->intro_Label->setText(labels[labelsIdx].text);
        ui->intro_Label->move(labels[labelsIdx].position);
        labelsIdx++;
        timer->start(3000);
    }
}

void MainWindow::newHint(QString hint){
    ui->hint->setText(hint);
}

void MainWindow::updateGoal(QColor color) {
    ui->goalColor->setStyleSheet(QString("background-color: "+color.name()+";"));
}

void MainWindow::updateInstructions(QImage image) {
    // TODO: implement instruction ui updating
}

MainWindow::~MainWindow()
{
    delete ui;
}
