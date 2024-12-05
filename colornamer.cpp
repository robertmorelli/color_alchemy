// colornamer.cpp
#include "colornamer.h"
#include <QJsonArray>
#include <QNetworkRequest>
#include <QTimer>


ColorNamer::ColorNamer(QObject* parent)
    : QObject(parent)
    , networkManager(this) { }

// https://platform.openai.com/docs/api-reference/introduction
void ColorNamer::requestColorName(const QColor& color) {
    QTimer::singleShot(0,this, [this, color]() {
        if (color.name() == QColor("#FFFFFF").name()) {
            emit colorNameReceived("titanium white");
            return;
        } else if (color.name() == QColor("#FDDA0D").name()) {
            emit colorNameReceived("cadmium yellow");
            return;
        } else if (color.name() == QColor("#E68125").name()) {
            emit colorNameReceived("cadmium deep yellow");
            return;
        } else if (color.name() == QColor("#D22B2B").name()) {
            emit colorNameReceived("cadmium red");
            return;
        } else if (color.name() == QColor("#E32636").name()) {
            emit colorNameReceived("alizarin crimson");
            return;
        } else if (color.name() == QColor("#2a52be").name()) {
            emit colorNameReceived("cerulean blue");
            return;
        } else if (color.name() == QColor("#0047AB").name()) {
            emit colorNameReceived("cobalt blue");
            return;
        } else if (color.name() == QColor("#0C0B0A").name()) {
            emit colorNameReceived("ivory black");
            return;
        }
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
        jsonObj["model"] = "chatgpt-4o-latest";
        jsonObj["messages"] = messagesArray;
        jsonObj["max_tokens"] = 10; // extremely short
        jsonObj["temperature"] = 0.0;

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
