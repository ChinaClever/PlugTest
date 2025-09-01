#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    mPlugThread = new PlugThread(this);
    QString str = tr("整机通断电测试");
    mDevOnOffWid = new DevOnOff(ui->tabWidget);
    ui->tabWidget->addTab(mDevOnOffWid, str);

    str = tr("开关通断测试");
    mSwOnOffWid = new SwitchOnOff(ui->tabWidget);
    ui->tabWidget->addTab(mSwOnOffWid, str);
    mMode = 0;

    mItem = mPlugThread->mItem;
    mConfig =ConfigBase::bulid();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::inputCheck()
{
    if(0 == mMode){
        QString str = mItem->swIp;
        if(!cm_isIPaddress(str)) {
            CriticalMsgBox box(this, tr("开关PDU IP地址设置错误"));
            return false;
        }
        str = mItem->testIp;
        if(!cm_isIPaddress(str)) {
            CriticalMsgBox box(this, tr("测试PDU IP地址设置错误"));
            return false;
        }
    }else{
        QString str = mItem->testIp;
        if(!cm_isIPaddress(str)) {
            CriticalMsgBox box(this, tr("开关PDU IP地址设置错误"));
            return false;
        }
    }

    return true;
}

void MainWindow::on_startBtn_clicked()
{
    mDevOnOffWid->get();
    mSwOnOffWid->get();
    mConfig->save();
    if(inputCheck()) {
        mPlugThread->startThread(mMode);
    }
}

void MainWindow::on_overBtn_clicked()
{
    mPlugThread->quitThread();
}

void MainWindow::on_modeBox_currentIndexChanged(int index)
{
    mMode = index;
}
