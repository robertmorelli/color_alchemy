#ifndef COLORGRADIENT_H
#define COLORGRADIENT_H

#include "vivid/color.h"
#include <QColor>
#include <QWidget>

class colorGradient : public QWidget {
    Q_OBJECT
private:
    vivid::Color targetColor;
    vivid::Color sourceColor;

public:
    explicit colorGradient(QWidget* parent = nullptr);
    QColor colorFunctionVert(double x, double y);
    QColor colorFunctionLessLum(double x, double y);
    void setTargetColor(const QColor&);
    void setSourceColor(const QColor&);

protected:
    void paintEvent(QPaintEvent* event) override;
    QSize sizeHint() const override { return QSize(400, 400); }
};

#endif // COLORGRADIENT_H
