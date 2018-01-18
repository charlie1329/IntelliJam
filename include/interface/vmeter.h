/**
  * file contains class definition,
  * constants and function protottypes for the drawing of
  * the volume meter
  * Author: Charlie Street
  */

#ifndef VMETER_H
#define VMETER_H

#include <QWidget>
#include <QPainter>

//colours
#define GREEN "#08c207"
#define YELLOW "#cbcc0f"
#define RED "#c60404"

#define MAX_ABS 0.5
#define TRANSPARENCY 0.3

#define NUM_GREEN 54
#define NUM_YELLOW 6
#define NUM_RED 13

/**
 * class represents a volume meter widget
 */
class VMeter: public QWidget {
private:
    double currentValue;

    /**
     * function paints the volume meter onto the window
     * called by paintEvent
     */
    void paintVMeter();

protected:
    void paintEvent(QPaintEvent *event);

public:
    VMeter(QWidget *parent = 0);
    void setNewValue(double newValue);

};

#endif // VMETER_H
