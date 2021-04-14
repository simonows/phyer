
#include <registerflag.h>

RegisterFlag::RegisterFlag()
{

}

RegisterFlag::RegisterFlag(size_t id, QString name, QString value, QString evalue)
{
    this->id = id;
    this->name = name;
    this->value = value;
    this->evalue = evalue;
}

QString RegisterFlag::getValue() const
{
    return value;
}

void RegisterFlag::setValue(const QString &value)
{
    this->value = value;
}

QString RegisterFlag::getEvalue() const
{
    return evalue;
}

void RegisterFlag::setEvalue(const QString &value)
{
    this->evalue = evalue;
}

QString RegisterFlag::getName() const
{
    return name;
}

void RegisterFlag::setName(const QString &value)
{
    name = value;
}

int RegisterFlag::getId() const
{
    return id;
}

void RegisterFlag::setId(int value)
{
    id = value;
}

