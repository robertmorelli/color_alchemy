#ifndef COLORFLASK_H
#define COLORFLASK_H

#include <QWidget>
#include <QColor>
#include <QPoint>

class ColorFlask : public QWidget
{
private:
    QColor color;
    QPoint* position;
protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit ColorFlask(QWidget *parent = nullptr, QColor color = nullptr, QPoint* position = nullptr);

signals:
};

#endif // COLORFLASK_H
