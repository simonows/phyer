#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include <registerflag.h>
#include <qtableviewmodel.h>

#include <vector>
#include <QMainWindow>
#include <QComboBox>

#include <ifaddrs.h>
#include <net/if.h>

class Hardware
{
    struct ifreq ifr;
    struct ifaddrs *addrs,*tmp;

    QString selected_item;
    QStringList items;

public:
    Hardware();
    ~Hardware();

    QStringList getItems(void);
    void setItem(QString _item);
    QList<RegisterFlag> getRegisterSet(void);
    int setRegisterValue(unsigned long addr, unsigned short value);
};
#endif // __HARDWARE_H__

