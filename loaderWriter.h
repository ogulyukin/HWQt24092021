#ifndef LOADERWRITER_H
#define LOADERWRITER_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QVariant>
#include <QList>
#include <QByteArray>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QtSql>
#include "person.h"

class loaderWriter
{
public:
    static bool readXml(QList<Person*>* persons, QString filename, QSqlDatabase* db);
    static int writeToSqlite(QString name, QString company, int age, QSqlDatabase* db);
    static bool readFromSqlite(QList<Person*>* persons, QSqlDatabase* db);
    static bool writeToJson(QList<Person*>* persons, QString path);
};

#endif // LOADERWRITER_H
