#ifndef COUNTRYFLAG_H
#define COUNTRYFLAG_H

#include <QString>

class RegisterFlag
{
public:
    RegisterFlag();
    RegisterFlag(unsigned long id, QString name, QString value, QString evalue, QString desc);
    QString getValue() const;
    void setValue(const QString &value);

    QString getEvalue() const;
    void setEvalue(const QString &value);

    QString getName() const;
    void setName(const QString &value);

    unsigned long getId() const;
    void setId(unsigned long value);

    QString getDesc() const;
    void setDesc(const QString &sti);
private:
    unsigned long id;
    QString value;
    QString evalue;
    QString name;
    QString desc;
};

#endif // COUNTRYFLAG_H

