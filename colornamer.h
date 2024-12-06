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
    const QString systemParameter = "two word magic color name. dont use synonyms for magic like mystical or enchanted or magic";
    const QUrl apiUrl = QUrl("https://api.openai.com/v1/chat/completions");
    const QString KEY = "sk-proj-lkdxpXyJ4EEi4"
                        "xK1_pHoaIjce1QYmeEJUJNw5x_uE"
                        "86OT4AvEEk6EPLh4DzD3m37eM6oz"
                        "vqt6xT3BlbkFJAGp5CIYjni3UZkU"
                        "ZXNZvO01EwynVBpIjKIPU4lR0BZc"
                        "P1V-YnkWuYmrhmYEh08zW7S8B9bwysA";
    const QByteArray accessToken = QString("Bearer " + KEY).toUtf8();


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
