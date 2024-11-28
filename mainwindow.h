#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Game.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

///
/// \brief The LabelInfo struct stores the text to be displayed and position to introduce the
/// player to the game.
///
struct LabelInfo {
    QString text;
    QPoint position;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void flasks();

public slots:
    void updateGoal(QColor color);
    void updateInstructions(QImage image);
    void newHint(QString);
    ///
    /// \brief walkThroughLabels a function to walk the user through the setup of the game, called on setup timer.
    ///
    void walkThroughLabels();
private:

    Ui::MainWindow* ui;
    Game game;
    QTimer *timer; // timer for label display
    int labelsIdx; // index for label display
    QVector<LabelInfo> labels; // stores label info
};
#endif // MAINWINDOW_H
