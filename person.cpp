#include "person.h"

Person::Person(int id, QString name) : id(id), name(name)
{

}

Person::Person(int id, QString name, QString company, int age)  : id(id), name(name), company(company), age(age)
{

}
