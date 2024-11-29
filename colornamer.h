#ifndef COLORNAMER_H
#define COLORNAMER_H

#include <QColor>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

class ColorNamer : public QObject {
    Q_OBJECT

public:
    explicit ColorNamer(QObject* parent = nullptr);

private slots:
    void handleNetworkReply(QNetworkReply* reply);

private:
    QNetworkAccessManager networkManager;

public slots:
    void requestColorName(const QColor& color);

signals:
    void colorNameReceived(const QString& colorName);
};

#endif // COLORNAMER_H
