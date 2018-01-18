/**
 * file implements member functions
 * from devicebar.h
 * Author: Charlie Street
 */

#include "../../include/interface/devicebar.h"

/**
 * Constructor just initialises everything and organises the widget
 * @param parent the parent widget
 */
DeviceBar::DeviceBar(QWidget *parent):QWidget(parent) {
    setStyleSheet("background-image: url(\"C:/Users/charl/CLionProjects/FYP/images/dark-back.png\");"
                  "color: black");

    layout = new QHBoxLayout(this);

    devices = new QComboBox(this);
    title = new QLabel(this);

    title->setText("IntelliJam");

    devices->addItem("No Device Selected");

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
