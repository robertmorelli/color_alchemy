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
    timer.start(4);
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
        if (body->GetPosition().y > 100)  world.DestroyBody(body);
        body = nextBody;
    }

    int bubbleCount = 0;
    for (b2Body* body = world.GetBodyList(); body; body = body->GetNext()) ++bubbleCount;

    //add at random times
    if (bubbleCount < 100 && rand() % 100 < 5) {
        b2BodyDef bubbleDef;
        bubbleDef.type = b2_dynamicBody;

        //at the top but random x
        bubbleDef.position.Set(static_cast<float>(50 + rand() % 200), 0.0f);
        b2Body* newBubble = world.CreateBody(&bubbleDef);

        //random size
        b2CircleShape bubbleShape;
        bubbleShape.m_radius = rand() % 5 + 3;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &bubbleShape;

        //give them a random angle
        newBubble->SetLinearVelocity(b2Vec2(10 - rand() % 20, 10 - rand() % 20));
        newBubble->CreateFixture(&fixtureDef);
    }
    update();
}

void Cauldron::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    painter.drawImage(rect(), image, image.rect());

    painter.setBrush(getColor());
    painter.setPen(Qt::NoPen);

    for (b2Body* body = world.GetBodyList(); body; body = body->GetNext()) {
        b2Vec2 position = body->GetPosition();
        painter.setOpacity(1.0 - position.y / 100.0);
        painter.drawEllipse(
            QRectF(
                position.x - 5,
                position.y - 5,
                body->GetFixtureList()->GetShape()->m_radius * 2,
                body->GetFixtureList()->GetShape()->m_radius * 2));
    }
}
