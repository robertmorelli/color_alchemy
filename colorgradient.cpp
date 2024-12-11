#include "colorgradient.h"
#include "vivid/color.h"
#include "vivid/vivid.h"
#include <QColor>
#include <QPainter>
#include <QPainterPath>

colorGradient::colorGradient(QWidget* parent)
    : QWidget(parent)
    , targetColor(vivid::rgb_t(0.1, 0.7, 0.5))
    , sourceColor(vivid::rgb_t(0.6, 0.6, 0.2)) {
    this->setAttribute(Qt::WA_TranslucentBackground);
}

void colorGradient::setTargetColor(const QColor& color) {
    targetColor = vivid::Color(vivid::rgb_t(color.redF(), color.greenF(), color.blueF()));
    update();
}

void colorGradient::setSourceColor(const QColor& color) {
    sourceColor = vivid::Color(vivid::rgb_t(color.redF(), color.greenF(), color.blueF()));
    update();
}

/**
 * Color Space Geometry Overview
 * ----------------------------
 * Both functions define a plane in OKLAB color space through which we sample colors.
 * Each plane is defined by two vectors:
 * 1. The primary vector: Line connecting source and target colors
 * 2. The secondary vector: Determines the plane's orientation
 *
 * Input coordinates (x,y) are always in [0,1] range.
 *
 * colorFunctionVert():
 * --------------------
 * Defines a vertical plane in OKLAB space where:
 * - Primary vector = (targetColor - sourceColor)
 *   This forms the main gradient direction from source to target
 * - Secondary vector = (0, 0, 1) [vertical unit vector]
 *   This keeps the plane perfectly vertical
 * - Source color appears at (0.25, 0.25)
 * - Target color appears at (0.75, 0.75)
 * - x controls position along a/b components (chroma)
 * - y controls position along L component (luminance)
 *
 * colorFunctionLessLum():
 * -----------------------
 * Defines a plane in OKLAB space where:
 * - Primary vector = (targetColor - sourceColor)
 *   This forms the main gradient direction
 * - Secondary vector = (-ΔB, ΔA, 0)
 *   This vector is "perpendicular" to the primary vector in the a/b plane
 *   (no luminance/L component)
 * - The secondary vector is not actually perpendicular to the primary
 *   vector in OKLAB space but sheared for computational simplicity
 * - Source color appears at (0.5, 0.2)
 * - Target color appears at (0.5, 0.8)
 * - x controls position along the perpendicular vector
 * - y controls position along the primary vector
 *
 * Both functions:
 * --------------
 * - Take (x,y) coordinates in [0,1] range
 * - Transform these coordinates based on where source/target colors should appear
 * - Apply the interpolation in OKLAB space
 * - Convert back to RGB for display
 */

QColor colorGradient::colorFunctionVert(double x, double y) {
    vivid::col_t targ = targetColor.oklab().value();
    vivid::col_t sour = sourceColor.oklab().value();

    double xFactor = (x - 0.25) / (0.75 - 0.25);
    double yFactor = (y - 0.25) / (0.75 - 0.25);

    double l = sour[0] + yFactor * (targ[0] - sour[0]);
    double a = sour[1] + xFactor * (targ[1] - sour[1]);
    double b = sour[2] + xFactor * (targ[2] - sour[2]);

    vivid::Color color = vivid::Color(vivid::oklab_t(l, a, b));
    vivid::col_t rgb = color.rgb().value();
    return QColor::fromRgbF(rgb[0], rgb[1], rgb[2]);
}

QColor colorGradient::colorFunctionLessLum(double x, double y) {
    vivid::col_t targ = targetColor.oklab().value();
    vivid::col_t sour = sourceColor.oklab().value();

    double lDiff = sour[0] - targ[0];
    double aDiff = sour[1] - targ[1];
    double bDiff = sour[2] - targ[2];

    double perpA = bDiff;
    double perpB = -aDiff;

    double xFactor = (x - 0.5) * 2.0;
    double yFactor = (y - 0.8) / (0.2 - 0.8);

    double l = targ[0] + yFactor * lDiff;
    double a = targ[1] + yFactor * aDiff + xFactor * perpA;
    double b = targ[2] + yFactor * bDiff + xFactor * perpB;

    vivid::Color color = vivid::Color(vivid::oklab_t(l, a, b));
    vivid::col_t rgb = color.rgb().value();

    return QColor::fromRgbF(rgb[0], rgb[1], rgb[2]);
}

void colorGradient::paintEvent(QPaintEvent* /*event*/) {
    QPainter painter(this);
    int w = width();
    int h = height();
    bool vh = objectName().contains('V');

    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            double xNorm = static_cast<double>(x) / w;
            double yNorm = static_cast<double>(y) / h;
            painter.setPen(vh ? colorFunctionVert(xNorm, yNorm) : colorFunctionLessLum(xNorm, yNorm));
            painter.drawPoint(x, y);
        }
    }

    int sourceX = 0;
    int sourceY = 0;
    int targetX = 0;
    int targetY = 0;

    if (vh) {
        sourceX = w * 0.25;
        sourceY = h * 0.25;
        targetX = w * 0.75;
        targetY = h * 0.75;

    } else {
        sourceX = w * 0.5;
        sourceY = h * 0.2;
        targetX = w * 0.5;
        targetY = h * 0.8;
    }

    QFont font("Comic Sans MS", 15);
    painter.setFont(font);

    double sour = sourceColor.oklab().value()[0];
    QPen penSour(sour > 0.6 ? QColor("#544b42") : QColor("#d1bf9b"));
    penSour.setWidth(2);
    painter.setPen(penSour);
    painter.drawEllipse(QPoint(sourceX, sourceY), 5, 5);
    painter.drawText(sourceX - 25, sourceY + 25, "pot color");
    double targ = targetColor.oklab().value()[0];
    QPen penTarg(targ > 0.6 ? QColor("#544b42") : QColor("#d1bf9b"));
    penTarg.setWidth(2);
    painter.setPen(penTarg);
    painter.drawEllipse(QPoint(targetX, targetY), 5, 5);
    painter.drawText(targetX - 25, targetY + 25, "goal color");

    QRect rect = this->rect();
    int radius = 10;
    QPainterPath fullPath, roundedRectPath;
    fullPath.addRect(rect);
    roundedRectPath.addRoundedRect(rect, radius, radius);
    QPainterPath outsidePath = fullPath.subtracted(roundedRectPath);
    painter.fillPath(outsidePath, QColor("#d1bf9b"));
}
