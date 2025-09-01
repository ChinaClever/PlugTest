#ifndef DATAPACKET_H
#define DATAPACKET_H
#include <QtCore>
#include <QColor>

#define ARRAY_SIZE 4 //一包数据最长

/**
 * 数据包
 */
struct sDataPacket
{
    bool en;
    uchar action;
    uint open;
    uint close;
    uint ok;
    uint err;
    uint value;
};




#endif // DATAPACKET_H
