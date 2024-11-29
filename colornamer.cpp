// colornamer.cpp
#include "colornamer.h"
#include <QJsonArray>
#include <QNetworkRequest>
#include <QTimer>

const QString systemParameter = "an agent that converts rgb "
                                "hex codes into very creative color names that are "
                                "also accurate. I will respond with a two word name "
                                "for the color and only those two words."
                                "I will avoid using generic names like 'red' or 'blue'."
                                "or even well known names like the css color names."
                                // i did some testing and i think we need this instruction as well as the next one
                                "you will not use racial terms like 'indian red' or 'french blue'."
                                "you will not use suggestive terms like 'sexy pink' or 'hot black'."
                                "if possible you will use terms related to alchemy or magical objects."
                                "but not words relating to the subject of magic itself like 'mystic' or 'magic'."
                                "if possible you will use terms related to the natural world."
                                "if possible you will use rare gemstone or mineral names.";
const QUrl apiUrl = QUrl("https://api.openai.com/v1/chat/completions");
// try not to spend all my money lol
const QString KEY = "sk-proj-lkdxpXyJ4EEi4"
                    "xK1_pHoaIjce1QYmeEJUJNw5x_uE"
                    "86OT4AvEEk6EPLh4DzD3m37eM6oz"
                    "vqt6xT3BlbkFJAGp5CIYjni3UZkU"
                    "ZXNZvO01EwynVBpIjKIPU4lR0BZc"
                    "P1V-YnkWuYmrhmYEh08zW7S8B9bwysA";
const QByteArray accessToken = QString("Bearer " + KEY).toUtf8();

ColorNamer::ColorNamer(QObject* parent)
    : QObject(parent)
    , networkManager(this) { }

// https://platform.openai.com/docs/api-reference/introduction
void ColorNamer::requestColorName(const QColor& color) {
    QTimer::singleShot(0, [this, color]() {
        QNetworkRequest request(apiUrl);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader("Authorization", accessToken);

        QJsonArray messagesArray;
        QJsonObject systemObj;
        QJsonObject messageObj;
        systemObj["role"] = "system";
        systemObj["content"] = systemParameter; // tells the ai what its doing
        messageObj["role"] = "user";
        messageObj["content"] = color.name(); // the color to name
        messagesArray.append(systemObj);
        messagesArray.append(messageObj);

        QJsonObject jsonObj;
        jsonObj["model"] = "gpt-3.5-turbo"; // cheap model
        jsonObj["messages"] = messagesArray;
        jsonObj["max_tokens"] = 10; // extremely short
        jsonObj["temperature"] = 1.1; // a little creative but not wild

        QNetworkReply* reply = networkManager.post(request, QJsonDocument(jsonObj).toJson());
        connect(reply, &QNetworkReply::finished, this, [this, reply]() { handleNetworkReply(reply); });
    });
}

void ColorNamer::handleNetworkReply(QNetworkReply* reply) {
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) return;
    // simply assume the first message exists and return it lul
    emit colorNameReceived(
        (((QJsonDocument::fromJson(reply->readAll()).object()["choices"].toArray())[0].toObject())["message"].toObject())["content"].toString().trimmed());
}
