#include "Cauldron.h"
#include <QColor>
#include <QPainter>
#include <QPaintEvent>
#include <QImage>
#include <Box2D/Box2D.h>
#include <QTimer>
#include <qrandom.h>

Cauldron::Cauldron(QWidget* parent) : QWidget{ parent }, image(":/images/pot.png"), totalColors{ 0 }, world(b2Vec2(0.0f, 10.0f)), timer(this) {
    connect(&timer, &QTimer::timeout, this, &Cauldron::simulatePhysics);
    timer.start(8);
}

void Cauldron::addColor(QColor newColor) {
    if (colors.contains(newColor.name())) {
        colors[newColor.name()]++;
    }
    else {
        colors.insert(newColor.name(), 1);
    }
    totalColors++;

    emit newMixedColor(getColor());
    emit colorsChanged(colors, totalColors);
}

QColor Cauldron::getColor() {
    if (!totalColors) return Qt::white;
    double reds = 0, greens = 0, blues = 0;
    foreach(const QString & key, colors.keys()) {
        double weight = static_cast<double>(colors[key]) / static_cast<double>(totalColors);
        QColor color(key);
        reds += weight * (1.0 - (static_cast<double>(color.red()) / 255.0));
        greens += weight * (1.0 - (static_cast<double>(color.green()) / 255.0));
        blues += weight * (1.0 - (static_cast<double>(color.blue()) / 255.0));
    }
    return QColor(255 * (1 - reds), 255 * (1 - greens), 255 * (1 - blues));
}

void Cauldron::dump() {
    colors.clear();
    totalColors = 0;
    emit newMixedColor(getColor());
    emit colorsChanged(colors, totalColors);
}

void Cauldron::simulatePhysics() {
    world.Step(1.0f / 60.0f, 6, 2);

    for (b2Body* body = world.GetBodyList(); body;) {
        b2Body* nextBody = body->GetNext();

        //remove bubbles that are too low
        if (body->GetPosition().y > 400)  {
            bubbleColors.remove(body);
            world.DestroyBody(body);
        }
        body = nextBody;
    }

    int bubbleCount = world.GetBodyCount();

    //add at random times
    if (bubbleCount < 10000 && rand() % 100 < 50) {
        b2BodyDef bubbleDef;
        bubbleDef.type = b2_dynamicBody;

        //random size
        b2CircleShape bubbleShape;
        int radius = rand() % 5 + 3;
        bubbleShape.m_radius = radius;

        //at the top but random x
        float startHeight = height() - (width() * image.height() / image.width());
        bubbleDef.position.Set(static_cast<float>(50 + rand() % 200), startHeight - radius);
        b2Body* newBubble = world.CreateBody(&bubbleDef);

        bubbleColors.insert(newBubble, getColor());


        b2FixtureDef fixtureDef;
        fixtureDef.shape = &bubbleShape;

        //give them a random angle
        newBubble->SetLinearVelocity(b2Vec2(20 - rand() % 40, 50 - rand() % 60));
        newBubble->CreateFixture(&fixtureDef);
    }
    update();
}

void Cauldron::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);


    //scale image by width and place at bottom of widget
    int scalledHeight = width() * image.height() / image.width();
    painter.drawImage(QRect(0, height() - scalledHeight, width(), scalledHeight), image, image.rect());

    painter.setPen(Qt::NoPen);

    float startHeight = height() - (width() * image.height() / image.width());
    for (b2Body* body = world.GetBodyList(); body; body = body->GetNext()) {
        b2Vec2 position = body->GetPosition();
        painter.setBrush(QBrush(bubbleColors[body]));
        painter.setOpacity(1.0 - (position.y - startHeight) / 200);
        painter.drawEllipse(
            QRectF(
                position.x - 5,
                position.y - 5,
                body->GetFixtureList()->GetShape()->m_radius * 2,
                body->GetFixtureList()->GetShape()->m_radius * 2));
    }
}
