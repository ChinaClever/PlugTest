#ifndef CONFIGBASH
#define CONFIGBASH
#include "sysconfigfile.h"

#define SNMP_SIZE 3
#define RTUCMD_SIZE 4

struct sConfigItem
{
    QString ip;
    QString com; // 串口名
    int delay;

    QString openCmd[RTUCMD_SIZE];
    QString closeCmd[RTUCMD_SIZE];
    int rtuCmdEn[RTUCMD_SIZE];

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

    int getRtuEn(int id);
    void setRtuEn(int id, int s);

    QString getOid(int id);
    void setOid(int id, const QString &url);

    QString getCom();
    void setCom(const QString &name);

    QString getOpenCmd(int index);
    void setOpenCmd(const QString &cmd, int index);

    QString getCloseCmd(int index);
    void setCloseCmd(const QString &cmd, int index);

    QString getIp();
    void setIp(const QString &ip);

    int getPushTime();
    void setPushTime(int s);

    virtual QString getPrefix(){return "con";}
};

#endif // CONFIGBASH
