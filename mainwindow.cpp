#include "mainwindow.h"
#include "Cauldron.h"
#include "Game.h"
#include "QRegularExpression"
#include "colorflask.h"
#include "ui_mainwindow.h"
#include <QColor>
#include <QSoundEffect>
#include <QString>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , game()
    , colorNamer(this)
    , hintVisiblity(false) {
    ui->setupUi(this);

    ui->tit_white->changeColor(QColor("#FFFFFF"));
    ui->cad_yel->changeColor(QColor("#FDDA0D"));
    ui->cad_deep_yellow->changeColor(QColor("#f6af15"));
    ui->cad_red->changeColor(QColor("#D22B2B"));
    ui->al_crimson->changeColor(QColor("#E32636"));
    ui->cer_blue->changeColor(QColor("#2997bc"));
    ui->cobalt_blue->changeColor(QColor("#0047AB"));
    ui->iv_black->changeColor(QColor("#0C0B0A"));

    ui->hint->hide();
    ui->instructionsPopup->hide();
    ui->closeInstructions->hide();

    // connect cauldron effect to complete
    connect(&game, &Game::cauldronEffect, ui->pot, &Cauldron::complete);

    connect(ui->closeInstructions, &QPushButton::clicked, ui->potColor, [this]() {
        ui->instructionsPopup->hide();
        ui->closeInstructions->hide();
    });

    connect(ui->instructionsReadMore, &QPushButton::clicked, ui->instructionsPopup, [this]() {
        ui->instructionsPopup->show();
        ui->closeInstructions->show();
        // probably add a QWidget[] to the Level struct to store the instructions
        // and then add them to the scroll thing here or something
        for (int i = 0; i < 1000; ++i) {
            QLabel* contentWidget = new QLabel();
            // y no scroll :/
            contentWidget->setText("hi");
            ui->InstructionsContent->addWidget(contentWidget);
        }
    });

    // find all flasks and connect them
    for (ColorFlask* flask : this->findChildren<ColorFlask*>())
        connect(flask, &ColorFlask::addToCauldron, ui->pot, &Cauldron::addColor);
    connect(ui->reset, &QPushButton::clicked, ui->pot, &Cauldron::dump);
    connect(ui->pot, &Cauldron::newMixedColor, &game, &Game::checkGoal);
    connect(ui->pot, &Cauldron::newMixedColor, ui->potColor, [this](QColor color) {
        ui->potColor->setStyleSheet(QString("background-color:" + color.name()
            + ";"
              "border-radius: 25px;"));
        ui->colorName->setStyleSheet(QString("color: " + color.name() + ";"));
        ui->colorName->setText("..."); // Show loading state
        colorNamer.requestColorName(color); // this will not hang the ui (singleshot inside)
    });

    connect(&colorNamer, &ColorNamer::colorNameReceived, this, [this](const QString& name) { ui->colorName->setText(name); });
    connect(ui->pot, &Cauldron::colorsChanged, ui->pieChart, &MixingPieChart::updatePieSeries);

    connect(&game, &Game::newGoals, this, &MainWindow::updateGoals);
    connect(&game, &Game::newInstructions, this, &MainWindow::updateInstructions);
    connect(&game, &Game::correctColor, this, &MainWindow::showCheckmark);
    connect(&game, &Game::newHint, this, &MainWindow::newHint);

    connect(ui->tutorialNext, &QPushButton::clicked, this, &MainWindow::nextInstruction);
    connect(ui->tutorialBack, &QPushButton::clicked, this, &MainWindow::backInstruction);
    connect(ui->tutorialSkip, &QPushButton::clicked, this, &MainWindow::skipInstructions);
    connect(ui->hintButton, &QPushButton::clicked, this, &MainWindow::toggleHint);

    ui->intro_Label->setWordWrap(true);
    ui->tutorialArrow->hide();
    ui->tutorialBack->hide();

    connect(ui->levelOneButton, &QPushButton::clicked,[&]() { game.setIndex(0);});
    connect(ui->levelTwoButton, &QPushButton::clicked,[&]() { game.setIndex(0);});
    connect(ui->levelThreeButton, &QPushButton::clicked, [&]() { game.setIndex(0);});

    //user cannot acces higher levels until completing level 1
    ui->levelTwoButton->setEnabled(false);
    ui->levelThreeButton->setEnabled(false);

    connect(&game, &Game::newLevel, this, [this](int newLevelIdx) {
        if(newLevelIdx == 1)
            ui->levelTwoButton->setEnabled(true);
        else if (newLevelIdx == 2)
            ui->levelTwoButton->setEnabled(true);
    });

    labelsIdx = 0;
    labels = { { "Here are your flasks of the 8 basic oil painting colors. "
                 "You can drag and drop them into the cauldron to mix colors.",
                   QPoint(325, 10) },
        { "This is Ivory Black. It has a slight warm tone, and is good for darkening shadows. "
          "Mixed with white, it is good for making neutral grays.",
            QPoint(30, 10) },
        { "Here is Titanium White. It's an opaque white good for brightening colors.", QPoint(110, 10) },
        { "This is Cobalt Blue. It's a bright, cool blue with a slightly greenish undertone, good "
          "for mixing greens and purples.",
            QPoint(190, 10) },
        { "This is Cerulean Blue. It's a light, sky blue good for mixing light greens. Its slight "
          "opacity makes it ideal for softer, pastel tones.",
            QPoint(270, 10) },
        { "On the right shelf is Cadmium Deep Yellow. It's a rich, warm yellow with a deep orange "
          "undertone good for mixing vibrant oranges and earthy tones.",
            QPoint(675, 10) },
        { "Next to that is Cadmium Yellow. It's a bright, sunny yellow that adds vividness to "
          "mixes and is perfect for creating greens and oranges.",
            QPoint(752, 10) },
        { "Then we have Alizarin Crimson. It's a deep, cool red with a slight bluish tint, good "
          "for mixing "
          "rich purples, dark shadows, and muted tones, and often used to add depth as a glaze.",
            QPoint(830, 10) },
        { "Finally, we have Cadmium Red. It's a bold, warm red good for mixing oranges and vibrant "
          "purples.",
            QPoint(905, 10) },
        { "The cauldron shows you a pie chart of what ratio of color is currently in the pot.", QPoint(625, 150) },
        { "On the right, you can see the goal swatches to match. Follow the instructions for a "
          "guided walk-through of what to mix.",
            QPoint(740, 245) },
        { "If you are stuck on how to create a question, toggle the hint button to show how far "
          "you are from the goal.",
            QPoint(50, 320) },
        { "If you ever make a mistake or want to start over, you can dump out the cauldron using "
          "this button.",
            QPoint(910, 370) },
        { "Happy color mixing!", QPoint(340, 20) } };

    glassSound.setSource(QUrl("qrc:/sounds/glass.wav"));
    glassSound.setVolume(10.0);

    boilingSound.setSource(QUrl("qrc:/sounds/boiling.wav"));
    boilingSound.setVolume(0.5);
    boilingSound.setLoopCount(QSoundEffect::Infinite);
    boilingSound.play();

    connect(ui->tit_white, &ColorFlask::makeSound, this, &MainWindow::playGlassSound);
    connect(ui->cad_yel, &ColorFlask::makeSound, this, &MainWindow::playGlassSound);
    connect(ui->cad_deep_yellow, &ColorFlask::makeSound, this, &MainWindow::playGlassSound);
    connect(ui->cad_red, &ColorFlask::makeSound, this, &MainWindow::playGlassSound);
    connect(ui->al_crimson, &ColorFlask::makeSound, this, &MainWindow::playGlassSound);
    connect(ui->cer_blue, &ColorFlask::makeSound, this, &MainWindow::playGlassSound);
    connect(ui->cobalt_blue, &ColorFlask::makeSound, this, &MainWindow::playGlassSound);
    connect(ui->iv_black, &ColorFlask::makeSound, this, &MainWindow::playGlassSound);

    game.update();
}

