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
    const size_t num;
    const char *name;
    const unsigned long addr;
} StdReg;

#ifndef _WIN32
static struct StdReg const stdRegNames[] = {
    {0, "Control", MII_BMCR}
  , {1, "Status", MII_BMSR}
  , {2, "PHY Identifier", MII_PHYSID1}
  , {3, "PHY Identifier", MII_PHYSID2}
  , {4, "Auto-Negotiation Advertisement", MII_ADVERTISE}
  , {5, "Auto-Negotiation Link Partner Base Page Ability", MII_LPA}
  , {6, "Auto-Negotiation Expansion", MII_EXPANSION}
  , {7, "Auto-Negotiation Next Page Transmit", 7}
  , {8, "Auto-Negotiation Link Partner Received Next Page", 8}
  , {9, "1000BASE-T Control Register", MII_CTRL1000}
  , {10, "1000BASE-T Status Register", MII_STAT1000}
  , {11, "PSE Control register", MII_MMD_CTRL}
  , {12, "PSE/PD Status register", MII_MMD_DATA}
  , {13, "Reserved (MMD control)", MII_MMD_CTRL}
  , {14, "Reserved (MMD data)", MII_MMD_DATA}
  , {15, "Extended Status", MII_ESTATUS}
};
#else
static struct StdReg const stdRegNames[] = {
    {0, "Control", 0}
  , {1, "Status", 1}
  , {2, "PHY Identifier", 2}
  , {3, "PHY Identifier", 3}
  , {4, "Auto-Negotiation Advertisement", 4}
  , {5, "Auto-Negotiation Link Partner Base Page Ability", 5}
  , {6, "Auto-Negotiation Expansion", 6}
  , {7, "Auto-Negotiation Next Page Transmit", 7}
  , {8, "Auto-Negotiation Link Partner Received Next Page", 8}
  , {9, "1000BASE-T Control Register", 9}
  , {10, "1000BASE-T Status Register", 10}
  , {11, "PSE Control register", 11}
  , {12, "PSE/PD Status register", 12}
  , {13, "Reserved (MMD control)", 13}
  , {14, "Reserved (MMD data)", 14}
  , {15, "Extended Status", 15}
};
#endif

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
                    stdRegNames[i].num
                  , stdRegNames[i].name
                  , QString::asprintf("0x%04hX", mii->val_out)
                  , ""
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

