/**
  * file defines class for widget
  * which shows device options
  * Author: Charlie Street
  */

#ifndef DEVICEBAR_H
#define DEVICEBAR_H

#define BACKGROUND_COLOUR "#535c50"

#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>

/**
 * Class represents
 */
class DeviceBar: public QWidget {
private:
    QComboBox *devices;
    QLabel *title;
    QHBoxLayout *layout;

public:
    DeviceBar(QWidget *parent);
    ~DeviceBar();
};

#endif // DEVICEBAR_H
