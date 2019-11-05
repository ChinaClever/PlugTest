#include "displaywid.h"

DisplayWid::DisplayWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
}


void DisplayWid::initWid()
{
    QStringList header;
    header << tr("编号") << tr("动态") << tr("状态值") << tr("拔插次数") << tr("成功次数") << tr("失败次数");

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
            list << tr("插入");
        } else {
            list << tr("拔出");
        }
        list << QString::number(packet->value);
        list << QString::number(packet->all);
        list << QString::number(packet->ok);
        list << QString::number(packet->err);
        setTableRow(id, list);
    } else {
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

