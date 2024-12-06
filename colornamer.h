#ifndef COLORNAMER_H
#define COLORNAMER_H

#include <QNetworkReply>
#include <QObject>

///
/// \brief The ColorNamer class generates names for any hex code
///
class ColorNamer : public QObject {
    Q_OBJECT

public:
    explicit ColorNamer(QObject* parent = nullptr);

private slots:
    ///
    /// \brief handleNetworkReply Handle the network reply from the OpenAI API
    /// \param reply The network reply
    ///
    void handleNetworkReply(QNetworkReply* reply);

private:
    QNetworkAccessManager networkManager;                                               /// the network manager
    const QString systemParameter = "two word magic color name. dont use synonyms "     /// the parameter for querying the LLM
                                    "for magic like mystical or enchanted or magic";
    const QUrl apiUrl = QUrl("https://api.openai.com/v1/chat/completions");             /// the api url
    const QString KEY = "sk-proj-lkdxpXyJ4EEi4"                                         /// the key for the api
                        "xK1_pHoaIjce1QYmeEJUJNw5x_uE"
                        "86OT4AvEEk6EPLh4DzD3m37eM6oz"
                        "vqt6xT3BlbkFJAGp5CIYjni3UZkU"
                        "ZXNZvO01EwynVBpIjKIPU4lR0BZc"
                        "P1V-YnkWuYmrhmYEh08zW7S8B9bwysA";
    const QByteArray accessToken = QString("Bearer " + KEY).toUtf8();                   /// the access token for the api


public slots:
    ///
    /// \brief requestColorName Request the name of a color from the OpenAI API
    /// \param color The color to name
    ///
    void requestColorName(const QColor& color);

signals:
    ///
    /// \brief colorNameReceived Emitted when the color name is received from the OpenAI API
    /// \param colorName The name of the color
    ///
    void colorNameReceived(const QString& colorName);
};

#endif // COLORNAMER_H
