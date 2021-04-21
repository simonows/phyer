#include <phyer/registerflag.h>


RegisterFlag::RegisterFlag()
{

}

RegisterFlag::RegisterFlag(
    unsigned long id
  , QString name
  , QString value
  , QString evalue
  , QString desc
){
    this->id = id;
    this->name = name;
    this->value = value;
    this->evalue = evalue;
    this->desc = desc;
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

unsigned long RegisterFlag::getId() const
{
    return id;
}

void RegisterFlag::setId(unsigned long value)
{
    id = value;
}

QString RegisterFlag::getDesc() const
{
    return desc;
}

void RegisterFlag::setDesc(const QString &sti)
{
    desc = sti;
}

