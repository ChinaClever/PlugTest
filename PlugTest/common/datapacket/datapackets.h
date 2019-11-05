#ifndef DATAPACKETS_H
#define DATAPACKETS_H

#include <QObject>
#include "datapacket.h"

class DataPackets
{
public:
    static DataPackets *bulid();
    sDataPacket *get(int devNum);
    void clear(int id);
    void clears();

private:
    sDataPacket mPackets[ARRAY_SIZE];
};

#endif // DATAPACKETS_H
