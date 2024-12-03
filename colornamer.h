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
    /// @brief Handle the network reply from the OpenAI API
    /// @param reply The network reply
    void handleNetworkReply(QNetworkReply* reply);

private:
    QNetworkAccessManager networkManager;

public slots:
    /// @brief Request the name of a color from the OpenAI API
    /// @param color The color to name
    void requestColorName(const QColor& color);

signals:
    /// @brief Emitted when the color name is received from the OpenAI API
    /// @param colorName The name of the color
    void colorNameReceived(const QString& colorName);
};

#endif // COLORNAMER_H
