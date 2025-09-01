#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plugthread.h"
#include "devonoff.h"
#include "switchonoff.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool inputCheck();

private slots:
    void on_startBtn_clicked();
    void on_overBtn_clicked();

    void on_modeBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    PlugThread *mPlugThread;
    DevOnOff *mDevOnOffWid;
    SwitchOnOff *mSwOnOffWid;
    int mMode;
    sConfigItem *mItem;
};

#endif // MAINWINDOW_H
