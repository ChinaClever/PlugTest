#ifndef CONFIGBASH
#define CONFIGBASH
#include "sysconfigfile.h"

#define SNMP_SIZE 4
#define RTUCMD_SIZE 4

struct sConfigItem
{
    QString swIp;
    QString testIp;
    int delayClose;
    int delayOpen;
    int bitNum;

    QString openCmd[RTUCMD_SIZE];
    QString closeCmd[RTUCMD_SIZE];

    int snmpEn[SNMP_SIZE];
    QString oids[SNMP_SIZE];
    QString readVolOids[SNMP_SIZE];
    QString readCurOids[SNMP_SIZE];
    QString writeCrMinVolOids[SNMP_SIZE];
    QString writeMinVolOids[SNMP_SIZE];
};

class ConfigBase
{
    ConfigBase();
public:
    static ConfigBase *bulid();

    sConfigItem *item;
    CfgCom *mCfg;

    void save();
protected:
    int getSnmpEn(int id);
    void setSnmpEn(int id, int s);

    QString getOid(int id);
    void setOid(int id, const QString &url);

    QString getReadVolOids(int id);
    void setReadVolOids(int id, const QString &url);

    QString getReadCurOids(int id);
    void setReadCurOids(int id , const QString &url);

    QString getWriteCrMinVolOids(int id);
    void setWriteCrMinVolOids(int id , const QString &url);

    QString getWriteMinVolOids(int id);
    void setWriteMinVolOids(int id , const QString &url);

    QString getSwIp();
    void setSwIp(const QString &ip);

    QString getTestIp();
    void setTestIp(const QString &ip);

    int getCloseTime();
    void setCloseTime(int s);

    int getOpenTime();
    void setOpenTime(int s);

    int getBitNum();
    void setBitNum(int s);

    void write(const QString &key, const QVariant& v, const QString &g="cfg");
    QVariant read(const QString &key, const QVariant &v = QVariant(), const QString &g="cfg");
    QString getPrefix(){return "cfg";}
};

#endif // CONFIGBASH
