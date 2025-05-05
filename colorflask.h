#ifndef COLORFLASK_H
#define COLORFLASK_H

#include <Box2D/Box2D.h>
#include <QColor>
#include <QLabel>
#include <QPoint>
#include <QTimer>
#include <QWidget>
#include <QTimerEvent>
#include <QElapsedTimer>

///
/// \brief The ColorFlask class holds the colors for each flask and the necessary information for their interactions with the game.
///
class ColorFlask : public QWidget {
    Q_OBJECT
private:
    QElapsedTimer intervalWatch;
    QColor color;               /// color of the flask
    QPoint initialPosition;     /// location of the flask
    float lastY;                /// previous y position of flask for physics calculations
    float lastX;                /// previous y position of flask for physics calculations
    QPoint dragOffset;          /// offset relative to the pointer to keep it under the cursor
    b2World world;              /// box 2d world body
    b2Body* groundBody;         /// box 2d body for the floor
    b2Body* leftShelfBody;      /// box 2d body of the left shelf
    b2Body* rightShelfBody;     /// box 2d body of the right shelf
    b2Body* body;               /// box 2d body for the flask
    bool isDragging;            /// true if the flask is being dragged, false otherwise
    QTimer timer;               /// triggers gamesteps for physics rendering
    QTimer setupTimer;          /// triggers the setUpFlask function after the game loads in

    ///
    /// \brief setUpFlask function to render the flask after the ui is rendered
    ///
    void setUpFlask();

protected:
    ///
    /// \brief mousePressEvent for when the user clicks on a flask. engages dragging with box2d logic.
    /// \param event clicked position
    ///
    void mousePressEvent(QMouseEvent* event) override;

    ///
    /// \brief mouseMoveEvent if dragging, moves the flask position to where the mouse is.
    /// \param event clicked position
    ///
    void mouseMoveEvent(QMouseEvent* event) override;

    ///
    /// \brief mouseReleaseEvent when dropped, uses box2d logic to drop the flask.
    /// \param event clicked position
    ///
    void mouseReleaseEvent(QMouseEvent* event) override;

    ///
    /// \brief paintEvent draws the image of the flask
    /// \param event unused
    ///
    void paintEvent(QPaintEvent* event) override;
    void timerEvent(QTimerEvent* e) override;

public:
    explicit ColorFlask(QWidget* parent = nullptr);

    /// \brief change the color of the flask
    /// \param color The new color
    void changeColor(QColor color);

private slots:
    /// \brief simulate the physics of the flask and the shelf
    void simulatePhysics();

signals:
    /// \brief signal to add the color of the flask to the cauldron
    /// \param color The color of the flask
    void addToCauldron(QColor color);

    /// \brief signal to make a sound when the flask hits the shelf
    void makeSound();
};

#endif // COLORFLASK_H
