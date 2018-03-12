/**
 * file implements member functions
 * from devicebar.h
 * Author: Charlie Street
 */

#include "../../include/interface/devicebar.h"
#include <QPixmap>

/**
 * Constructor just initialises everything and organises the widget
 * @param parent the parent widget
 * @param bridge the bridge to the back-end
 */
DeviceBar::DeviceBar(QWidget *parent, shared_ptr<Bridge> bridge):QWidget(parent) {
    setStyleSheet("background-image: url(\"images/dark-back.png\");"
                  "color: black");

    layout = new QHBoxLayout(this);

    devices = new QComboBox(this);

    devices->setStyleSheet("QComboBox QListView{background: white;}");

    title = new QLabel(this);

    QPixmap image("images/logo_small.png");
    title->setPixmap(image);

    devices->addItem("No Device Selected");

    //add the found devices to the drop down menu
    vector<string> deviceNames = bridge->getDevices();
    for(const auto &d : deviceNames) {
        devices->addItem(QString::fromStdString(d));
    }

    layout->addWidget(title,90);
    layout->addWidget(devices,10);

    setLayout(layout);
}

/**
 * destructor deallocates any heap allocated memory
 */
DeviceBar::~DeviceBar() {
    delete devices;
    delete title;
    delete layout;
}

/**
 * function returns the currently selected device from the drop down menu
 * @return currently selected device
 */
string DeviceBar::getSelectedDevice() {
    return devices->currentText().toStdString();
}

