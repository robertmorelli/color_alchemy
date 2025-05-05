#ifndef CAULDRON_H
#define CAULDRON_H

#include <Box2D/Box2D.h>
#include <QPieSeries>
#include <QString>
#include <QTimer>
#include <QWidget>
#include <QTimerEvent>

///
/// \brief The Cauldron class represents the cauldron and the bubbles,
/// encompassing the colors currently in the cauldron as well as emptying the cauldron
///
class Cauldron : public QWidget {
    Q_OBJECT
public:
    explicit Cauldron(QWidget* parent = nullptr);

    ///
    /// \brief getColor gets the current mixed color in the cauldron
    /// \return a QColor that represents the sum of all the colors in the cauldron being mixed together
    ///
    QColor getColor();

    ///
    /// \brief dump empties the cauldron of colors and resets it to white
    ///
    void dump();

private:
    QMap<QString, int> colors;          /// colors in the cauldron where string is color name and int is how many of that color has been added
    QImage image;                       /// drawing of the cauldron
    int totalColors;                    /// total number of flasks that have been added to the cauldron
    b2World world;                      /// box 2d world body
    bool completeJustHappened;          /// to make more bubbles when a color is completed
    QTimer timer;                       /// triggers gamesteps for physics renderings
    QMap<b2Body*, QColor> bubbleColors; /// maps the box 2d bubble to the color of the bubble

    /// \brief simulatePhysics simulates the physics of the bubbles
    ///
    void simulatePhysics();

public slots:
    /// \brief add a color to the cauldron
    /// \param newColor The color to add
    void addColor(QColor newColor);

    /// \brief Complete level makes effects
    void complete();

protected:
    /// \brief override the paint event to draw the cauldron with bubbles
    /// \param event The paint event (unused)
    void paintEvent(QPaintEvent* event) override;
    void timerEvent(QTimerEvent* e) override;

signals:
    /// \brief Emitted when a new color is mixed
    /// \param color The new mixed color
    void newMixedColor(QColor color);

    /// \brief Emitted when the colors in the cauldron change
    /// \param colors The new colors in the cauldron
    /// \param totalColors The total number of colors in the cauldron
    void colorsChanged(const QMap<QString, int>& colors, int totalColors);
};

#endif // CAULDRON_H
