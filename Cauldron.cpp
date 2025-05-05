#include "Cauldron.h"
#include <Box2D/Box2D.h>
#include <QColor>
#include <QImage>
#include <QPaintEvent>
#include <QPainter>
#include <QRandomGenerator>
#include <QTimer>
#include <qrandom.h>
#include <vivid/color.h>
#include <vivid/types.h>

Cauldron::Cauldron(QWidget* parent)
    : QWidget { parent }
    , image(":/images/pot.png")
    , totalColors { 0 }
    , world(b2Vec2(0.0f, 10.0f))
    , completeJustHappened(false)
    , timer(this) {
    // connect(&timer, &QTimer::timeout, this, &Cauldron::simulatePhysics);
    // timer.start(100);
// }
    startTimer(1, Qt::PreciseTimer);
}

void Cauldron::timerEvent(QTimerEvent* e) {
    simulatePhysics();
}

void Cauldron::addColor(QColor newColor) {
    if (colors.contains(newColor.name()))
        colors[newColor.name()]++;
    else
        colors.insert(newColor.name(), 1);

    totalColors++;

    emit newMixedColor(getColor());
    emit colorsChanged(colors, totalColors);
}

QColor Cauldron::getColor() {
    // Early exit if no colors
    if (totalColors == 0) return Qt::white;
    if (colors.size() == 1) return QColor(colors.begin().key());

    double comL = 0.0, comA = 0.0, comB = 0.0, comS = 0.0;

    foreach (const QString& key, colors.keys()) {
        double weight = static_cast<double>(colors[key]) / static_cast<double>(totalColors);
        QColor color(key);

        float r = static_cast<float>(color.red()) / 255.0f;
        float g = static_cast<float>(color.green()) / 255.0f;
        float b = static_cast<float>(color.blue()) / 255.0f;

        vivid::Color vividColor(vivid::rgb_t { r, g, b });
        const vivid::col_t oklabValue = vividColor.oklab().value();
        const vivid::col_t hsvValue = vividColor.hsv().value();

        comL += weight * oklabValue[0];
        comA += weight * oklabValue[1];
        comB += weight * oklabValue[2];
        comS += weight * hsvValue[1];
    }

    // Average OKLab components
    vivid::Color oklabInterpolation = vivid::Color({ comL, comA, comB }, vivid::Color::Space::Oklab);
    vivid::col_t oklabValue = oklabInterpolation.hsv().value();
    double blendAverageChroma = 0.2;
    oklabValue[1] = comS * blendAverageChroma + oklabValue[1] * (1 - blendAverageChroma);
    vivid::col8_t oklabColor = vivid::Color(oklabValue, vivid::Color::Space::Hsv).rgb8();
    return QColor::fromRgb(oklabColor[0], oklabColor[1], oklabColor[2]);
}

void Cauldron::dump() {
    colors.clear();
    totalColors = 0;

    emit newMixedColor(getColor());
    emit colorsChanged(colors, totalColors);
}

void Cauldron::complete() {
    completeJustHappened = true;
    QTimer::singleShot(300, this, [this]() { completeJustHappened = false; });
}

void Cauldron::simulatePhysics() {
    world.Step(1.0f / 60.0f, 1, 1);

    for (b2Body* body = world.GetBodyList(); body;) {
        b2Body* nextBody = body->GetNext();

        // remove bubbles that are too low
        if (body->GetPosition().y > 400) {
            bubbleColors.remove(body);
            world.DestroyBody(body);
        }
        body = nextBody;
    }

    // add a bubble every so often (30% chance)
    if (QRandomGenerator::global()->generate() % 100 < 30 || completeJustHappened) {
        b2BodyDef bubbleDef;
        bubbleDef.type = b2_dynamicBody;

        b2CircleShape bubbleShape;
        // random radius between 1 and 7
        int radius = QRandomGenerator::global()->generate() % 7 + (completeJustHappened ? 5 : 1);
        bubbleShape.m_radius = radius;

        float startHeight = height() - (width() * image.height() / image.width());
        // top of pot with random x between 35 and 265
        bubbleDef.position.Set(static_cast<float>(35 + QRandomGenerator::global()->generate() % 230), startHeight - radius);
        b2Body* newBubble = world.CreateBody(&bubbleDef);

        bubbleColors.insert(newBubble, getColor());

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &bubbleShape;

        // random velocity between -20 and 20 for x and -10 and 30 for y
        newBubble->SetLinearVelocity(b2Vec2((20 - rand() % 40) * (completeJustHappened ? 2 : 1), 30 - rand() % 40 + (completeJustHappened ? 5 : 0)));
        newBubble->CreateFixture(&fixtureDef);
    }
    update();
}

void Cauldron::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);

    // scale image by width and place at bottom of widget
    int scaledHeight = width() * image.height() / image.width();
    painter.drawImage(QRect(0, height() - scaledHeight, width(), scaledHeight), image, image.rect());

    painter.setPen(Qt::NoPen);

    // start height is the top of the image of the cauldron
    float startHeight = height() - (width() * image.height() / image.width());
    for (b2Body* body = world.GetBodyList(); body; body = body->GetNext()) {
        b2Vec2 position = body->GetPosition();
        painter.setBrush(QBrush(bubbleColors[body]));
        double radius = body->GetFixtureList()->GetShape()->m_radius;

        // make the bubbles more transparent if they are close to the edge
        // the fade off uses this formula: min(dangerZone - distance - radius, startfade) / startfade
        double opacityTooCloseToBottom = std::min(startHeight + 200.0f - position.y, 200.0f) / 200.0f;
        double opacityTooCloseToLeft = std::min(0.0f + (position.x - radius), (double)50.0f) / 50.0f;
        double opacityTooCloseToRight = std::min(0.0f + (width() - position.x - radius), (double)50.0f) / 50.0f;
        double opacityTooCloseToTop = std::min(0.0f + (position.y - radius), (double)20.0f) / 20.0f;

        // the opacity is the minimum of the four
        double upDownOpacity = std::min(opacityTooCloseToBottom, opacityTooCloseToTop);
        double leftRightOpacity = std::min(opacityTooCloseToLeft, opacityTooCloseToRight);
        double opacity = std::min(upDownOpacity, leftRightOpacity);

        painter.setOpacity(opacity);
        painter.drawEllipse(QRectF(position.x - 5, position.y - 5, radius * 2, radius * 2));
    }
}
