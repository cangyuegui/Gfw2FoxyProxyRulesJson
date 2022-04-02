#ifndef PARSE_H
#define PARSE_H

#include <QtCore>

bool GetGfwCode(const QString& filePath, QString& res);

QStringList GetUrlList(const QString& inList);
QJsonDocument ToRule(const QStringList& inList);
bool SaveFileToJson(const QJsonDocument& doc, QString& filePath);
bool Convert(const QString& gfwPath, QString& errorCode);

#endif // PARSE_H
