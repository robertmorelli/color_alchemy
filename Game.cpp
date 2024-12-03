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
    , colorGoals()
    , instructions()
    , currentLevel(0) {
    Q_UNUSED(parent);
    colorGoals.append(QColor("#5d5c5b"));
    colorGoals.append(QColor("#8c3455"));
    colorGoals.append(QColor("#FFFFFFF"));
}

struct coord {
    double x;
    double y;
    double z;
};
// TODO: add to header file
/// @brief Compute the distance between two colors in the OKLAB color space
/// @param target the target color
/// @param current the current color
/// @return the distance between the two colors (with some scaling)
double colorDistanceInOKLAB(QColor target, QColor current) {
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
    const QColor& targetColor = colorGoals[currentLevel];
    double distance = colorDistanceInOKLAB(color, targetColor);
    // Maybe we want a difficulty slider???
    const double threshold = 0.04;
    if (distance > threshold) {
        emit newHint(checkGoalHint(color, targetColor));
        return;
    }
    emit newHint("Correct!");
    currentLevel++;
    emit newGoal(colorGoals[currentLevel]);
}

// // Define the gravity vector.

// // Define the ground body.
// b2BodyDef groundBodyDef;
// groundBodyDef.position.Set(0.0f, -10.0f);

// // Call the body factory which allocates memory for the ground body
// // from a pool and creates the ground box shape (also from a pool).
// // The body is also added to the world.
// groundBody = world.CreateBody(&groundBodyDef);

// // Define the ground box shape.
// b2PolygonShape groundBox;

// // The extents are the half-widths of the box.
// groundBox.SetAsBox(50.0f, 10.0f);

// // Add the ground fixture to the ground body.
// groundBody->CreateFixture(&groundBox, 0.0f);

// // Define the dynamic body. We set its position and call the body factory.
// b2BodyDef bodyDef;
// bodyDef.type = b2_dynamicBody;
// bodyDef.position.Set(0.0f, 4.0f);
// b2Body* body = world.CreateBody(&bodyDef);

// // Define another box shape for our dynamic body.
// b2PolygonShape dynamicBox;
// dynamicBox.SetAsBox(1.0f, 1.0f);

// // Define the dynamic body fixture.
// b2FixtureDef fixtureDef;
// fixtureDef.shape = &dynamicBox;

// // Set the box density to be non-zero, so it will be dynamic.
// fixtureDef.density = 1.0f;

// // Override the default friction.
// fixtureDef.friction = 0.3f;

// fixtureDef.restitution = 0.9f;

// // Add the shape to the body.
// body->CreateFixture(&fixtureDef);

// float32 timeStep = 1.0f / 60.0f;
// int32 velocityIterations = 6;
// int32 positionIterations = 2;

// // This is our little game loop.
// for (int32 i = 0; i < 60; ++i)
// {
//     // Instruct the world to perform a single step of simulation.
//     // It is generally best to keep the time step and iterations fixed.
//     world.Step(timeStep, velocityIterations, positionIterations);

//     // Now print the position and angle of the body.
//     b2Vec2 position = body->GetPosition();
//     float32 angle = body->GetAngle();

//     printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
// }
