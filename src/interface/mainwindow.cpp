/**
  * file implements functions from mainwindow.h
  * Author: Charlie Street
  */

#include "../../include/interface/mainwindow.h"

/**
 * constructor for the MainWindow
 * basically brings everything together
 * to form the overall GUI
 * @param parent the parent widget
 * @param bridge the bridge to the backend
 */
MainWindow::MainWindow(QWidget *parent, shared_ptr<Bridge> newBridge) : QWidget(parent), bridge(newBridge){

    //set size and title of window
    resize(640,380);
    setWindowTitle("IntelliJam");

    //set background colour of window
    QPixmap bkgnd("C:/Users/charl/CLionProjects/FYP/images/slate-back.png");
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

    devBar = new DeviceBar(this);

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

    QWidget::closeEvent(event);
}