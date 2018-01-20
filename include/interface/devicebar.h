/**
  * file defines class for widget
  * which shows device options
  * Author: Charlie Street
  */

#ifndef DEVICEBAR_H
#define DEVICEBAR_H

#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include <memory>

#include "../bridge/bridge.h"

/**
 * Class represents the bar which provides device selection
 * in the system
 */
class DeviceBar: public QWidget {
private:
    QComboBox *devices;
    QLabel *title;
    QHBoxLayout *layout;

public:

    //constructor/destructor
    explicit DeviceBar(QWidget *parent = nullptr, shared_ptr<Bridge> bridge = nullptr);
    ~DeviceBar() override;

    //retrieve value from combo box
    string getSelectedDevice();
};

#endif // DEVICEBAR_H
