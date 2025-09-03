/*
 * 配置文件公共基类
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "configbase.h"

ConfigBase::ConfigBase()
{
    mCfg = CfgCom::bulid();
    item = new sConfigItem();
    item->swIp = getSwIp();
    item->testIp = getTestIp();

    item->delayClose = getCloseTime();
    item->delayOpen = getOpenTime();
    item->bitNum = getBitNum();
    item->delay = getDelayTime();

    for(int i=0; i<SNMP_SIZE; ++i) {
        item->readVolOids[i] = getReadVolOids(i);
        item->readCurOids[i] = getReadCurOids(i);
        item->writeMinVolOids[i] = getWriteMinVolOids(i);
        item->writeCrMinVolOids[i] = getWriteCrMinVolOids(i);
        item->oids[i] = getOid(i);
    }
}

ConfigBase *ConfigBase::bulid()
{
    static ConfigBase* sington = nullptr;
    if(sington == nullptr)
        sington = new ConfigBase();
    return sington;
}

void ConfigBase::save()
{
    setSwIp(item->swIp);
    setTestIp(item->testIp);
    setBitNum(item->bitNum);

    setCloseTime(item->delayClose);
    setOpenTime(item->delayOpen);
    setDelayTime(item->delay);

    for(int i=0; i<SNMP_SIZE; ++i) {
        setReadVolOids(i , item->readVolOids[i]);
        setReadCurOids(i , item->readCurOids[i]);
        setWriteMinVolOids(i , item->writeMinVolOids[i]);
        setWriteCrMinVolOids(i , item->writeCrMinVolOids[i]);
        setOid(i, item->oids[i]);
    }
}

void ConfigBase::setSnmpEn(int id, int s)
{
    QString prefix = getPrefix();
    QString str = QString("%1_snmp_en_%2").arg(prefix).arg(id);
    write(str, QString::number(s), prefix);
}

int ConfigBase::getSnmpEn(int id)
{
    QString prefix = getPrefix();
    QString str = QString("%1_snmp_en_%2").arg(prefix).arg(id);
    int ret = read(str, 1 , prefix).toInt();
    if(ret <= 0)  ret = 0;
    return ret;
}

QString ConfigBase::getOid(int id)
{
    QString prefix = getPrefix();
    QString str = QString("%1_snmp_oid_%2").arg(prefix).arg(id);
    return  read(str,"",prefix).toString();
}


void ConfigBase::setOid(int id, const QString &url)
{
    QString prefix = getPrefix();
    QString str = QString("%1_snmp_oid_%2").arg(prefix).arg(id);
    write(str, url, prefix);
}

void ConfigBase::setTestIp(const QString &ip)
{
    QString prefix = getPrefix();
    QString str = QString("%1_test_ip_addr").arg(prefix);
    write(str, ip, prefix);
}

QString ConfigBase::getTestIp()
{
    QString prefix = getPrefix();
    QString str = QString("%1_test_ip_addr").arg(prefix);
    return read(str,"192.168.1.164",prefix).toString();
}

QString ConfigBase::getSwIp()
{
    QString prefix = getPrefix();
    QString str = QString("%1_sw_ip_addr").arg(prefix);
    return read(str,"192.168.1.163",prefix).toString();
}

void ConfigBase::setSwIp(const QString &ip)
{
    QString prefix = getPrefix();
    QString str = QString("%1_sw_ip_addr").arg(prefix);
    write(str, ip, prefix);
}

QString ConfigBase::getReadVolOids(int id)
{
    QString prefix = getPrefix();
    QString str = QString("%1_read_Vol_oid_%2").arg(prefix).arg(id);
    return read(str,"", prefix).toString();
}


void ConfigBase::setReadVolOids(int id , const QString &url)
{
    QString prefix = getPrefix();
    QString str = QString("%1_read_Vol_oid_%2").arg(prefix).arg(id);
    write(str, url, prefix);
}

QString ConfigBase::getReadCurOids(int id)
{
    QString prefix = getPrefix();
    QString str = QString("%1_read_Cur_oid_%2").arg(prefix).arg(id);
    return read(str,"", prefix).toString();
}


void ConfigBase::setReadCurOids(int id , const QString &url)
{
    QString prefix = getPrefix();
    QString str = QString("%1_read_Cur_oid_%2").arg(prefix).arg(id);
    write(str, url, prefix);
}

QString ConfigBase::getWriteCrMinVolOids(int id)
{
    QString prefix = getPrefix();
    QString str = QString("%1_write_CrMin_Vol_oid_%2").arg(prefix).arg(id);
    return read(str,"",prefix).toString();
}


void ConfigBase::setWriteCrMinVolOids(int id , const QString &url)
{
    QString prefix = getPrefix();
    QString str = QString("%1_write_CrMin_Vol_oid_%2").arg(prefix).arg(id);
    write(str, url, prefix);
}

QString ConfigBase::getWriteMinVolOids(int id)
{
    QString prefix = getPrefix();
    QString str = QString("%1_write_Min_Vol_oid_%2").arg(prefix).arg(id);
    return read(str,"",prefix).toString();
}


void ConfigBase::setWriteMinVolOids(int id , const QString &url)
{
    QString prefix = getPrefix();
    QString str = QString("%1_write_Min_Vol_oid_%2").arg(prefix).arg(id);
    write(str, url, prefix);
}

int ConfigBase::getCloseTime()
{
    QString prefix = getPrefix();
    QString str = QString("%1_Close_time").arg(prefix);
    int ret = read(str, 5 ,prefix).toInt();
    return ret;
}

/**
 * @brief 设置延时后关闭继电器时间
 * @param num
 */
