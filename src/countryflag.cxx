
#include <countryflag.h>

CountryFlag::CountryFlag()
{

}

CountryFlag::CountryFlag(int id, QString name, QString icon)
{
    this->id = id;
    this->name = name;
    this->icon = icon;
}

QString CountryFlag::getIcon() const
{
    return icon;
}

void CountryFlag::setIcon(const QString &value)
{
    icon = value;
}

QString CountryFlag::getName() const
{
    return name;
}

void CountryFlag::setName(const QString &value)
{
    name = value;
}

int CountryFlag::getId() const
{
    return id;
}

void CountryFlag::setId(int value)
{
    id = value;
}
