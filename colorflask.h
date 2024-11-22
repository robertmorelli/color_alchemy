#ifndef COLORFLASK_H
#define COLORFLASK_H

#include <QWidget>
#include <QColor>

class ColorFlask : public QWidget
{
private:
    QColor color;

public:
    explicit ColorFlask(QWidget *parent = nullptr, QColor color = nullptr);

signals:
};

#endif // COLORFLASK_H
