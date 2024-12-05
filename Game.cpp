#include "Game.h"
#include "vivid/color.h"
#include "vivid/types.h"
#include <QColor>
#include <QString>
#include <QTimer>
#include <cmath>
#include <vivid/color.h>
#include <vivid/types.h>

Game::Game(QObject* parent)
    : gravity(0.0f, -10.0f)
    , world(gravity)
    , levels()
    , currentLevel(0)
    , currentSection(0) {
    Q_UNUSED(parent);

    Level level1 = { {
        { { { QColor("#90a369") }, { QColor("#dd5724") }, { QColor("#5ea891") }, { QColor("#9e324b") } }, ":/images/instruction1_1.png" }, // Section 1
        { { { QColor("#9f7362") }, { QColor("#778f88") }, { QColor("#7a565e") }, { QColor("#999a69") } }, ":/images/instruction1_2.png" }, // Section 2
        { { { QColor("#66546b") }, { QColor("#4d7160") }, { QColor("#752848") }, { QColor("#363f63") } }, ":/images/instruction1_3.png" }, // Section 3
        { { { QColor("#efa017") }, { QColor("#f5c674") }, { QColor("#546d9b") }, { QColor("#b1543f") } }, ":/images/instruction1_4.png" }  // Section 4
    } };

    Level level2 = { {
        { { { QColor("#b1b056") }, { QColor("#60948c") }, { QColor("#87956f") }, { QColor("#948a5f") } }, ":/images/instruction2_1.png" }, // Section 1
        { { { QColor("#aeb45c") }, { QColor("#747d78") }, { QColor("#c2b84d") }, { QColor("#93b965") } }, ":/images/instruction2_2.png" }, // Section 2
        { { { QColor("#a39657") }, { QColor("#9eaf88") }, { QColor("#7e9a74") }, { QColor("#cfdec9") } }, ":/images/instruction2_3.png" }  // Section 3
    } };

    Level level3 = { {
        { { { QColor("#ceb3a1") }, { QColor("#f0d2b2") }, { QColor("#a07a73") }, { QColor("#6e534d") } }, ":/images/instruction1_1.png" }, // Section 1
        { { { QColor("#4d423f") }, { QColor("#f2d38c") }, { QColor("#f7e8d5") }, { QColor("#8a7145") } }, ":/images/instruction1_1.png" }, // Section 2
        { { { QColor("#f2dee3") }, { QColor("#f3e7da") }, { QColor("#a07977") }, { QColor("#bf907a") } }, ":/images/instruction1_1.png" }, // Section 3
        { { { QColor("#362d1e") }, { QColor("#b4a9a5") }, { QColor("#969590") }, { QColor("#6c6460") } }, ":/images/instruction1_1.png" }  // Section 4
    } };

    levels.append(level1);
    levels.append(level2);
    levels.append(level3);
}

double Game::colorDistanceInOKLAB(QColor target, QColor current) {
    // Construct vivid::Color objects from the RGB values of the QColors
    // Replace the ambiguous constructor calls with explicit ones
    vivid::Color targetColor(
        vivid::rgb_t { static_cast<float>(target.red()) / 255.0f, static_cast<float>(target.green()) / 255.0f, static_cast<float>(target.blue()) / 255.0f });

    vivid::Color currentColor(
        vivid::rgb_t { static_cast<float>(current.red()) / 255.0f, static_cast<float>(current.green()) / 255.0f, static_cast<float>(current.blue()) / 255.0f });

    const vivid::col_t targetValue = targetColor.oklab().value();
    const vivid::col_t currentValue = currentColor.oklab().value();

    double deltaL = targetValue[0] - currentValue[0];
    double deltaA = targetValue[1] - currentValue[1];
    double deltaB = (targetValue[2] - currentValue[2]) / 2.5;
    return std::sqrt(deltaL * deltaL + deltaA * deltaA + deltaB * deltaB);
}

// function to produce a string hint of what is wrong (eg. the wrong hue/saturation/value or to
// muddy)
QString Game::checkGoalHint(const QColor color, const QColor targetColor) {
    // Convert QColor to vivid::Color using col8_t
    vivid::Color c1(vivid::col8_t { static_cast<uint8_t>(color.red()), static_cast<uint8_t>(color.green()), static_cast<uint8_t>(color.blue()) });

    vivid::Color c2(
        vivid::col8_t { static_cast<uint8_t>(targetColor.red()), static_cast<uint8_t>(targetColor.green()), static_cast<uint8_t>(targetColor.blue()) });

    // Convert both colors to Oklab
    const auto& c1Val = c1.oklab().value();
    const auto& c2Val = c2.oklab().value();

    // Compute Oklab distance components
    double dl = c1Val[0] - c2Val[0];
    double da = c1Val[1] - c2Val[1];
    double db = c1Val[2] - c2Val[2];

    // Normalize lightness, a, and b for hint analysis
    double dh = 1.6 * std::abs(da);
    double ds = 0.4 * std::abs(db);
    double dL = 0.8 * std::abs(dl);

    // Analyze distance components
    QString hint = "No hint";

    if (dh > ds && dh > dL) {
        hint = "Incorrect hue";
        // TODO: Add more hints about the specific hue issue
    } else if (ds > dL) {
        if (c1Val[1] < c2Val[1]) {
            hint = "Too little color";
        } else {
            hint = "Too much color";
        }
    } else {
        if (c1Val[0] > c2Val[0]) {
            hint = "Too light";
        } else {
            hint = "Too dark";
        }
    }
    return hint;
}

void Game::checkGoal(const QColor color) {
    for (int i = 0; i < 4; i++) {
        Goal& goal = levels[currentLevel].sections[currentSection].goals[i];
        const QColor& targetColor = goal.color;

        double distance = colorDistanceInOKLAB(color, targetColor);
        // Maybe we want a difficulty slider???
        const double threshold = 0.03;
        if (distance > threshold && !goal.done) {
            emit newHint(checkGoalHint(color, targetColor));
            continue;
        }
        emit newHint("Correct!");
        emit cauldronEffect();
        emit correctColor(i);
        goal.done = true;
    }

    for (Goal& goal : levels[currentLevel].sections[currentSection].goals) {
        if (!goal.done) {
            return;
        }
    }

    checkNewLevel();

}

void Game::skipPressed(){
    for (int i = 0; i < 4; i++) {
        Goal& goal = levels[currentLevel].sections[currentSection].goals[i];
        goal.done = true;
    }

    if(currentLevel == 2 && currentSection == 3)
        emit lastSection();
    else
        checkNewLevel();
}

void Game::checkNewLevel(){
    currentSection++;

    if (currentSection >= levels[currentLevel].sections.size()) {
        currentSection = 0;
        currentLevel++;
        emit newLevel(currentLevel);
    }
    update();
}

void Game::update() {
    emit newGoals(levels[currentLevel].sections[currentSection].goals);
    emit newInstructions(levels[currentLevel].sections[currentSection].instructions);
}

void Game::setIndex(int idx){
    currentLevel = idx;
}
void Game::setSection(int idx){
    currentSection = idx;
}
