#ifndef CONFIGBASH
#define CONFIGBASH
#include "sysconfigfile.h"

#define SNMP_SIZE 3

struct sConfigItem
{
    QString ip;
    QString com; // 串口名
    int delay;

    QString openCmd;
    QString closeCmd;

    int snmpEn[SNMP_SIZE];
    QString oids[SNMP_SIZE];
};

class ConfigBase
{
    ConfigBase();
public:
    static ConfigBase *bulid();

    sConfigItem *item;

    void save();
protected:
    int getSnmpEn(int id);
    void setSnmpEn(int id, int s);

    QString getOid(int id);
    void setOid(int id, const QString &url);

    QString getCom();
    void setCom(const QString &name);

    QString getOpenCmd();
    void setOpenCmd(const QString &cmd);

    QString getCloseCmd();
    void setCloseCmd(const QString &cmd);

    QString getIp();
    void setIp(const QString &ip);

    int getPushTime();
    void setPushTime(int s);

    virtual QString getPrefix(){return "con";}
};

#endif // CONFIGBASH
