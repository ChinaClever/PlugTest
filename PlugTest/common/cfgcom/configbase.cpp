/*
 * 配置文件公共基类
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "configbase.h"

ConfigBase::ConfigBase()
{
    item = new sConfigItem();
    item->ip = getIp();
    item->com = getCom();
    item->openCmd = getOpenCmd();
    item->closeCmd = getCloseCmd();
    item->delay = getPushTime();

    for(int i=0; i<SNMP_SIZE; ++i) {
        item->snmpEn[i] = getSnmpEn(i);
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
    setIp(item->ip);
    setCom(item->com);
    setOpenCmd(item->openCmd);
    setCloseCmd(item->closeCmd);
    setPushTime(item->delay);

    for(int i=0; i<SNMP_SIZE; ++i) {
        setSnmpEn(i, item->snmpEn[i]);
        setOid(i, item->oids[i]);
    }
}

void ConfigBase::setSnmpEn(int id, int s)
{
    QString prefix = getPrefix();
    QString str = QString("%1_snmp_en_%2").arg(prefix).arg(id);
    com_cfg_writeParam(str, QString::number(s), prefix);
}

int ConfigBase::getSnmpEn(int id)
{
    QString prefix = getPrefix();
    QString str = QString("%1_snmp_en_%2").arg(prefix).arg(id);
    int ret = com_cfg_readInt(str, prefix);
    if(ret <= 0)  ret = 8080;
    return ret;
}

QString ConfigBase::getOid(int id)
{
    QString prefix = getPrefix();
    QString str = QString("%1_snmp_oid_%2").arg(prefix).arg(id);
    return  com_cfg_readStr(str, prefix);
}


void ConfigBase::setOid(int id, const QString &url)
{
    QString prefix = getPrefix();
    QString str = QString("%1_snmp_oid_%2").arg(prefix).arg(id);
    com_cfg_writeParam(str, url, prefix);
}


QString ConfigBase::getCom()
{
    QString prefix = getPrefix();
    QString str = QString("%1_com_name").arg(prefix);
    return com_cfg_readStr(str, prefix);
}


void ConfigBase::setCom(const QString &name)
{
    QString prefix = getPrefix();
    QString str = QString("%1_com_name").arg(prefix);
    com_cfg_writeParam(str, name, prefix);
}



QString ConfigBase::getOpenCmd()
{
    QString prefix = getPrefix();
    QString str = QString("%1_open_cmd").arg(prefix);
    return com_cfg_readStr(str, prefix);
}


void ConfigBase::setOpenCmd(const QString &cmd)
{
    QString prefix = getPrefix();
    QString str = QString("%1_open_cmd").arg(prefix);
    com_cfg_writeParam(str, cmd, prefix);
}



QString ConfigBase::getCloseCmd()
{
    QString prefix = getPrefix();
    QString str = QString("%1_close_cmd").arg(prefix);
    return com_cfg_readStr(str, prefix);
}


void ConfigBase::setCloseCmd(const QString &cmd)
{
    QString prefix = getPrefix();
    QString str = QString("%1_close_cmd").arg(prefix);
    com_cfg_writeParam(str, cmd, prefix);
}


QString ConfigBase::getIp()
{
    QString prefix = getPrefix();
    QString str = QString("%1_ip_addr").arg(prefix);
    return com_cfg_readStr(str, prefix);
}


void ConfigBase::setIp(const QString &ip)
{
    QString prefix = getPrefix();
    QString str = QString("%1_ip_addr").arg(prefix);
    com_cfg_writeParam(str, ip, prefix);
}


int ConfigBase::getPushTime()
{
    QString prefix = getPrefix();
    QString str = QString("%1_push_time").arg(prefix);
    int ret = com_cfg_readInt(str, prefix);
    if(ret <= 0)  ret = 10;
    return ret;
}

/**
 * @brief 设置日志存储时间间隔
 * @param num
 */
void ConfigBase::setPushTime(int s)
{
    QString prefix = getPrefix();
    QString str = QString("%1_push_time").arg(prefix);
    com_cfg_writeParam(str, QString::number(s), prefix);
}
