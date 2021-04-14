#ifndef COUNTRYFLAG_H
#define COUNTRYFLAG_H

#include <QString>



class CountryFlag
{
public:
    CountryFlag();
    CountryFlag(int id, QString name, QString icon);
    QString getIcon() const;
    void setIcon(const QString &value);

    QString getName() const;
    void setName(const QString &value);

    int getId() const;
    void setId(int value);

private:
    int id;
    QString icon;
    QString name;
};

#endif // COUNTRYFLAG_H

