#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include <vector>
#include <QMainWindow>
#include <QComboBox>

#ifndef _WIN32
#include <ifaddrs.h>
#include <net/if.h>
#endif

#include <phyer/registerflag.h>


class Hardware
{
#ifndef _WIN32
    struct ifreq ifr;
    struct ifaddrs *addrs,*tmp;
#endif

    QString selected_item;
    QStringList items;

public:
    Hardware();
    ~Hardware();

    static size_t getStdRegCount();
    QStringList getItems(void);
    void setItem(QString _item);
    QList<RegisterFlag> getRegisterSet(void);
    int setRegisterValue(unsigned long addr, unsigned short value);
    unsigned short getRegisterValue(unsigned long _addr);

    static unsigned long const MAX_ADDR = 31;
};
#endif // __HARDWARE_H__

