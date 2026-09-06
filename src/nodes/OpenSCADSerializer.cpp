#include "OpenSCADSerializer.hpp"
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>

QJsonObject oscd_loadJson(QString fileName) {
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    QJsonDocument document = QJsonDocument().fromJson(jsonFile.readAll());
    QJsonObject object = document.object();
    return object;
}

void oscd_saveJson(QJsonObject object, QString fileName) {
    QJsonDocument document;
    document.setObject(object);
    QFile jsonFile(fileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
}

