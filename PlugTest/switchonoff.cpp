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
    ui->delayBox->setValue(mItem->delay);
}

void SwitchOnOff::get()
{
    mItem = ConfigBase::bulid()->item;
    mItem->testIp = ui->testIPEdit->text();
    mItem->oids[0] = ui->bitSwOidEdit1->text();
    mItem->oids[1] = ui->bitSwOidEdit2->text();
    mItem->oids[2] = ui->bitSwOidEdit3->text();
    mItem->oids[3] = ui->bitSwOidEdit4->text();

    mItem->readCurOids[0] = ui->bitCurOidEdit1->text();
    mItem->readCurOids[1] = ui->bitCurOidEdit1->text();
    mItem->readCurOids[2] = ui->bitCurOidEdit1->text();
    mItem->readCurOids[3] = ui->bitCurOidEdit1->text();
    mItem->bitNum = ui->numspinBox->value();
    mItem->delayOpen = ui->delayOpenspinBox->value();
    mItem->delayClose = ui->delayClosespinBox->value();
    mItem->delay = ui->delayBox->value();
}
