#include "loaderWriter.h"

bool loaderWriter::readXml(QList<Person*>* persons, QString filename, QSqlDatabase* db)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "ERROR can't open xml file!!!";
        return false;
    }
    QByteArray data = file.readAll();
    file.close();
    QXmlStreamReader stream(data);
    QString name = "";
    QString company = "";
    int age = 0;
    while(!stream.atEnd())
    {
        QXmlStreamReader::TokenType token = stream.readNext();
        if(token == QXmlStreamReader::StartElement)
        {
            if(stream.name() == "user")
            {
                name = (stream.attributes().at(0)).value().toString();
                qDebug() << "Name: " << name;
            }
            if(stream.name() == "company")
            {
                company = stream.readElementText();
                qDebug() << "Company: " << company;
            }
            if(stream.name() == "age")
            {
                age = stream.readElementText().toInt();
                qDebug() << "Age: " << age;
                int id = writeToSqlite(name, company, age, db);
                if(id != 0)
                {
                    qDebug() << "insrting: " + QString::number(id);
                    Person* person = new Person(id, name, company, age);
                    persons->append(person);
                }
            }
        }
    }
    return true;
}

int loaderWriter::writeToSqlite(QString name, QString company, int age, QSqlDatabase* db)
{
    if(db->open())
    {
        QSqlQuery query;
        QString command = "INSERT INTO 'Person_Tab' ('name', 'company', 'age') VALUES ('"
                      + name + "', '" + company + "', '" + QString::number(age) + "')";
        if(query.exec(command))
        {
            int result = query.lastInsertId().toInt();
            db->close();
            return result;
        }
        qDebug() << "Error: " << query.lastError().text();

        db->close();
    }
    return 0;
}

bool loaderWriter::readFromSqlite(QList<Person *>* persons, QSqlDatabase *db)
{
    if(db->open())
    {
        QSqlQuery query;
        if(!query.exec("select * from 'Person_Tab'"))
        {
            qInfo() << "Error: " << query.lastError().text();
            return false;
        }else
        {
            while(query.next())
            {
                Person* person = new Person(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(), query.value(3).toInt());
                persons->append(person);
            }
        }
        db->close();
    }
    return true;
}

bool loaderWriter::writeToJson(QList<Person *> *persons, QString path)
{
    path = path + QDir::separator() + "result.json";
    QVariantMap finallyMap;
    QVariantMap tempMap;
    for(int i = 0; i < persons->count(); i++)
    {
        tempMap.insert("id", persons->at(i)->id);
        tempMap.insert("company", persons->at(i)->company);
        tempMap.insert("age", persons->at(i)->age);
        finallyMap[persons->at(i)->name] = QVariant(tempMap);
    }
    QJsonDocument document = QJsonDocument::fromVariant(finallyMap);
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Не открывается файл конфигурации для записи!";
        return false;
    }
    qDebug() << "Begin serialization";
    file.write(document.toJson());
    qDebug() << file.fileName();
    file.close();
    return true;
}

