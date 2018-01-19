/**
  * Header file for the main window of the GUI
  * Brings all of the widgets together to form the overall gui
  * Author: Charlie Street
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "piano.h"
#include "vmeter.h"
#include "nametile.h"
#include "devicebar.h"
#include "controlbar.h"

#include "../bridge/bridge.h"
#include <memory>

#include <QWidget>
#include <QVBoxLayout>

/**
 * class represents the main window of the GUI
 */
class MainWindow : public QWidget {
private:
    shared_ptr<Bridge> bridge;
    Piano *piano;
    VMeter *vmeter;
    NameTile *userTile;
    NameTile *aiTile;
    DeviceBar *devBar;
    ControlBar *controlBar;
    QVBoxLayout *vbox;
    QHBoxLayout *userBox;
    QHBoxLayout *aiBox;
protected:
    void closeEvent(QCloseEvent *event);
public:
    MainWindow(QWidget *parent = 0, shared_ptr<Bridge> newBridge = nullptr);
    ~MainWindow();

};

#endif // MAINWINDOW_H
