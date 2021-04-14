#ifndef COUNTRYFLAG_H
#define COUNTRYFLAG_H

#include <QString>

class RegisterFlag
{
public:
    RegisterFlag();
    RegisterFlag(size_t id, QString name, QString value, QString evalue);
    QString getValue() const;
    void setValue(const QString &value);

    QString getEvalue() const;
    void setEvalue(const QString &value);

    QString getName() const;
    void setName(const QString &value);

    int getId() const;
    void setId(int value);

private:
    int id;
    QString value;
    QString evalue;
    QString name;
};

#endif // COUNTRYFLAG_H

