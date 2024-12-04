#ifndef CAULDRON_H
#define CAULDRON_H
#include <Box2D/Box2D.h>
#include <QPieSeries>
#include <QString>
#include <QTimer>
#include <QWidget>

class Cauldron : public QWidget {
    Q_OBJECT
public:
    explicit Cauldron(QWidget* parent = nullptr);

    QColor getColor();
    void dump();

private:
    QMap<QString, int> colors;
    QImage image;
    int totalColors;
    b2World world;
    QTimer timer;
    void simulatePhysics();
    QMap<b2Body*, QColor> bubbleColors;
    bool completeJustHappened = false;

public slots:
    /// @brief Add a color to the cauldron
    /// @param newColor The color to add
    void addColor(QColor newColor);

    /// @brief Complete level makes effects
    void complete();

protected:
    /// @brief override the paint event to draw the cauldron with bubbles
    /// @param event The paint event (unused)
    void paintEvent(QPaintEvent* event) override;

signals:
    /// @brief Emitted when a new color is mixed
    /// @param color The new mixed color
    void newMixedColor(QColor color);
    /// @brief Emitted when the colors in the cauldron change
    /// @param colors The new colors in the cauldron
    /// @param totalColors The total number of colors in the cauldron
    void colorsChanged(const QMap<QString, int>& colors, int totalColors);
};

#endif // CAULDRON_H
