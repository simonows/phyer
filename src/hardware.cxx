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
#include <fstream>
#include <string>
#include <cstdlib>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <phyer/hardware.h>
#include <phyer/qtableviewmodel.h>

using namespace std;

Hardware::Hardware()
{
    items << "Device1";
    items << "Device2";
    items << "Device3";
}

Hardware::~Hardware()
{
#ifndef _WIN32
    //freeifaddrs(addrs);
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


static struct StdReg const stdRegNamesPage1[] = {
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

static struct StdReg const stdRegNamesPage2[] = {
    { 0, "Fiber Control Register", "rw", "desc1" }
  , { 1, "Fiber Status Register", "ro", "desc2" }
  , { 2, "PHY Identifier", "ro", "desc3" }
  , { 3, "PHY Identifier", "ro", "desc4" }
  , { 4, "Fiber Auto-Negotiation Advirtisement Register", "rw", "desc5" }
  , { 5, "Auto-Negotiation Link Partner Base Page Ability", "ro", "desc6" }
  , { 6, "Reserved (MMD control)", "rw", "desc7" }
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

static struct StdReg const stdRegNamesPage3[] = {
    { 0, "PHY Identifier", "rw", "desc1" }
  , { 1, "Fiber Status Register", "ro", "desc2" }
  , { 2, "PHY Identifier", "ro", "desc3" }
  , { 3, "PHY Identifier", "ro", "desc4" }
  , { 4, "Fiber Auto-Negotiation Advirtisement Register", "rw", "desc5" }
  , { 5, "Auto-Negotiation Link Partner Base Page Ability", "ro", "desc6" }
  , { 6, "Reserved (MMD control)", "rw", "desc7" }
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

static struct StdReg const stdRegNamesPage4[] = {
    { 0, "Fiber Control Register", "rw", "desc1" }
  , { 1, "Fiber Status Register", "ro", "desc2" }
  , { 2, "PHY Identifier", "ro", "desc3" }
  , { 3, "PHY Identifier", "ro", "desc4" }
  , { 4, "Fiber Auto-Negotiation Advirtisement Register", "rw", "desc5" }
  , { 5, "Auto-Negotiation Link Partner Base Page Ability", "ro", "desc6" }
  , { 6, "Reserved (MMD control)", "rw", "desc7" }
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

static struct StdReg const stdRegNamesPage5[] = {
    { 0, "Fiber Control Register", "rw", "desc1" }
  , { 1, "Fiber Status Register", "ro", "desc2" }
  , { 2, "PHY Identifier", "ro", "desc3" }
  , { 3, "PHY Identifier", "ro", "desc4" }
  , { 4, "Fiber Auto-Negotiation Advirtisement Register", "rw", "desc5" }
  , { 5, "Auto-Negotiation Link Partner Base Page Ability", "ro", "desc6" }
  , { 6, "Reserved (MMD control)", "rw", "desc7" }
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




static struct StdReg const *ctrl1[] = {
    stdRegNamesPage1
  , stdRegNamesPage2
  , stdRegNamesPage3
  , stdRegNamesPage4
  , stdRegNamesPage5
};

static struct StdReg const *ctrl2[] = {
    stdRegNamesPage1
  , stdRegNamesPage2
  , stdRegNamesPage3
  , stdRegNamesPage4
  , stdRegNamesPage5
};

static struct StdReg const *ctrl3[] = {
    stdRegNamesPage1
  , stdRegNamesPage2
  , stdRegNamesPage3
  , stdRegNamesPage4
  , stdRegNamesPage5
};

static struct StdReg const **devs[] = {
    ctrl1
  , ctrl2
  , ctrl3
};

void Hardware::ser(QString name, QList<RegisterFlag> items)
{
    QFile file(name);
    file.open(QIODevice::WriteOnly);
    QTextStream writeStream(&file);

    for (size_t i = 0; i < items.count(); i++)
    {
        writeStream << QString::asprintf("%ld@%s@%s@%s@%s\n"
          , items[i].getId()
          , items[i].getName().toLocal8Bit().data()
          , items[i].getValue().toLocal8Bit().data()
          , items[i].getEvalue().toLocal8Bit().data()
          , items[i].getDesc().toLocal8Bit().data()
        );
    }

    file.close();
}

QList<RegisterFlag> Hardware::des(QString item)
{
    QList<RegisterFlag> set;
    char buffer[10240] = { 0 };
    char *temp = nullptr;
    unsigned long id = 0;
    QString value;
    QString evalue;
    QString name;
    QString desc;

    ifstream file(item.toStdString());
    if (!file.is_open())
    {
        goto exit;
    }

    while (!file.eof())
    {
        file.getline(buffer, 10240);
        if (!buffer[0])
        {
            break;
        }
        temp = strtok(buffer, "@");
        temp = strtok(nullptr, "@");
        name = temp;
        temp = strtok(nullptr, "@");
        value = temp;
        temp = strtok(nullptr, "@");
        evalue = temp;
        temp = strtok(nullptr, "@");
        desc = temp;

        set.append(RegisterFlag(id, name, value, evalue, desc));
        id++;
    }
    file.close();

exit:
    return set;
}


QList<RegisterFlag> Hardware::getRegisterSet(int dev, int page)
{
    QList<RegisterFlag> set;
    struct StdReg const *ptr_page = devs[dev][page];

    if (page >= 5)
    {
        goto exit;
    }

    set = des(QString::asprintf("%s_%d.db", selected_item.toLocal8Bit().data(), page));
    if (set.count() != 0)
    {
        goto exit;
    }

    for (size_t i = 0; i < sizeof(stdRegNamesPage1) / sizeof(struct StdReg); i++)
    {
        set.append(RegisterFlag(
            ptr_page[i].addr
          , ptr_page[i].name
          , QString::asprintf("0x%04hX", 0)
          , ptr_page[i].status
          , ptr_page[i].desc
        ));
    }

exit:
    return set;
}

/*
int Device::reg_read_ext(unsigned pageNo, unsigned RN)
{
    uint16_t t;
    int ret = 0, ret2 = 0;

    if (model.contains("Intel"))
    {
        ret = reg_write(22, pageNo);
        if (ret < 0)
        {
            return ret;
        }
        return reg_read(RN);
    }
    else if (pageNo != 0)
    {
        ret = reg_read(0x0D);
        if (ret < 0)
        {
            goto out;
        }
        t = ret;
        t &= ~0xC000;
        t &= ~0x001F;
        t |= (pageNo) & 0x1f;
        ret = reg_write(0x0D, t);
        if (ret < 0)
            goto exit;
        ret = reg_write(0x0E, RN & 0xffff);
        QString ss =QString::number(RN & 0xffff);
        if (ret < 0)
            goto exit;
        ret = reg_write(0x0D, t | (1 << 14));
        if (ret < 0)
            goto exit;
        ret = reg_read(0x0E);
        if (ret < 0)
            goto exit;
        ret2 = reg_write(0x0D, 0);
        if (ret2 < 0)
            ret = ret2;
    } else
          ret = reg_read(RN);

exit:
    return ret;
}

int Device::reg_write_ext(unsigned pageNo,unsigned RN,unsigned value)
{
    uint16_t t;
        int ret = 0;
        if (model.contains("Intel"))
        {
            ret = reg_write(22, pageNo);
            if (ret < 0) return ret;
            return reg_write(RN, value);
        }
        else if (pageNo != 0) {
            ret = reg_read(0x0D);
            if (ret < 0)
            {
                goto out;
            }
            t = ret;
            t &= ~0xC000;
            t &= ~0x001F;
            t |= (pageNo) & 0x1f;
            ret = reg_write(0x0D, t);
            if (ret < 0)
                goto out;
            ret = reg_write(0x0E, RN & 0xffff);
            if (ret < 0)
                goto out;
            ret = reg_write(0x0D, t | (1 << 14));
            if (ret < 0)
                goto out;
            ret = reg_write(0x0E, value);
            if (ret < 0)
                goto out;
            ret = reg_write(0x0D, 0);
            goto out;
        } else {
            ret = reg_write(RN, value);
            goto out;
        }
        ret = 0;
    out:
        if (ret < 0) qDebug()<<"reg_write_ext ERROR\n";
        return ret;
}
*/


void Hardware::updatePage(int page, QList<RegisterFlag> items)
{
    ser(QString::asprintf("%s_%d.db", selected_item.toLocal8Bit().data(), page), items);
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
    return sizeof(stdRegNamesPage1) / sizeof(struct StdReg);
}

