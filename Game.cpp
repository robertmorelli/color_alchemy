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

struct coord {
    double x;
    double y;
    double z;
};

coord solidCoordinates(const QColor color) {
        // Get HSV values from QColor
        int hue, sat, val;
        color.getHsv(&hue, &sat, &val);
        
        // Normalize saturation and value to 0-1 range
        // QColor uses 0-255 for sat/val, but we need 0-1
        float saturation = sat / 255.0f;
        float value = val / 255.0f;
        
        // Convert hue to radians (QColor uses -1 for undefined hue)
        float hueRadians = (hue >= 0 ? hue : 0) * M_PI / 180.0f;
        
        // Calculate the radius at current height (value)
        // At v=0.5, radius is maximum (1.0)
        // At v=0 or v=1, radius is 0 (cone tips)
        float radius = (1.0f - std::abs(2.0f * value - 1.0f)) * saturation;
        
        // Calculate x and y coordinates based on hue angle and radius
        float x = radius * std::cos(hueRadians);
        float y = radius * std::sin(hueRadians);
        
        // Z coordinate is determined by value/brightness
        // Remap value from [0,1] to [-1,1] for bi-cone
        float z = 2.0f * value - 1.0f;
        
        return coord{x, y, z};
}


//function to produce a string hint of what is wrong (eg. the wrong hue/saturation/value or to muddy)
void Game::checkGoalHint(const QColor color, const QColor targetColor) {
    coord c1 = solidCoordinates(color);
    coord c2 = solidCoordinates(targetColor);
    int h1, s1, v1;
    color.getHsv(&h1, &s1, &v1);
    int h2, s2, v2;
    targetColor.getHsv(&h2, &s2, &v2);

    //find worst aspect
    double dh = std::abs(h1 - h2);
    if (dh > 180) dh = 360 - dh;
    dh = (dh * dh)/30;
    double ds = std::abs(s1 - s2) / 5;
    double dv = std::abs(v1 - v2) * 2;
    double targetMuddyness = std::sqrt(c1.x * c1.x + c1.y * c1.y + std::abs(c1.z - 210)) / 20;
    double muddyness = std::sqrt(c2.x * c2.x + c2.y * c2.y + std::abs(c2.z - 210)) / 20;
    double dm = std::abs(targetMuddyness - muddyness);

    qDebug() << "dh: " << dh << " ds: " << ds << " dv: " << dv << " dm: " << dm << Qt::endl;

    QString hint = "No hint";
    if (dh > ds && dh > dv && dh > dm) {
        hint = "Wrong hue";
    }
    else if (ds > dv && ds > dm) {
        if(s1 < s2) {
            hint = "Too dull";
        }
        else {
            hint = "Too much color";
        }
    }
    else if (dv > dm) {
        if(v1 > v2) {
            hint = "Too light";
        }
        else {
            hint = "Too dark";
        }
    }
    else {
        hint = "Too muddy";
    }
    emit newHint(hint);
}

double Game::hsvSolidDistanceSquared(const QColor color, const QColor targetColor) {
    coord c1 = solidCoordinates(color);
    coord c2 = solidCoordinates(targetColor);
    double dx = c1.x - c2.x;
    double dy = c1.y - c2.y;
    double dz = c1.z - c2.z;

    return dx * dx + dy * dy + dz * dz;
}

void Game::checkGoal(const QColor color) {
    const QColor& targetColor = colorGoals[currentLevel];
    double distance = hsvSolidDistanceSquared(color, targetColor);
    qDebug() << distance << Qt::endl;
    // Maybe we wnat a difficulty slider???
    const double threshold = 500;
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
