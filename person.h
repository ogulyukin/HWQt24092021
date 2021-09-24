#ifndef PERSON_H
#define PERSON_H
#include <QString>

class Person
{
public:
    Person(int id, QString name);
    Person(int id, QString name, QString company, int age);
    int id;
    QString name;
    QString company;
    int age;
};

#endif // PERSON_H
