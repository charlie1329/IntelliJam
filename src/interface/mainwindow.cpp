/**
  * file implements functions from mainwindow.h
  * Author: Charlie Street
  */

#include <utility>
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>

#include "../../include/interface/mainwindow.h"

/**
 * constructor for the MainWindow
 * basically brings everything together
 * to form the overall GUI
 * @param parent the parent widget
 * @param bridge the bridge to the backend
 */
MainWindow::MainWindow(QWidget *parent, shared_ptr<Bridge> newBridge) : QWidget(parent), bridge(std::move(newBridge)){

    //set size and title of window
    resize(640,380);
    setWindowTitle("IntelliJam");

    //set background colour of window
    QPixmap bkgnd("images/slate-back.png");
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    vbox = new QVBoxLayout(this);

    userBox = new QHBoxLayout();
    aiBox = new QHBoxLayout();

    piano = new Piano(this);
    vmeter = new VMeter(this);

    userTile = new NameTile(this,true,false);
    aiTile = new NameTile(this,false,true);

    devBar = new DeviceBar(this,bridge);

    controlBar = new ControlBar(this);

    userBox->addWidget(userTile,25);
    userBox->addWidget(vmeter,75);

    aiBox->addWidget(aiTile,25);
    aiBox->addWidget(piano,75);

    vbox->setMargin(0);
    vbox->addWidget(devBar,5);
    vbox->addLayout(userBox,35);
    vbox->addLayout(aiBox,35);
    vbox->addWidget(controlBar,10);
    setLayout(vbox);

    //allows me to mess with the painted GUI components
    bridge->setTiles(userTile,aiTile);
    bridge->setVMeter(vmeter);
    bridge->setPiano(piano);

    //add connections to the buttons
    connect(controlBar->getPlayButton(),SIGNAL(released()),this, SLOT(playPressed()));
    connect(controlBar->getStopButton(),SIGNAL(released()),this, SLOT(stopPressed()));
    connect(controlBar->getFileButton(),SIGNAL(released()),this, SLOT(filePressed()));

    //add connection from file button to now playing bar
    connect(this,SIGNAL(trackChanged(QString)),controlBar->getPlayBar(), SLOT(newTrackSlot(QString)));

    //TODO: Make logo!: this->setWindowIcon();

}

/**
 * destructor deletes any heap allocated fields
 */
MainWindow::~MainWindow() {
    delete piano;
    delete vmeter;
    delete userTile;
    delete aiTile;
    delete devBar;
    delete controlBar;
    delete userBox;
    delete aiBox;
    delete vbox;
}

/**
 * overwrites the function from QWidget
 * @param event the event occurring
 */
void MainWindow::closeEvent(QCloseEvent *event) {

    bridge->closeApp(); //close up all the back end
    //no need for error checking here, as we're going to shut down the system anyway

    QWidget::closeEvent(event);
}

/**
 * function deals with what to do when the play button is pressed
 * in the interface
 */
void MainWindow::playPressed() {

    if(!userTile->getActive()) bridge->switchPlayer(); //make sure on user player

    controlBar->getPlayButton()->setEnabled(false); //deactivate play button
    controlBar->getFileButton()->setEnabled(false); //deactivate browse button too while we're at it

    string deviceToUse = devBar->getSelectedDevice(); //which input device to use

    bridge->startApp(deviceToUse); //try to stop the process off

    //do some error checking
    if(bridge->getErr() != noError) {
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Error Starting System", "An error occurred during the system start-up.\n "
                "Please try again, or restart the system.");
        messageBox.setFixedSize(500, 200);
        messageBox.show();

        controlBar->getPlayButton()->setEnabled(true); //allow the button to be used again as there was an error
        controlBar->getFileButton()->setEnabled(true);
    }
}

/**
 * function deals with functionality when stop button is pressed
 * in the interface
 */
void MainWindow::stopPressed() {

    controlBar->getStopButton()->setEnabled(false); //don't allow multiple presses in time taken to stop application

    bridge->stopApp(); //stop the back-end and wait to finish

    //do some error checking
    if(bridge->getErr() != noError) {
        QMessageBox messageBox;
        messageBox.critical(nullptr, "Error Starting System", "An error occurred during the system start-up.\n"
                "The behaviour from the system may no longer be as expected.");
        messageBox.setFixedSize(500,200);
        messageBox.show();
    }

    //reactivate usage of all buttons
    controlBar->getStopButton()->setEnabled(true);
    controlBar->getPlayButton()->setEnabled(true);
    controlBar->getFileButton()->setEnabled(true);
}

/**
 * function deals with what happens when the file browse button
 * is pressed on the interface
 */
void MainWindow::filePressed() {

    //deactivate the button while file being selected
    controlBar->getFileButton()->setEnabled(false);

    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Select Backing Track"), "/", tr("Wave Files (*.wav)"));

    emit trackChanged(fileName);

    //reactivate the button now we're done
    controlBar->getFileButton()->setEnabled(true);

}