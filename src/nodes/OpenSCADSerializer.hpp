#include <QtCore/QString>
#include <QtCore/QJsonObject>

QJsonObject oscd_loadJson(QString fileName);
void oscd_saveJson(QJsonObject object, QString fileName);
