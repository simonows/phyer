#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifndef _WIN32
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/mii.h>
#include <linux/sockios.h>
#include <ifaddrs.h>
#endif

#include <vector>
#include <string>

#include <phyer/hardware.h>
#include <phyer/qtableviewmodel.h>

Hardware::Hardware()
{
#ifndef _WIN32
    getifaddrs(&addrs);
    tmp = addrs;

    while (tmp)
    {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET)
        {
            items << tmp->ifa_name;
        }
        tmp = tmp->ifa_next;
    }
#endif
}

Hardware::~Hardware()
{
#ifndef _WIN32
    freeifaddrs(addrs);
#endif
}

QStringList Hardware::getItems(void)
{
    return items;
}

void Hardware::setItem(QString _item)
{
    selected_item = _item;
}


typedef struct StdReg
{
    const unsigned long addr;
    const char *name;
    const char *status;
    const char *desc;
} StdReg;


static struct StdReg const stdRegNames[] = {
    { 0, "Control", "rw", "desc1" }
  , { 1, "Status", "ro", "desc2" }
  , { 2, "PHY Identifier", "ro", "desc3" }
  , { 3, "PHY Identifier", "ro", "desc4" }
  , { 4, "Auto-Negotiation Advertisement", "rw", "desc5" }
  , { 5, "Auto-Negotiation Link Partner Base Page Ability", "ro", "desc6" }
  , { 6, "Auto-Negotiation Expansion", "rw", "desc7" }
  , { 7, "Auto-Negotiation Next Page Transmit", "rw", "desc8" }
  , { 8, "Auto-Negotiation Link Partner Received Next Page", "ro", "desc9" }
  , { 9, "1000BASE-T Control Register", "rw", "desc10" }
  , { 10, "1000BASE-T Status Register", "ro", "desc11" }
  , { 11, "PSE Control register", "rw", "desc12" }
  , { 12, "PSE/PD Status register", "ro", "desc13" }
  , { 13, "Reserved (MMD control)", "rw", "desc14" }
  , { 14, "Reserved (MMD data)", "ro", "desc15" }
  , { 15, "Extended Status", "ro", "desc16" }
};


QList<RegisterFlag> Hardware::getRegisterSet(void)
{
    QList<RegisterFlag> set;
#ifndef _WIN32
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, selected_item.toLocal8Bit().data());

    struct mii_ioctl_data* mii = (struct mii_ioctl_data*)(&ifr.ifr_data);
    const int fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (fd == -1)
    {
        goto exit;
    }

    for (size_t i = 0; i < sizeof(stdRegNames) / sizeof(struct StdReg); i++)
    {
        mii->reg_num = stdRegNames[i].addr;
        mii->val_in  = 0;
        mii->val_out = 0;

        const int fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (fd >= 0)
        {
            if (ioctl(fd, SIOCGMIIREG, &ifr) >= 0)
            {
                set.append(RegisterFlag(
                    stdRegNames[i].addr
                  , stdRegNames[i].name
                  , QString::asprintf("0x%04hX", mii->val_out)
                  , stdRegNames[i].status
                  , stdRegNames[i].desc
                ));
            }
        }
    }
    close(fd);
#endif

exit:
    return set;
}


int Hardware::setRegisterValue(unsigned long _addr, unsigned short _value)
{
    int rc = 0;

#ifndef _WIN32
    memset(&ifr, 0, sizeof(ifr));
    struct mii_ioctl_data* mii = (struct mii_ioctl_data*)(&ifr.ifr_data);
    strcpy(ifr.ifr_name, selected_item.toLocal8Bit().data());
    mii->reg_num = _addr;
    mii->val_in  = _value;
    mii->val_out = 0;

    const int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        rc = 1;
        goto exit;
    }

    if (ioctl(fd, SIOCSMIIREG, &ifr) < 0)
    {
        rc = 2;
        close(fd);
        goto exit;
    }
    close(fd);
#endif

exit:
    return rc;
}


unsigned short Hardware::getRegisterValue(unsigned long _addr)
{
    unsigned short rc = 0;

#ifndef _WIN32
    memset(&ifr, 0, sizeof(ifr));
    struct mii_ioctl_data* mii = (struct mii_ioctl_data*)(&ifr.ifr_data);
    strcpy(ifr.ifr_name, selected_item.toLocal8Bit().data());
    mii->reg_num = _addr;
    mii->val_in  = 0;
    mii->val_out = 0;

    const int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        goto exit;
    }

    if (ioctl(fd, SIOCGMIIREG, &ifr) < 0)
    {
        close(fd);
        goto exit;
    }
    rc = mii->val_out;
    close(fd);
#endif

exit:
    return rc;
}

size_t Hardware::getStdRegCount()
{
    return sizeof(stdRegNames) / sizeof(struct StdReg);
}

