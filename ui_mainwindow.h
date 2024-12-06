/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Cauldron.h"
#include "colorflask.h"
#include "mixingpiechart.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QFrame *game;
    QLabel *instructions;
    QLabel *shelfRight;
    QLabel *shelfLeft;
    QLabel *ground;
    Cauldron *pot;
    QLabel *potColor;
    MixingPieChart *pieChart;
    QPushButton *reset;
    QLabel *hint;
    QFrame *goalList;
    QLabel *goalColor1;
    QLabel *goalColor2;
    QLabel *goalColor3;
    QLabel *goalColor4;
    QLabel *goal;
    QLabel *check1;
    QLabel *check2;
    QLabel *check3;
    QLabel *check4;
    QPushButton *tutorialNext;
    QPushButton *tutorialBack;
    QLabel *tutorialArrow;
    QPushButton *hintButton;
    QLabel *colorName;
    QLabel *intro_Label;
    ColorFlask *cobalt_blue;
    ColorFlask *cer_blue;
    ColorFlask *tit_white;
    ColorFlask *iv_black;
    ColorFlask *cad_deep_yellow;
    ColorFlask *cad_yel;
    ColorFlask *al_crimson;
    ColorFlask *cad_red;
    QPushButton *levelOneButton;
    QPushButton *levelTwoButton;
    QPushButton *levelThreeButton;
    QPushButton *tutorialSkip;
    QPushButton *instructionsReadMore;
    QPushButton *closeInstructions;
    QPushButton *skipColor;
    QScrollArea *instructionsPopup;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_2;
    QWidget *popupScrollContainer;
    QGridLayout *gridLayout;
    QVBoxLayout *popupVerticalLayout;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(950, 586);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8(""));
        game = new QFrame(centralwidget);
        game->setObjectName("game");
        game->setGeometry(QRect(-10, 0, 961, 541));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(game->sizePolicy().hasHeightForWidth());
        game->setSizePolicy(sizePolicy);
        game->setStyleSheet(QString::fromUtf8(".QFrame#game{\n"
"border-image: url(:/images/background.jpg) 0 0 0 0 stretch stretch\n"
"}"));
        game->setFrameShape(QFrame::Shape::StyledPanel);
        game->setFrameShadow(QFrame::Shadow::Raised);
        instructions = new QLabel(game);
        instructions->setObjectName("instructions");
        instructions->setGeometry(QRect(40, 220, 271, 231));
        instructions->setStyleSheet(QString::fromUtf8("border-image: url(:/images/instructions.png);"));
        shelfRight = new QLabel(game);
        shelfRight->setObjectName("shelfRight");
        shelfRight->setGeometry(QRect(560, 131, 441, 101));
        shelfRight->setStyleSheet(QString::fromUtf8("border-image: url(:/images/shelfRight.png) 0 0 0 0 stretch stretch;"));
        shelfLeft = new QLabel(game);
        shelfLeft->setObjectName("shelfLeft");
        shelfLeft->setGeometry(QRect(-10, 130, 461, 91));
        shelfLeft->setStyleSheet(QString::fromUtf8("image: url(:/images/shelfLeft.png);\n"
"border-image: none;\n"
""));
        ground = new QLabel(game);
        ground->setObjectName("ground");
        ground->setGeometry(QRect(-30, 460, 1081, 81));
        ground->setStyleSheet(QString::fromUtf8("image: url(:/images/ground.png) 0 0 0 0 stretch stretch;\n"
"border-image: none;\n"
""));
        pot = new Cauldron(game);
        pot->setObjectName("pot");
        pot->setGeometry(QRect(340, 143, 311, 341));
        pot->setStyleSheet(QString::fromUtf8(""));
        potColor = new QLabel(pot);
        potColor->setObjectName("potColor");
        potColor->setGeometry(QRect(130, 193, 51, 51));
        potColor->setStyleSheet(QString::fromUtf8("image: none;\n"
"background-color: #FFFFFF;\n"
"border-image: none;\n"
"border-radius: 25px;\n"
""));
        pieChart = new MixingPieChart(game);
        pieChart->setObjectName("pieChart");
        pieChart->setGeometry(QRect(580, 180, 131, 111));
        reset = new QPushButton(game);
        reset->setObjectName("reset");
        reset->setGeometry(QRect(900, 420, 61, 61));
        reset->setStyleSheet(QString::fromUtf8("    image: url(:/images/reset.png);\n"
"    background-color: transparent;  \n"
""));
        reset->setFlat(true);
        hint = new QLabel(game);
        hint->setObjectName("hint");
        hint->setGeometry(QRect(95, 389, 190, 10));
        hint->setStyleSheet(QString::fromUtf8("color: rgb(86, 65, 40);\n"
"font: 8pt \"Comic Sans MS\";\n"
"font-weight: 600;\n"
"text-transform: uppercase;"));
        goalList = new QFrame(game);
        goalList->setObjectName("goalList");
        goalList->setGeometry(QRect(690, 220, 221, 221));
        goalList->setStyleSheet(QString::fromUtf8("border: 2px solid transparent;\n"
""));
        goalList->setFrameShape(QFrame::Shape::StyledPanel);
        goalList->setFrameShadow(QFrame::Shadow::Raised);
        goalColor1 = new QLabel(goalList);
        goalColor1->setObjectName("goalColor1");
        goalColor1->setGeometry(QRect(45, 81, 50, 40));
        goalColor1->setStyleSheet(QString::fromUtf8("image: none;\n"
"background-color: #5d5c5b;\n"
"border-image: none;\n"
""));
        goalColor2 = new QLabel(goalList);
        goalColor2->setObjectName("goalColor2");
        goalColor2->setGeometry(QRect(120, 80, 61, 41));
        goalColor2->setStyleSheet(QString::fromUtf8("background-color: #d2F32F;"));
        goalColor3 = new QLabel(goalList);
        goalColor3->setObjectName("goalColor3");
        goalColor3->setGeometry(QRect(40, 140, 61, 51));
        goalColor3->setStyleSheet(QString::fromUtf8("background-color: #7e0521;"));
        goalColor4 = new QLabel(goalList);
        goalColor4->setObjectName("goalColor4");
        goalColor4->setGeometry(QRect(120, 140, 61, 51));
        goalColor4->setStyleSheet(QString::fromUtf8("background-color: #002f31;"));
        goal = new QLabel(goalList);
        goal->setObjectName("goal");
        goal->setGeometry(QRect(3, -10, 221, 241));
        goal->setStyleSheet(QString::fromUtf8("image: url(:/images/goal.png) 0 0 0 0 stretch stretch;\n"
""));
        check1 = new QLabel(goalList);
        check1->setObjectName("check1");
        check1->setEnabled(true);
        check1->setGeometry(QRect(28, 64, 30, 30));
        check1->setStyleSheet(QString::fromUtf8("border-image: url(:/images/check.png) 0 0 0 0 stretch stretch;"));
        check2 = new QLabel(goalList);
        check2->setObjectName("check2");
        check2->setEnabled(true);
        check2->setGeometry(QRect(108, 66, 30, 30));
        check2->setStyleSheet(QString::fromUtf8("border-image: url(:/images/check.png) 0 0 0 0 stretch stretch;"));
        check3 = new QLabel(goalList);
        check3->setObjectName("check3");
        check3->setEnabled(true);
        check3->setGeometry(QRect(27, 127, 30, 30));
        check3->setStyleSheet(QString::fromUtf8("border-image: url(:/images/check.png) 0 0 0 0 stretch stretch;"));
        check4 = new QLabel(goalList);
        check4->setObjectName("check4");
        check4->setEnabled(true);
        check4->setGeometry(QRect(109, 126, 30, 30));
        check4->setStyleSheet(QString::fromUtf8("border-image: url(:/images/check.png) 0 0 0 0 stretch stretch;"));
        tutorialNext = new QPushButton(game);
        tutorialNext->setObjectName("tutorialNext");
        tutorialNext->setGeometry(QRect(500, 130, 61, 29));
        tutorialNext->setStyleSheet(QString::fromUtf8("background-color: transparent;\n"
"color: #574128;\n"
"font: 8pt \"Comic Sans MS\";\n"
"font-weight: 600;\n"
""));
        tutorialNext->setFlat(true);
        tutorialBack = new QPushButton(game);
        tutorialBack->setObjectName("tutorialBack");
        tutorialBack->setGeometry(QRect(420, 130, 101, 29));
        tutorialBack->setStyleSheet(QString::fromUtf8("background-color: transparent;\n"
"color: #574128;\n"
"font: 8pt \"Comic Sans MS\";\n"
"font-weight: 600;\n"
""));
        tutorialBack->setFlat(true);
        tutorialArrow = new QLabel(game);
        tutorialArrow->setObjectName("tutorialArrow");
        tutorialArrow->setGeometry(QRect(600, 20, 41, 51));
        tutorialArrow->setStyleSheet(QString::fromUtf8("image: url(:/images/arrow.png);"));
        hintButton = new QPushButton(game);
        hintButton->setObjectName("hintButton");
        hintButton->setGeometry(QRect(60, 380, 31, 29));
        hintButton->setStyleSheet(QString::fromUtf8("image: url(:/images/lightbulb.png);\n"
"background-color: transparent;"));
        hintButton->setFlat(true);
        colorName = new QLabel(game);
        colorName->setObjectName("colorName");
        colorName->setGeometry(QRect(350, 50, 291, 61));
        QFont font;
        font.setFamilies({QString::fromUtf8("Comic Sans MS")});
        font.setPointSize(24);
        font.setBold(true);
        colorName->setFont(font);
        colorName->setAlignment(Qt::AlignmentFlag::AlignCenter);
        intro_Label = new QLabel(game);
        intro_Label->setObjectName("intro_Label");
        intro_Label->setGeometry(QRect(350, 10, 301, 121));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Comic Sans MS")});
        font1.setPointSize(8);
        font1.setWeight(QFont::DemiBold);
        font1.setItalic(false);
        intro_Label->setFont(font1);
        intro_Label->setAutoFillBackground(false);
        intro_Label->setStyleSheet(QString::fromUtf8("border-image: url(:/images/tutorial.png) 0 0 0 0 stretch stretch;\n"
"color: rgb(86, 65, 40);\n"
"font: 8pt \"Comic Sans MS\";\n"
"font-weight: 600;\n"
"padding: 15px\n"
""));
        cobalt_blue = new ColorFlask(game);
        cobalt_blue->setObjectName("cobalt_blue");
        cobalt_blue->setGeometry(QRect(260, 60, 63, 78));
        cobalt_blue->setStyleSheet(QString::fromUtf8(""));
        cer_blue = new ColorFlask(game);
        cer_blue->setObjectName("cer_blue");
        cer_blue->setGeometry(QRect(180, 60, 63, 78));
        cer_blue->setStyleSheet(QString::fromUtf8(""));
        tit_white = new ColorFlask(game);
        tit_white->setObjectName("tit_white");
        tit_white->setGeometry(QRect(100, 60, 63, 78));
        tit_white->setStyleSheet(QString::fromUtf8(""));
        iv_black = new ColorFlask(game);
        iv_black->setObjectName("iv_black");
        iv_black->setGeometry(QRect(20, 60, 63, 78));
        iv_black->setStyleSheet(QString::fromUtf8(""));
        cad_deep_yellow = new ColorFlask(game);
        cad_deep_yellow->setObjectName("cad_deep_yellow");
        cad_deep_yellow->setGeometry(QRect(660, 70, 63, 78));
        cad_deep_yellow->setStyleSheet(QString::fromUtf8(""));
        cad_yel = new ColorFlask(game);
        cad_yel->setObjectName("cad_yel");
        cad_yel->setGeometry(QRect(742, 70, 63, 78));
        cad_yel->setStyleSheet(QString::fromUtf8(""));
        al_crimson = new ColorFlask(game);
        al_crimson->setObjectName("al_crimson");
        al_crimson->setGeometry(QRect(820, 70, 63, 78));
        al_crimson->setStyleSheet(QString::fromUtf8(""));
        cad_red = new ColorFlask(game);
        cad_red->setObjectName("cad_red");
        cad_red->setGeometry(QRect(891, 70, 63, 78));
        cad_red->setStyleSheet(QString::fromUtf8(""));
        levelOneButton = new QPushButton(game);
        levelOneButton->setObjectName("levelOneButton");
        levelOneButton->setGeometry(QRect(120, 490, 93, 29));
        levelOneButton->setStyleSheet(QString::fromUtf8("background-color: #574128;\n"
"color: #cebf9e;\n"
"font: 8pt \"Comic Sans MS\";\n"
"font-weight: 600;\n"
""));
        levelTwoButton = new QPushButton(game);
        levelTwoButton->setObjectName("levelTwoButton");
        levelTwoButton->setGeometry(QRect(230, 490, 93, 29));
        levelTwoButton->setStyleSheet(QString::fromUtf8("background-color: #574128;\n"
"color: #cebf9e;\n"
"font: 8pt \"Comic Sans MS\";\n"
"font-weight: 600;\n"
""));
        levelThreeButton = new QPushButton(game);
        levelThreeButton->setObjectName("levelThreeButton");
        levelThreeButton->setGeometry(QRect(340, 490, 93, 29));
        levelThreeButton->setStyleSheet(QString::fromUtf8("background-color: #574128;\n"
"color: #cebf9e;\n"
"font: 8pt \"Comic Sans MS\";\n"
"font-weight: 600;\n"
""));
        tutorialSkip = new QPushButton(game);
        tutorialSkip->setObjectName("tutorialSkip");
        tutorialSkip->setGeometry(QRect(640, 10, 51, 29));
        tutorialSkip->setStyleSheet(QString::fromUtf8("background-color: transparent;\n"
"color: #574128;\n"
"font: 8pt \"Comic Sans MS\";\n"
"font-weight: 600;\n"
""));
        tutorialSkip->setFlat(true);
        instructionsReadMore = new QPushButton(game);
        instructionsReadMore->setObjectName("instructionsReadMore");
        instructionsReadMore->setGeometry(QRect(120, 260, 111, 29));
        instructionsReadMore->setStyleSheet(QString::fromUtf8("background-color: transparent;\n"
"color: rgb(86, 65, 40);\n"
"font: 8pt \"Comic Sans MS\";\n"
"font-weight: 600;\n"
""));
        instructionsReadMore->setFlat(true);
        closeInstructions = new QPushButton(game);
        closeInstructions->setObjectName("closeInstructions");
        closeInstructions->setGeometry(QRect(630, 40, 61, 41));
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(closeInstructions->sizePolicy().hasHeightForWidth());
        closeInstructions->setSizePolicy(sizePolicy1);
        closeInstructions->setStyleSheet(QString::fromUtf8("background-color: transparent;\n"
"color: #574128;\n"
"font: 8pt \"Comic Sans MS\";\n"
"font-weight: 600;\n"
""));
        closeInstructions->setFlat(true);
        skipColor = new QPushButton(game);
        skipColor->setObjectName("skipColor");
        skipColor->setGeometry(QRect(750, 440, 93, 29));
        skipColor->setStyleSheet(QString::fromUtf8("background-color: #574128;\n"
"color: #cebf9e;\n"
"font: 8pt \"Comic Sans MS\";\n"
"font-weight: 600;\n"
""));
        instructionsPopup = new QScrollArea(game);
        instructionsPopup->setObjectName("instructionsPopup");
        instructionsPopup->setGeometry(QRect(330, 40, 371, 451));
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(instructionsPopup->sizePolicy().hasHeightForWidth());
        instructionsPopup->setSizePolicy(sizePolicy2);
        instructionsPopup->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
        instructionsPopup->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 354, 449));
        gridLayout_2 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_2->setObjectName("gridLayout_2");
        popupScrollContainer = new QWidget(scrollAreaWidgetContents);
        popupScrollContainer->setObjectName("popupScrollContainer");
        gridLayout = new QGridLayout(popupScrollContainer);
        gridLayout->setObjectName("gridLayout");
        popupVerticalLayout = new QVBoxLayout();
        popupVerticalLayout->setObjectName("popupVerticalLayout");
        popupVerticalLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinAndMaxSize);

        gridLayout->addLayout(popupVerticalLayout, 0, 0, 1, 1);


        gridLayout_2->addWidget(popupScrollContainer, 0, 0, 1, 1);

        instructionsPopup->setWidget(scrollAreaWidgetContents);
        instructions->raise();
        shelfRight->raise();
        shelfLeft->raise();
        ground->raise();
        pot->raise();
        pieChart->raise();
        reset->raise();
        hint->raise();
        goalList->raise();
        tutorialNext->raise();
        tutorialBack->raise();
        tutorialArrow->raise();
        hintButton->raise();
        colorName->raise();
        intro_Label->raise();
        cobalt_blue->raise();
        cer_blue->raise();
        tit_white->raise();
        iv_black->raise();
        cad_deep_yellow->raise();
        cad_yel->raise();
        al_crimson->raise();
        cad_red->raise();
        levelOneButton->raise();
        levelTwoButton->raise();
        levelThreeButton->raise();
        tutorialSkip->raise();
        instructionsReadMore->raise();
        skipColor->raise();
        instructionsPopup->raise();
        closeInstructions->raise();
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 950, 69));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        instructions->setText(QString());
        shelfRight->setText(QString());
        shelfLeft->setText(QString());
        ground->setText(QString());
        potColor->setText(QString());
        reset->setText(QString());
        hint->setText(QCoreApplication::translate("MainWindow", "no hint avaliable", nullptr));
        goalColor1->setText(QString());
        goalColor2->setText(QString());
        goalColor3->setText(QString());
        goalColor4->setText(QString());
        goal->setText(QString());
        check1->setText(QString());
        check2->setText(QString());
        check3->setText(QString());
        check4->setText(QString());
        tutorialNext->setText(QCoreApplication::translate("MainWindow", "NEXT", nullptr));
        tutorialBack->setText(QCoreApplication::translate("MainWindow", "BACK", nullptr));
        tutorialArrow->setText(QString());
        hintButton->setText(QString());
        colorName->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#ffffff;\">titanium white</span></p></body></html>", nullptr));
        intro_Label->setText(QCoreApplication::translate("MainWindow", " Welcome to ColorAlchemy!  ", nullptr));
        levelOneButton->setText(QCoreApplication::translate("MainWindow", "Level One", nullptr));
        levelTwoButton->setText(QCoreApplication::translate("MainWindow", "Level Two", nullptr));
        levelThreeButton->setText(QCoreApplication::translate("MainWindow", "Level Three", nullptr));
        tutorialSkip->setText(QCoreApplication::translate("MainWindow", "SKIP", nullptr));
        instructionsReadMore->setText(QCoreApplication::translate("MainWindow", "learn more", nullptr));
        closeInstructions->setText(QCoreApplication::translate("MainWindow", "close", nullptr));
        skipColor->setText(QCoreApplication::translate("MainWindow", "Skip Color", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