void MainWindow::backInstruction() {
    if (labelsIdx > 0) {
        labelsIdx--;
        walkThroughLabels();
    }
}

void MainWindow::nextInstruction() {
    labelsIdx++;
    walkThroughLabels();
}

void MainWindow::skipInstructions() {
    labelsIdx = labels.count();
    walkThroughLabels();
}

void MainWindow::toggleHint() {
    if (hintVisiblity) {
        ui->hint->show();
    } else {
        ui->hint->hide();
    }
    hintVisiblity = !hintVisiblity;
}

void MainWindow::walkThroughLabels() {
    if (labelsIdx == 0) {
        ui->tutorialBack->hide();
        ui->tutorialArrow->hide();
    } else {
        ui->tutorialArrow->show();
        ui->tutorialBack->show();
    }
    if (labelsIdx == labels.count() - 1) ui->tutorialArrow->hide();
    if (labelsIdx == labels.count()) {
        ui->intro_Label->hide();
        ui->tutorialNext->hide();
        ui->tutorialSkip->hide();
        ui->tutorialBack->hide();
        ui->tutorialArrow->hide();
    } else {
        ui->intro_Label->setText(labels[labelsIdx].text);
        QPoint currentLocation = labels[labelsIdx].position;
        ui->tutorialArrow->move(currentLocation);
    }
}
void MainWindow::playGlassSound() {
    glassSound.play();
    // qDebug() << "playing sound";
}

void MainWindow::newHint(QString hint) { ui->hint->setText(hint); }

void MainWindow::updateGoals(Goal goals[4]) {
    ui->goalColor1->setStyleSheet(QString("background-color: " + goals[0].color.name() + ";"));
    ui->goalColor2->setStyleSheet(QString("background-color: " + goals[1].color.name() + ";"));
    ui->goalColor3->setStyleSheet(QString("background-color: " + goals[2].color.name() + ";"));
    ui->goalColor4->setStyleSheet(QString("background-color: " + goals[3].color.name() + ";"));

    ui->check1->setVisible(false);
    ui->check2->setVisible(false);
    ui->check3->setVisible(false);
    ui->check4->setVisible(false);

    for (int i = 0; i < 4; i++) {
        if (goals[i].done) {
            showCheckmark(i);
        }
    }
}

void MainWindow::updateInstructions(QString& imagePath) { ui->instructions->setStyleSheet(QString("border-image: url(" + imagePath + ");")); }

void MainWindow::showCheckmark(int idx) {
    switch (idx) {
    case 0:
        ui->check1->setVisible(true);
        break;
    case 1:
        ui->check2->setVisible(true);
        break;
    case 2:
        ui->check3->setVisible(true);
        break;
    case 3:
        ui->check4->setVisible(true);
        break;
    default:
        return;
    }
}

MainWindow::~MainWindow() { delete ui; }
