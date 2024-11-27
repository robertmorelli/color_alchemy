#include "Game.h"
#include <QTimer>
#include <QColor>
#include <cmath>

Game::Game(QObject* parent) :
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
    int hue, sat, light;
    color.getHsl(&hue, &sat, &light);

    double saturation = sat / 255.0f;
    double value = 2.0f * (light / 255.0f) - 1.0f;
    double hueRadians = hue * 2 * M_PI / 360.0f;

    //make bicone
    double x = saturation * (1.0f - std::abs(value)) * std::cos(hueRadians);
    double y = saturation * (1.0f - std::abs(value)) * std::sin(hueRadians);
    double z = value;

    return coord{ 255 * x, 255 * y, 255 * z };
}


//function to produce a string hint of what is wrong (eg. the wrong hue/saturation/value or to muddy)
QString Game::checkGoalHint(const QColor color, const QColor targetColor) {
    coord c1 = solidCoordinates(color);
    coord c2 = solidCoordinates(targetColor);
    int h1, s1, l1;
    color.getHsl(&h1, &s1, &l1);
    int h2, s2, l2;
    targetColor.getHsl(&h2, &s2, &l2);

    //find worst aspect
    //hue scales to 2 because its more important
    double dh = 1.5f * (std::min(std::abs(h1 - h2 + 0.0f), std::abs(h1 - h2 + 360.0f)) / 360.0f);
    double ds = 0.7 * (std::abs(s1 - s2) / 255.0f);
    double dl = std::abs(l1 - l2) / 255.0f;

    //if c2 is close to greyscale then we shouldnt care about hue
    //use x and y to calculate distance from grey
    //this is a clumn 1/5 of the bicone
    //sqrt(2*255^2) = 360
    if (std::sqrt(c2.x * c2.x + c2.y * c2.y) < 360.0f / 5.0f) {
        dh = 0;
    }

    //muddyness is distance from grey (origin) sort of
    double targetMuddyness = std::sqrt(c2.x * c2.x + c2.y * c2.y + std::abs(c2.z));
    double muddyness = std::sqrt(c1.x * c1.x + c1.y * c1.y + std::abs(c1.z));
    //sqrt(2*255^2+255) = 361
    double dm = 0.7 * (std::abs(targetMuddyness - muddyness) / 361);

    QString hint = "No hint";
    if (dh > ds && dh > dl && dh > dm) {
        QString pigment;

        coord yellow = solidCoordinates(QColor("#FFFF00"));
        coord red = solidCoordinates(QColor("#FF0000"));
        coord blue = solidCoordinates(QColor("#0000FF"));

        double dy = (c1.x - yellow.x) * (c1.x - yellow.x) + (c1.y - yellow.y) * (c1.y - yellow.y) + (c1.z - yellow.z) * (c1.z - yellow.z);
        double dr = (c1.x - red.x) * (c1.x - red.x) + (c1.y - red.y) * (c1.y - red.y) + (c1.z - red.z) * (c1.z - red.z);
        double db = (c1.x - blue.x) * (c1.x - blue.x) + (c1.y - blue.y) * (c1.y - blue.y) + (c1.z - blue.z) * (c1.z - blue.z);

        if (dy < dr && dy < db) {
            pigment = "yellow";
        }
        else if (dr < db) {
            pigment = "red";
        }
        else {
            pigment = "blue";
        }
        hint = "Too much " + pigment;
    }
    else if (ds > dl && ds > dm) {
        if (s1 < s2) {
            hint = "Too dull";
        }
        else {
            hint = "Too much color";
        }
    }
    else if (dl > dm) {
        if (l1 > l2) {
            hint = "Too light";
        }
        else {
            hint = "Too dark";
        }
    }
    else {
        hint = "Too muddy";
    }
    return hint;
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
    // Maybe we wnat a difficulty slider???
    const double threshold = 2000;
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
