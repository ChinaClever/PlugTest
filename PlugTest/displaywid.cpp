#include "displaywid.h"

DisplayWid::DisplayWid(int mode, QWidget *parent) : ComTableWid(parent)
{
    mMode = mode;
    if(mode == 1) initVolWid();
    else initCurWid();
}


void DisplayWid::initVolWid()
{
    QStringList header;
    header << tr("编号") << tr("动态") << tr("电压值") << tr("开次数")<< tr("关次数") << tr("成功次数") << tr("失败次数");

    QString title = tr("测试数据列表");
    initTableWid(header, 1, title);
}

void DisplayWid::initCurWid()
{
    QStringList header;
    header << tr("编号") << tr("动态") << tr("电流值") << tr("开次数")<< tr("关次数") << tr("成功次数") << tr("失败次数");

    QString title = tr("测试数据列表");
    initTableWid(header, ARRAY_SIZE, title);
}


void DisplayWid::updateData(int id)
{
    QStringList list;
    list << QString::number(id+1);
    sDataPacket *packet = DataPackets::bulid()->get(id);
    if(packet->en) {
        if(packet->action) {
            list << tr("开启");
        } else {
            list << tr("关闭");
        }
        if ( 0 == mMode) list << QString::number(packet->value/10.0,'f',1);
        else list << QString::number(packet->value/100.0,'f',2);
        list << QString::number(packet->open);
        list << QString::number(packet->close);
        list << QString::number(packet->ok);
        list << QString::number(packet->err);
        setTableRow(id, list);
    }else {
        clearRow(id);
        setTableRow(id, list);
    }
}


void DisplayWid::timeoutDone()
{
    for(int i=0; i<ARRAY_SIZE; ++i) {
        updateData(i);
    }
}

