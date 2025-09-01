#include "switchonoff.h"
#include "ui_switchonoff.h"

SwitchOnOff::SwitchOnOff(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwitchOnOff)
{
    ui->setupUi(this);
    mDisplayWid = new DisplayWid(2 , ui->widget);
    init();
}

SwitchOnOff::~SwitchOnOff()
{
    delete ui;
}

void SwitchOnOff::init()
{
    mItem = ConfigBase::bulid()->item;
    ui->testIPEdit->setText(mItem->testIp);
    ui->bitSwOidEdit1->setText(mItem->oids[0]);
    ui->bitSwOidEdit2->setText(mItem->oids[1]);
    ui->bitSwOidEdit3->setText(mItem->oids[2]);
    ui->bitSwOidEdit4->setText(mItem->oids[3]);

    ui->bitCurOidEdit1->setText(mItem->readCurOids[0]);
    ui->bitCurOidEdit1->setText(mItem->readCurOids[1]);
    ui->bitCurOidEdit1->setText(mItem->readCurOids[2]);
    ui->bitCurOidEdit1->setText(mItem->readCurOids[3]);
    ui->numspinBox->setValue(mItem->bitNum);
    ui->delayOpenspinBox->setValue(mItem->delayOpen);
    ui->delayClosespinBox->setValue(mItem->delayClose);
}