void ConfigBase::setCloseTime(int s)
{
    QString prefix = getPrefix();
    QString str = QString("%1_Close_time").arg(prefix);
    write(str, QString::number(s), prefix);
}

int ConfigBase::getDelayTime()
{
    QString prefix = getPrefix();
    QString str = QString("%1_Delay_time").arg(prefix);
    int ret = read(str, 1 ,prefix).toInt();
    return ret;
}

/**
 * @brief 设置读取状态延时时间
 * @param num
 */
void ConfigBase::setDelayTime(int s)
{
    QString prefix = getPrefix();
    QString str = QString("%1_Delay_time").arg(prefix);
    write(str, QString::number(s), prefix);
}

int ConfigBase::getOpenTime()
{
    QString prefix = getPrefix();
    QString str = QString("%1_Open_time").arg(prefix);
    int ret = read(str, 5, prefix).toInt();
    return ret;
}

/**
 * @brief 设置延时后开启继电器时间
 * @param num
 */
void ConfigBase::setOpenTime(int s)
{
    QString prefix = getPrefix();
    QString str = QString("%1_Open_time").arg(prefix);
    write(str, QString::number(s), prefix);
}

int ConfigBase::getBitNum()
{
    QString prefix = getPrefix();
    QString str = QString("%1_BitNum").arg(prefix);
    int ret = read(str,4, prefix).toInt();
    return ret;
}

/**
 * @brief 设置输出位数量
 * @param num
 */
void ConfigBase::setBitNum(int s)
{
    QString prefix = getPrefix();
    QString str = QString("%1_BitNum").arg(prefix);
    write(str, QString::number(s), prefix);
}

/**
 * @brief 根据名称获取配置文件数值
 * @return 对应的配置文件数值
 */
QVariant ConfigBase::read(const QString &key, const QVariant &v, const QString &g)
{
    return mCfg->read(key, v, g);
}

/**
 * @brief 设置对应名称的配置文件数值
 * @param value
 * @param name
 */
void ConfigBase::write(const QString &key, const QVariant& v, const QString &g)
{
    mCfg->write(key, v, g);
}
