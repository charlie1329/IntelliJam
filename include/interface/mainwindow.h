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
    Q_OBJECT
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


private slots:
    //slot functions for each of the buttons
    void playPressed();
    void stopPressed();

protected:
    //let me execute some clean up on the closing of the system
    void closeEvent(QCloseEvent *event) override;

public:
    //constructor/destructor
    explicit MainWindow(QWidget *parent = nullptr, shared_ptr<Bridge> newBridge = nullptr);
    ~MainWindow() override;

};

#endif // MAINWINDOW_H
