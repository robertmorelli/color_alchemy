#include "Game.h"
#include <QTimer>
#include <QColor>
#include <cmath>

Game::Game(QObject *parent) :
    gravity(0.0f, -10.0f)
    , world(gravity)
    , colorGoals()
    , instructions()
    , currentLevel(0)
{
    colorGoals.append(QColor("#5d5c5b"));
    colorGoals.append(QColor("#8c3455"));
    colorGoals.append(QColor("#FFFFFFF"));
}

double hsvSolidDistanceSquared(const QColor color, const QColor targetColor) {
    // Convert both colors to HSV
    int h1, s1, v1, h2, s2, v2;
    color.getHsv(&h1, &s1, &v1);
    targetColor.getHsv(&h2, &s2, &v2);

    //convert cylindrical coordinates to cartesian
    double x1 = s1 * cos(h1 * 2 * M_PI / 360);
    double y1 = s1 * sin(h1 * 2 * M_PI / 360);
    double z1 = std::log(v1+0.5) * 100; //make lightness perceptually linear

    double x2 = s2 * cos(h2 * 2 * M_PI / 360);
    double y2 = s2 * sin(h2 * 2 * M_PI / 360);
    double z2 = std::log(v2+0.5) * 100; //make lightness perceptually linear

    //calculate the distance between the two points
    double dx = x1 - x2;
    double dy = y1 - y2;
    double dz = z1 - z2;

    qDebug() << dx << " : " << dy << " : " << dz << Qt::endl;

    return dx * dx + dy * dy + dz * dz;
}

void Game::checkGoal(const QColor color) {
    const QColor& targetColor = colorGoals[currentLevel];
    double distance = hsvSolidDistanceSquared(color, targetColor);
    // Maybe we wnat a difficulty slider???
    const double threshold = 1000;
    if (distance > threshold) {
        return;
    }

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
