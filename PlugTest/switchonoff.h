#ifndef SWITCHONOFF_H
#define SWITCHONOFF_H

#include <QWidget>
#include "displaywid.h"
#include "common/cfgcom/configbase.h"
namespace Ui {
class SwitchOnOff;
}

class SwitchOnOff : public QWidget
{
    Q_OBJECT

public:
    explicit SwitchOnOff(QWidget *parent = nullptr);
    ~SwitchOnOff();
    void init();

private:
    Ui::SwitchOnOff *ui;
    DisplayWid *mDisplayWid;
    sConfigItem *mItem;
};

#endif // SWITCHONOFF_H
