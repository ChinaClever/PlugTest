#ifndef DISPLAYWID_H
#define DISPLAYWID_H

#include "comtablewid.h"
#include "datapackets.h"

class DisplayWid : public ComTableWid
{
    Q_OBJECT
public:
    DisplayWid(QWidget *parent = nullptr);

protected:
    void initWid();
    void updateData(int id);

protected slots:
   virtual void timeoutDone();

private:

};

#endif // DISPLAYWID_H
